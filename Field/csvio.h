#ifndef CSVIO_H
#define CSVIO_H

#include "field.h"
class csvIO
{

public:
    csvIO();
    procon::Field importField(std::string path);
    void exportField(procon::Field data, std::string path);

private:
    const int FUJISAN = 0;
    const int TAKAOSAN = 1;
    const int MITAKESAN = 2;
    const int HAKUTOUSAN = 3;
};

#endif // CSVIO_H
