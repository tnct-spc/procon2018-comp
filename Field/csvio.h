#ifndef CSVIO_H
#define CSVIO_H

#include "field.h"
class CsvIo
{

public:
    CsvIo();
    static procon::Field importField(std::string path);
    static void exportField(procon::Field data, std::string path);

private:
    static const int FUJISAN = 0;
    static const int TAKAOSAN = 1;
    static const int MITAKESAN = 2;
    static const int HAKUTOUSAN = 3;
};

#endif // CSVIO_H
