#ifndef SIMPLEMONTECARLO_H
#define SIMPLEMONTECARLO_H

#include "algorithmwrapper.h"


class SimpleMonteCalro : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);

};

#endif // SIMPLEMONTECARLO_H
