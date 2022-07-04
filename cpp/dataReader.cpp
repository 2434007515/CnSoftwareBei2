//
// Created by 光球层上的黑子 on 2022/6/15.
//

#include "../head/dataReader.h"
#include "../head/Utils.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <unistd.h>
using namespace std;
void dataReader::getReaderParms()
{
    return;
}

void dataReader::storePart(ZipPart &part)
{
    if (partIdHash.count(part.pPartkey))
        return;
    sendPart(part);
    // std::cout << "store part " << Utils::zipPartToString(part) << "\n";
    // m_mtx.lock();
    partIdHash[part.pPartkey] = parts.size();
    parts.emplace_back(part);
    // m_mtx.unlock();
}

int dataReader::loadParts()
{
    cout << "\nstart load parts! index=" << index << " n=" << n << endl;
    std::ifstream partFile;
    partFile.open(partFileName, std::ios::in);

    int cnt = 0;
    std::string s;

    auto st = clock();
    while (getline(partFile, s))
    {
        cnt++;
        if (cnt % n + 1 != index)
            continue;
        auto part = Utils::loadPart(s);
        partIdHash[part.pPartkey] = parts.size();
        parts.emplace_back(part);
        // cout << "store pPartkey " << part.pPartkey << "\n";
    }
    auto ed = clock();
    cout << "-------------------------\n";
    cout << "load parts data over.\n";
    cout << "time cost:" << (double)(ed - st) / 1000000 << "s.\n";
    cout << "load " << parts.size() << " lines\n\n";
    return 0;
}

int dataReader::loadOrders()
{
    cout << "\nstart load orders! index=" << index << " n=" << n << endl;
    std::ifstream partFile;
    partFile.open(orderFileName, std::ios::in);

    int cnt = 0;
    std::string s;
    auto st = clock();
    while (getline(partFile, s))
    {
        cnt++;
        if (cnt % n + 1 != index)
            continue;
        auto zipOrder = Utils::loadOrder(s);
        orders.emplace_back(zipOrder);
        // cout << "store order " << Utils::zipOrderToString(zipOrder) << "\n";
    }
    auto ed = clock();
    cout << "-------------------------\n";
    cout << "load orders data over.\n";
    cout << "time cost:" << (double)(ed - st) / 1000000 << "s.\n";
    cout << "load " << orders.size() << " lines\n\n";
    return 0;
}
