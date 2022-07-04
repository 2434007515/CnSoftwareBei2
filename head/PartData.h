//
// Created by 光球层上的黑子 on 2022/6/15.
//

#ifndef CNSOFTBEI_PARTDATA_H
#define CNSOFTBEI_PARTDATA_H


#include <utility>

class PartData {

public:
    PartData();
    int id;// 零件id
    std::pair<int,int> brandCode;//品牌名
    int saleCount; // 销售数量
    double totalSales; // 销售额
    PartData(int id,std::pair<int,int>brandCode):id(id),brandCode(brandCode){}

};


#endif //CNSOFTBEI_PARTDATA_H
