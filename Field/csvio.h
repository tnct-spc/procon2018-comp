#ifndef CSVIO_H
#define CSVIO_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include "field.h"
namespace procon{
    namespace CsvIo{

        procon::Field importField(std::string path);
        void exportField(procon::Field& data, std::string path);

        const int FUJISAN = 0;
        const int TAKAOSAN = 1;
        const int MITAKESAN = 2;
        const int HAKUTOUSAN = 3;
    }
}

#endif // CSVIO_H
