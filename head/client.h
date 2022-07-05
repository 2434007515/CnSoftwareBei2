//
// Created by 光球层上的黑子 on 2022/7/2.
//

#ifndef CNSOFTBEI_CLIENT_H
#define CNSOFTBEI_CLIENT_H
#include <iostream>
#include <mutex>
#include "asio.hpp"
#include "message.h"

using namespace asio;
using namespace std;
class client
{
public:
    int port;
    io_service ios;
    char msgToServer[42];
    char msgFromServer[42];
    ip::tcp::socket sock;
    ip::tcp::endpoint ep;
    std::mutex m_mtx;
    client(int p) : port(p), sock(ios), ep(ip::address::from_string("127.0.0.1"), p)
    {
        try
        {
            sock.connect(ep);
            cout << "connect to the server: " << ep << endl;
            // send(message(message::headEnum::order, "111"));
        }
        catch (const std::exception &e)
        {
            cout << "connect fail!" << endl;
            std::cerr << e.what() << '\n';
        }
    }
    void send(const string &s)
    {
        lock_guard lg(m_mtx);
        // m_mtx.lock();
        if (s.size() > 255)
        {
            cout << "to large msg" << endl;
        }
        strcpy(msgToServer, s.c_str());
        // asio::write(sock, asio::buffer(msgToServer));
        asio::async_write(sock, asio::buffer(msgToServer), [](const asio::error_code &ec, std::size_t bytes_transferred) {});
        // m_mtx.unlock();
    }
    void send(const message &msg)
    {
        lock_guard lg(m_mtx);
        // m_mtx.lock();
        strcpy(msgToServer, msg.getSendMsg());
        // asio::write(sock, asio::buffer(msgToServer));
        asio::async_write(sock, asio::buffer(msgToServer), [](const asio::error_code &ec, std::size_t bytes_transferred) {});
        // m_mtx.unlock();
    }
};

#endif // CNSOFTBEI_CLIENT_H
