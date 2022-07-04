#include <iostream>
#include <ctime>
#include "head/Part.h"
#include "head/Lineitem.h"
#include "head/dataReader.h"
#include "head/TotalData.h"
#include "head/Calculator.h"
#include "asio.hpp"
#include "head/server.h"
#include "head/client.h"
#include "head/Utils.h"
#include "head/Global.h"
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>
using namespace std;
const int serverPort[] = {0, 8001, 8002, 8003, 8004};
#ifdef _WIN64
#define SYS_PAUSE system("pause")
#else
#define SYS_PAUSE pause()
#endif

// #define IS_TEST

#ifdef IS_TEST
#define testflag 1
#else
#define testflag 0
#endif

void sendConnect(int index, int n, client &sender)
{
    string ss = "client " + to_string(index) + " connect to server " + to_string(index - 1 == 0 ? 4 : index - 1);
    sender.send(message(message::headEnum::connect, ss.c_str()));
    // cout << "send: " << ss << endl;
}
void sendLoad(client &sender)
{
    string ss = "load!";
    sender.send(message(message::headEnum::load, ss.c_str()));
}
void sendStart(client &sender)
{
    string ss = "start!";
    sender.send(message(message::headEnum::start, ss.c_str()));
}
int main(int argc, char *argv[])
{
    int n = 4;
    if (argc != 2 || (argv[1][0] != '1' && argv[1][0] != '2' && argv[1][0] != '3' && argv[1][0] != '4'))
    {
        // cout << argc << endl;
        // cout << argv[1] << endl;
        cout << "please input program number (1~4)." << endl;
        return -1;
    }
    int index = argv[1][0] - '0';
    cout << "the " << index << "st program launch." << endl;
    // cout << index << endl;
    // Global::index = index, Global::n = 4;
    // Global::init(index, n);
    if (index == 1)
    {
        // init
        mutex mtx;
        condition_variable cv;
        unique_lock lck(mtx);

        dataReader reader(index, n, testflag);
        TotalData totalData;
        Calculator calculator(totalData, reader);
        server loadMessager(serverPort[index], cv, calculator);
        cout << "wait for load!" << endl;

        // load
        string s;
        while (cin >> s)
        {
            if (s != "load")
                cout << "please enter \"load\"!" << endl;
            else
                break;
        }
        // cout << "serverPort[n]=" << serverPort[n] << endl;
        client sendMessager(serverPort[n]);
        reader.sendMessager = &sendMessager;
        sendConnect(index, n, sendMessager);
        sendLoad(sendMessager);
        cv.wait(lck);

        calculator.loadDatas();
        cout << "wait for start!" << endl;

        // start
        while (cin >> s)
        {
            if (s != "start")
                cout << "please enter \"start\"!" << endl;
            else
                break;
        }
        // cout << "started!" << endl;
        sendStart(sendMessager);
        cv.wait(lck);
        calculator.doCalculate(cv, lck);
        SYS_PAUSE;
    }
    else
    {
        // init
        mutex mtx;
        condition_variable cv;
        unique_lock lck(mtx);
        cout << "index=" << index << ",serverPort[index]=" << serverPort[index] << endl;
        dataReader reader(index, n, testflag);
        // cout << "111" << endl;
        TotalData totalData;
        // cout << "222" << endl;
        Calculator calculator(totalData, reader);
        server loadMessager(serverPort[index], cv, calculator);
        client sendMessager(serverPort[index - 1]);
        reader.sendMessager = &sendMessager;
        sendConnect(index, n, sendMessager);
        cout << "wait for load!" << endl;

        // load
        cv.wait(lck);
        sendLoad(sendMessager);

        calculator.loadDatas();
        cout << "wait for start!" << endl;

        // start
        cv.wait(lck);
        sendStart(sendMessager);
        calculator.doCalculate(cv, lck);

        SYS_PAUSE;
        return 0;
    }
    //    dataReader reader;
    //    TotalData totalData;
    //    Calculator calculator(totalData,reader);
    //    calculator.loadDatas();
    //    calculator.doCalculate();
}

void testZipUnzip()
{
    auto p = Utils::loadPart("1|goldenrod lavender spring chocolate lace|Manufacturer#1|Brand#13|PROMO BURNISHED COPPER|7|JUMBO PKG|901.00|ly. slyly ironi|");
    cout << p.pPartkey << ' ' << '<' << p.pBrandCode.first << ',' << p.pBrandCode.second << '>' << endl;
    auto s = Utils::zipPartToString(p);
    cout << s << endl;
    auto p2 = Utils::unZipPartString(s);
    cout << p2.pPartkey << ' ' << '<' << p2.pBrandCode.first << ',' << p2.pBrandCode.second << '>' << endl;

    auto order1 = Utils::loadOrder("1|1551894|76910|1|17|33078.94|0.04|0.02|N|O|1996-03-13|1996-02-12|1996-03-22|DELIVER IN PERSON|TRUCK|egular courts above the|");
    cout << order1.lPartkey << ' ' << order1.lQuantity << ' ' << order1.lExtendedprice << endl;
    string ss = Utils::zipOrderToString(order1);
    cout << ss << endl;
    auto order2 = Utils::unZipOrderString(ss);
    cout << order2.lPartkey << ' ' << order2.lQuantity << ' ' << order2.lExtendedprice << endl;
}
