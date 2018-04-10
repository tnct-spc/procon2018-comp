#ifndef TESTALGORITHM_H
#define TESTALGORITHM_H

#include "field.h"
#include "algorithmwrapper.h"
#include <memory>

class TestAlgorithm : public AlgorithmWrapper
{
public:
    AlgorithmWrapper::AlgorithmWrapper;
    std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct();
};

#endif // TESTALGORITHM_H
