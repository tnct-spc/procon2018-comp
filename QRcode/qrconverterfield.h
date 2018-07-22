#ifndef QRCONVERTERFIELD_H
#define QRCONVERTERFIELD_H

#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include "field.h"

class QrConverterField
{
public:
    QrConverterField();
    procon::Field ConvertCsvToField(std::string csv_string);
    int ctoi(std::string s, int& i);
};



#endif // QRCONVERTERFIELD_H
