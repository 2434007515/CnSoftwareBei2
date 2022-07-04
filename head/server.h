//
// Created by 光球层上的黑子 on 2022/7/1.
//

#ifndef CNSOFTBEI_SERVER_H
#define CNSOFTBEI_SERVER_H

#include <iostream>
#include "asio.hpp"
#include "message.h"
#include "Calculator.h"
using namespace asio;
using namespace std;
class server
{
public:
    int port;
    io_service ios;
    ip::tcp::acceptor acceptor;
    char msgFromClient[256];
    char msgToClient[256];
    ip::tcp::socket sock;
    server(int p, condition_variable &cv, Calculator &calculator) : port(p),
                                                                    acceptor(ios, ip::tcp::endpoint(ip::tcp::v4(), port)),
                                                                    sock(ios)
    {
        std::thread th1([&]
                        {
                            acceptor.listen();
                            // acceptor.async_accept(std::bind(&server::acceptHandler,this,std::placeholders::_1));
                            acceptor.accept(sock);
                            while (1)
                            {
                                try
                                {
                                    read(sock, buffer(msgFromClient));
                                    message m(msgFromClient);
                                    // cout << "msg get from:" << sock.remote_endpoint().address() <<":"<<sock.remote_endpoint().port()<< endl;
                                    // cout << "head: " << m.getHead() << endl;
                                    // cout << "Body: " << m.getBody() << endl;
                                    // cout<<strcmp(m.getHead(),"load")<<endl;
                                    if (strcmp(m.getHead(),"load")==0)
                                    {
                                        //cout<<111<<endl;
                                        cv.notify_one();
                                        //cout << "load notify!" << endl;
                                    }
                                    else if (strcmp(m.getHead(),"start")==0)
                                    {
                                        cv.notify_one();
                                        // cout << "start notify!" << endl;
                                    }
                                    else if (strcmp(m.getHead(),"part")==0)
                                    {
                                        // cout<<"receive order:"<<m.getBody()<<"\n";
                                        auto temp=Utils::unZipPartString(m.getBody());
                                        calculator.reader.storePart(temp);
                                        // calculator.solveOrder(temp);
                                    }
                                    else if (strcmp(m.getHead(),"finish")==0)
                                    {
                                        if(calculator.is_notify)
                                          continue;
                                        // cout<<"get finish cnt:"<<m.getBody()<<"\n";
                                        if(calculator.checkFinish(m.getBody()))
                                        {
                                            calculator.sendFinish(m.getBody());
                                            calculator.is_notify=1;
                                            cv.notify_one();
                                            // cout<<"finish notify!"<<endl;
                                        }
                                        else if(calculator.is_finished)
                                        {
                                            calculator.sendFinish(m.getBody());
                                        }
                                    }
                                }
                                catch (std::system_error e)
                                {
                                    cout << "error!!!" << endl;
                                    cout << e.what() << endl;
                                    break;
                                }
                            } });
        th1.detach();
        cout << "server init over. port:" << port << endl;
    };
    //监听
    void listenThreadFunction(condition_variable &cv)
    {
        acceptor.listen();
        // acceptor.async_accept(std::bind(&server::acceptHandler,this,std::placeholders::_1));
        acceptor.accept(sock);
        while (1)
        {
            try
            {
                read(sock, buffer(msgFromClient));
                message m(msgFromClient);
                cout << sock.remote_endpoint().port() << endl;
                cout << "msg get:" << m.getSendMsg() << endl;
                cout << "head: " << m.getHead() << endl;
                cout << "Body: " << m.getBody() << endl;
                if (m.getHead() == "load")
                {
                    cv.notify_one();
                    cout << "notify!" << endl;
                }
            }
            catch (std::system_error e)
            {
                cout << "error!!!" << endl;
                cout << e.what() << endl;
                break;
            }
        }
    }
    void acceptHandler(const error_code &ec)
    {
        if (ec)
        {
            cout << ec.message() << endl;
        }
        else
        {
            try
            {
                cout << this->port << endl;
                read(sock, buffer(msgFromClient));
                cout << msgFromClient << endl;
            }
            catch (system_error e)
            {
                cout << e.what() << endl;
            }
        }
    }

    //    void establishServer(){
    //        acceptor.accept(sock);
    //        while (1){
    //            string tmp = string(msgFromClient) + "<< handled";
    //            strcpy(msgToClient, tmp.c_str());
    //            asio::write(sock,asio::buffer(msgToClient));
    //        }
    //    }
};

#endif // CNSOFTBEI_SERVER_H
