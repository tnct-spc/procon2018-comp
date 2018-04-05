#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"
#include <memory>

class AlgorithmWrapper
{

public:
    AlgorithmWrapper(std::shared_ptr<procon::Field> field_ptr);

private:
    std::shared_ptr<procon::Field> field;
};

#endif // ALGORITHMWRAPPER_H
