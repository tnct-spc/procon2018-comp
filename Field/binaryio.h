#ifndef BINARYIO_H
#define BINARYIO_H
#include"field.h"


namespace procon {
    namespace BinaryIo {

    procon::Field importField(std::string path);
    void exportField(procon::Field& data, std::string path);
    std::string exportToString(procon::Field& data);

    }
}

#endif // BINARYIO_H
