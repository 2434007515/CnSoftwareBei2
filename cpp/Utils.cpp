//
// Created by 光球层上的黑子 on 2022/6/15.
//

#include <sstream>
#include <iostream>
#include "../head/Utils.h"

std::vector<std::string> Utils::split(const std::string &str, const char ch)
{
    std::vector<std::string> rec;
    std::string tmp;
    for (auto &i : str)
    {
        if (i == ch)
        {
            if (tmp.length() >= 1)
            {
                rec.emplace_back(tmp);
                tmp.clear();
            }
        }
        else
        {
            tmp += i;
        }
    }
    return rec;
}

ZipPart Utils::loadPart(const std::string &line)
{
    return Utils::loadPart(line.c_str());
}

ZipOrder Utils::loadOrder(const std::string &line)
{
    return Utils::loadOrder(line.c_str());
    //    ZipOrder zipOrder{0,0,0};
    //    int cnt=0;
    //    double base=1;
    //    for(char i : line){
    //        if(i=='|'){
    //            if(cnt==5)  break;
    //            cnt++;
    //        }
    //        else{
    //            if(cnt==1)
    //                zipOrder.lPartkey=zipOrder.lPartkey*10+i-'0';
    //            else if(cnt==4)
    //                zipOrder.lQuantity=zipOrder.lQuantity*10+i-'0';
    //            else if(cnt==5){
    //                if(i=='.'){
    //                    base*=0.1;
    //                    continue;
    //                }
    //                if(base>=1) zipOrder.lExtendedprice=zipOrder.lExtendedprice*10+i-'0';
    //                else    zipOrder.lExtendedprice+=base*(i-'0');
    //                if(base<1)  base*=0.1;
    //            }
    //        }
    //    }
    //    return zipOrder;
    //    1|1551894|76910|1|17|33078.94|0.04|0.02|N|O|1996-03-13|1996-02-12|1996-03-22|DELIVER IN PERSON|TRUCK|egular courts above the|//    auto v = Utils::split(s,'|');
    //    int partId = atoi(v[1].c_str());
    //    int saleCount = atoi(v[4].c_str());
    //    double sales = atof(v[5].c_str());
}

ZipOrder Utils::loadOrder(const char *line)
{
    ZipOrder zipOrder{0, 0, 0};
    int cnt = 0;
    double base = 1;
    for (int j = 0;; j++)
    {
        char i = line[j];
        if (i == '\0')
            throw std::runtime_error{std::string("error in ZipOrder Utils::loadOrder(const char *line). error: string end before unZip")};
        if (i == '|')
        {
            if (cnt == 5)
                break;
            cnt++;
        }
        else
        {
            if (cnt == 1)
                zipOrder.lPartkey = zipOrder.lPartkey * 10 + i - '0';
            else if (cnt == 4)
                zipOrder.lQuantity = zipOrder.lQuantity * 10 + i - '0';
            else if (cnt == 5)
            {
                if (i == '.')
                {
                    base *= 0.1;
                    continue;
                }
                if (base >= 1)
                    zipOrder.lExtendedprice = zipOrder.lExtendedprice * 10 + i - '0';
                else
                    zipOrder.lExtendedprice += base * (i - '0');
                if (base < 1)
                    base *= 0.1;
            }
        }
    }
    return zipOrder;
}

ZipPart Utils::loadPart(const char *line)
{
    ZipPart zipPart = {0, {0, 0}};
    int cnt = 0;
    for (int j = 0;; j++)
    {
        char i = line[j];
        if (i == '\0')
            throw std::runtime_error{std::string("error in ZipPart Utils::loadPart(const char *line). error: string end before unZip")};
        if (i == '|')
            cnt++;
        else
        {
            if (cnt == 0)
                zipPart.pPartkey = zipPart.pPartkey * 10 + i - '0';
            else if (cnt == 3)
            {
                if (i >= '0' && i <= '9')
                {
                    if (!zipPart.pBrandCode.first)
                        zipPart.pBrandCode.first = i - '0';
                    else
                    {
                        zipPart.pBrandCode.second = i - '0';
                        break;
                    }
                }
            }
        }
    }
    return zipPart;
}

std::string Utils::zipOrderToString(ZipOrder zipOrder)
{
    return std::to_string(zipOrder.lPartkey) + '|' + std::to_string(zipOrder.lQuantity) + '|' + std::to_string(zipOrder.lExtendedprice) + '|';
}

std::string Utils::zipPartToString(ZipPart zipPart)
{
    return std::to_string(zipPart.pPartkey) + '|' + std::to_string(zipPart.pBrandCode.first) + '|' + std::to_string(zipPart.pBrandCode.second) + '|';
}

ZipOrder Utils::unZipOrderString(const char *line)
{
    ZipOrder zipOrder{0, 0, 0};
    int cnt = 0;
    double base = 1;
    bool dig = false;
    for (int i = 0; cnt < 3; i++)
    {
        if (line[i] == '|')
            cnt++;
        else if (line[i] == '.')
        {
            dig = true;
        }
        else
        {
            if (cnt == 0)
            {
                zipOrder.lPartkey = zipOrder.lPartkey * 10 + line[i] - '0';
            }
            else if (cnt == 1)
            {
                zipOrder.lQuantity = zipOrder.lQuantity * 10 + line[i] - '0';
            }
            else
            {
                if (dig)
                {
                    base /= 10;
                    zipOrder.lExtendedprice += base * (line[i] - '0');
                }
                else
                {
                    zipOrder.lExtendedprice = zipOrder.lExtendedprice * 10 + line[i] - '0';
                }
            }
        }
    }
    return zipOrder;
}

ZipOrder Utils::unZipOrderString(const std::string &line)
{
    return Utils::unZipOrderString(line.c_str());
}

ZipPart Utils::unZipPartString(const std::string &line)
{
    return Utils::unZipPartString(line.c_str());
}

ZipPart Utils::unZipPartString(const char *line)
{
    ZipPart zipPart = {0, {0, 0}};
    int cnt = 0;
    for (int j = 0; cnt < 3; j++)
    {
        char i = line[j];
        if (i == '|')
            cnt++;
        else
        {
            if (cnt == 0)
                zipPart.pPartkey = zipPart.pPartkey * 10 + i - '0';
            else if (cnt == 1)
            {
                zipPart.pBrandCode.first = i - '0';
            }
            else if (cnt == 2)
            {
                zipPart.pBrandCode.second = i - '0';
            }
        }
    }
    return zipPart;
}

std::string Utils::zipTotalData(int from, int partCount, long long totalSale)
{
    return std::to_string(from) + '|' + std::to_string(partCount) + '|' + std::to_string(totalSale) + '|';
}

std::pair<int, std::pair<int, int>> Utils::unZipTotalData(const std::string &data)
{
    return Utils::unZipTotalData(data.c_str());
}
std::pair<int, std::pair<int, int>> Utils::unZipTotalData(const char *line)
{
    std::pair<int, std::pair<int, int>> rec{0, {0, 0}};
    int cnt = 0;
    for (int i = 0; cnt < 3; i++)
    {
        if (line[i] == '|')
            cnt++;
        else
        {
            if (cnt == 0)
            {
                rec.first = rec.first * 10 + line[i] - '0';
            }
            else if (cnt == 1)
            {
                rec.second.first = rec.second.first * 10 + line[i] - '0';
            }
            else
            {
                rec.second.second = rec.second.second * 10 + line[i] - '0';
            }
        }
    }
    return rec;
}

std::string Utils::zipPartData(PartData part)
{
    return std::to_string(part.id) + '|' + std::to_string(part.brandCode.first) + std::to_string(part.brandCode.second) + std::to_string(part.saleCount) + '|' + std::to_string(part.totalSales);
}

PartData Utils::unzipPartData(const char *line)
{
    int cnt = 0;
    double base = 1;
    bool dig = false;
    PartData part{};
    for (int i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == '|')
            cnt++;
        else if (line[i] == '.')
            dig = true;
        else
        {
            if (cnt == 0)
            {
                part.id = part.id * 10 + line[i] - '0';
            }
            else if (cnt == 1)
            {
                if (!part.brandCode.first)
                {
                    part.brandCode.first = line[i] - '0';
                }
                else if (!part.brandCode.second)
                {
                    part.brandCode.second = line[i] - '0';
                }
                else
                {
                    part.saleCount = part.saleCount * 10 + line[i] - '0';
                }
            }
            else
            {
                if (dig)
                {
                    base /= 10;
                    part.totalSales += (line[i] - '0') * base;
                }
                else
                {
                    part.totalSales = part.totalSales * 10 + line[i] - '0';
                }
            }
        }
    }
    return part;
}

PartData Utils::unzipPartData(const std::string &line)
{
    return Utils::unzipPartData(line.c_str());
}
