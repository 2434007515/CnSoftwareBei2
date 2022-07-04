//
// Created by 光球层上的黑子 on 2022/6/15.
//

#ifndef CNSOFTBEI_UTILS_H
#define CNSOFTBEI_UTILS_H

#include <vector>
#include <string>
#include "Part.h"
#include "ZipPart.h"
#include "ZipOrder.h"

class Utils
{
public:
    //慢，算了吧
    std::vector<std::string> static split(const std::string &str, char ch = ' ');

    ZipPart static loadPart(const std::string &line);
    ZipPart static loadPart(const char *line);

    ZipOrder static loadOrder(const std::string &line);
    ZipOrder static loadOrder(const char *line);

    //序列化
    std::string static zipOrderToString(ZipOrder zipOrder);
    //序列化
    std::string static zipPartToString(ZipPart zipPart);

    //反序列化
    ZipOrder static unZipOrderString(const char *line);
    //反序列化
    ZipOrder static unZipOrderString(const std::string &line);

    //反序列化
    ZipPart static unZipPartString(const std::string &line);
    //反序列化
    ZipPart static unZipPartString(const char *line);
};

#endif // CNSOFTBEI_UTILS_H