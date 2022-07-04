//
// Created by 光球层上的黑子 on 2022/6/24.
//

#ifndef CNSOFTBEI_CALCULATOR_H
#define CNSOFTBEI_CALCULATOR_H

#include "TotalData.h"
#include "dataReader.h"
#include "client.h"
#include "Utils.h"
#include "Global.h"
#include <mutex>

class Calculator
{
public:
    TotalData &totalData;
    dataReader &reader;

    void loadDatas();
    void doCalculate(condition_variable &cv, std::unique_lock<std::mutex> &lck);
    void solveOrder(ZipOrder &order);
    Calculator(TotalData &totalData, dataReader &reader) : totalData(totalData), reader(reader)
    {
        index = reader.index;
        n = reader.n;
        is_finished = false;
        is_notify = false;
    };

    // 用于finish的同步
    int index, n;
    bool is_finished;
    bool is_notify;
    void sendFinish(const char *cnt = " 0000")
    {
        // cout << "111 " << cnt << "\n";
        char temp[6];
        strcpy(temp, cnt);
        // cout << "222 index:" << index << " n:" << n << endl;
        // cout << "1 temp[index]=" << temp[index] << endl;
        temp[index]++;
        // cout << "2 temp[index]=" << temp[index] << endl;
        reader.sendMessager->send(message(message::headEnum::finish, temp));
    }
    bool checkFinish(const char *cnt)
    {
        for (int i = 1; i <= n; i++)
            if (cnt[i] < '2')
                return 0;
        return 1;
    }
};

#endif // CNSOFTBEI_CALCULATOR_H
