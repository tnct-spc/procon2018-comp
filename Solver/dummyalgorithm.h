#ifndef DUMMYALGORITHM_H
#define DUMMYALGORITHM_H

#include "algorithmwrapper.h"

class DummyAlgorithm : public AlgorithmWrapper
{

    using AlgorithmWrapper::AlgorithmWrapper;

public:
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int);
};

#endif // DUMMYALGORITHM_H
