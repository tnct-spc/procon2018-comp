#ifndef QRCONVERTERFIELD_H
#define QRCONVERTERFIELD_H

#include <vector>
#include <string>
#include <iostream>
#include <queue>
class QrConverterField
{
public:
    QrConverterField();
    std::string read_only_data="8 11:-2 1 0 1 2 0 2 1 0 1 -2:1 3 2 -2 0 1 0 -2 2 3 1:1 3 2 1 0 -2 0 1 2 3 1:2 1 1 2 2 3 2 2 1 1 2: 2 1 1 2 2 3 2 2 1 1 2: 1 3 2 1 0 -2 0 1 2 3 1:1 3 2 -2 0 1 0 -2 2 3 1: -2 1 0 1 2 0 2 1 0 1 -2:2 2:7 10:";
    std::vector<int> A;
    std::vector<int> B;
    std::vector<std::vector<int>> vec;
};



#endif // QRCONVERTERFIELD_H
