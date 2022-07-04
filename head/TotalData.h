//
// Created by 光球层上的黑子 on 2022/6/23.
//

#ifndef CNSOFTBEI_TOTALDATA_H
#define CNSOFTBEI_TOTALDATA_H

#include <iostream>
#include "BrandData.h"

class TotalData
{

public:
    // 5*5数组，datas[i][j]存放brand i+1 j+1数据
    std::vector<std::vector<BrandData *>> datas;
    //总销售数量
    int totalSales;
    TotalData() : totalSales(0)
    {
        // std::cout << "111" << std::endl;
        datas = std::vector<std::vector<BrandData *>>{5, std::vector<BrandData *>(5)};
        // std::cout << "222" << std::endl;
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                datas[i][j] = new BrandData();
                datas[i][j]->brandCode = {i, j};
            }
        }
        // std::cout << "333" << std::endl;
    }
};

#endif // CNSOFTBEI_TOTALDATA_H
