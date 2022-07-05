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
#include "PartData.h"

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
    //序列化
    std::string static zipTotalData(int from, int partCount, long long totalSale);
    //序列化
    std::string static zipPartData(PartData part);

    //反序列化
    PartData static unzipPartData(const char *line);
    //反序列化
    PartData static unzipPartData(const std::string &line);

    //反序列化
    ZipOrder static unZipOrderString(const char *line);
    //反序列化
    ZipOrder static unZipOrderString(const std::string &line);

    //反序列化
    ZipPart static unZipPartString(const std::string &line);
    //反序列化
    ZipPart static unZipPartString(const char *line);

    //反序列化
    std::pair<int, std::pair<int, int>> static unZipTotalData(const std::string &data);
    //反序列化
    std::pair<int, std::pair<int, int>> static unZipTotalData(const char *line);

    static std::string getTag(int x)
    {
        std::string s = "[";
        for (int i = 1; i <= x; i++)
            s += "**";
        for (int i = x; i < 10; i++)
            s += "..";
        s += ']';
        s += std::to_string(x * 10) + "%";
        return s;
    }
};

#endif // CNSOFTBEI_UTILS_H