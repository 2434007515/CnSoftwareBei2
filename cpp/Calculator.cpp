//
// Created by 光球层上的黑子 on 2022/6/24.
//

#include "../head/Calculator.h"
#include <iomanip>
#include <mutex>
#include <shared_mutex>

void Calculator::loadDatas()
{
    this->reader.loadOrders();
    this->reader.loadParts();
}

void Calculator::solveOrder(ZipOrder &order)
{
    //当前订单中的零件信息不在当前进程的零件表中，则将信息传递给下一个进程
    // if (!reader.partIdHash.count(order.lPartkey))
    // {
    //     // std::cout << order.lPartkey << "\n";
    //     // std::cout << "continue send:" << Utils::zipOrderToString(order) << "\n";
    //     //零件号 零件数量 零件价格
    //     // string ss = to_string(order.lPartkey) + " " + to_string(order.lQuantity) + " " + to_string(order.lExtendedprice);
    //     string ss = Utils::zipOrderToString(order);
    //     message msg(message::headEnum::part, ss.c_str());
    //     sendMessager->send(msg);
    //     return;
    // }

    //找到这个订单的零件对应的品牌
    int idHash = reader.partIdHash[order.lPartkey];
    std::pair<int, int> brand = reader.parts[idHash].pBrandCode;
    // cout << "add lPartkey " << Utils::zipOrderToString(order) << " Brand#"
    //      << brand.first << brand.second << "\n";

    //找到这个零件品牌对应的统计数据
    auto &targetBrandData = totalData.datas[brand.first - 1][brand.second - 1];

    // std::unique_lock grd(targetBrandData->m_mtx); //上锁
    lock_guard lg(targetBrandData->m_mtx);
    // targetBrandData->m_mtx.lock();

    //如果对应品牌数据中没有这个零件，就先插入
    if (!targetBrandData->partIdHash.count(order.lPartkey))
    {
        targetBrandData->partIdHash[order.lPartkey] = targetBrandData->parts.size();
        targetBrandData->parts.emplace_back(PartData(order.lPartkey, {brand.first, brand.second}));
    }

    //找到这个零件的统计数据
    auto &targetPartData = targetBrandData->parts[targetBrandData->partIdHash[order.lPartkey]];
    targetPartData.saleCount += order.lQuantity;
    targetPartData.totalSales += order.lQuantity * order.lExtendedprice;
    totalData.totalSales += order.lQuantity;

    // targetBrandData->m_mtx.unlock();
}

void Calculator::doCalculate(condition_variable &cv, unique_lock<mutex> &lck)
{
    // send part
    cout << "start send part!" << endl;
    int sz = reader.parts.size();
    cout << "total num:" << sz << endl;
    for (int i = 0; i < sz; i++)
    {
        reader.sendPart(reader.parts[i]);
        if (i % 10000 == 0)
        {
            cout << i << endl;
            sleep(0.1);
        }
    }
    is_finished = true;
    sendFinish();
    cv.wait(lck);
    cout << "send part finish,start data sum!" << endl;

    // data sum
    auto begin = clock();
    int cnt = 0;
    for (auto &order : this->reader.orders)
    {
        cnt++;
        solveOrder(order);
        // if (cnt % 10000 == 0)
        // {
        //     cout << cnt << endl;
        // }
    }

    int totalPartsCount = 0;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            totalPartsCount += totalData.datas[i][j]->parts.size();
    double avg = (totalData.totalSales + 0.) / totalPartsCount;
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "totalSales:" << totalData.totalSales << std::endl;
    std::cout << "totalPartCount:" << totalPartsCount << std::endl;
    std::cout << "avg:" << avg << std::endl;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            std::cout << "Brand" << (i + 1) << (j + 1) << ": \n";
            for (auto k : totalData.datas[i][j]->parts)
            {
                if (k.saleCount < avg * 0.3)
                    std::cout << k.id << ' ' << k.saleCount << ' ' << k.totalSales << std::endl;
            }
        }
    }
    auto end = clock();
    cout << "time cost:" << (double)(end - begin) / 1000000 << "s.\n";
}
