//
// Created by 光球层上的黑子 on 2022/6/15.
//

#ifndef CNSOFTBEI_BRANDDATA_H
#define CNSOFTBEI_BRANDDATA_H

#include <vector>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include "PartData.h"

class BrandData
{

public:
    //似乎冗余?
    std::pair<int, int> brandCode;
    std::vector<PartData> parts;
    std::unordered_map<int, int> partIdHash;
    mutable std::mutex m_mtx;
};

#endif // CNSOFTBEI_BRANDDATA_H
