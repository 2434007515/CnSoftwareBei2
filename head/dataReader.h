//
// Created by 光球层上的黑子 on 2022/6/15.
//

#ifndef CNSOFTBEI_DATAREADER_H
#define CNSOFTBEI_DATAREADER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../head/ZipPart.h"
#include "ZipOrder.h"
#include "message.h"
#include <mutex>
#include "Utils.h"
#include "client.h"

class dataReader
{
public:
    std::string partFileName = R"(../data/part.tbl)";
    std::string orderFileName = R"(../data/lineitem.tbl)";
    std::vector<ZipPart> parts;
    std::unordered_map<int, int> partIdHash;
    std::vector<ZipOrder> orders;
    client *sendMessager;
    int index, n;
    // std::mutex m_mtx;

    dataReader(int index, int n, bool isTesting = false, client *sendMessager = NULL) : n(n), index(index)
    {
        if (isTesting)
        {
            partFileName = R"(../data/testGoods)";
            orderFileName = R"(../data/testSales.txt)";
        }
    }
    void getReaderParms();
    int loadParts();  //获取Part表数据
    int loadOrders(); //获取lineitem表数据

    void sendPart(ZipPart &zippart)
    {
        // std::cout << "pPartkey:" << zippart.pPartkey << " pBrandCode:" << zippart.pBrandCode.first << zippart.pBrandCode.second << "\n";
        // std::cout << "send:" << Utils::zipPartToString(zippart) << "\n";
        sendMessager->send(message(message::headEnum::part, Utils::zipPartToString(zippart).c_str()));
    }
    void storePart(ZipPart &zippart);
};

#endif // CNSOFTBEI_DATAREADER_H
