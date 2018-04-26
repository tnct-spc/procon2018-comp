#ifndef GENETICALGO_H
#define GENETICALGO_H

#include "algorithmwrapper.h"

class GeneticAlgo : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    GeneticAlgo();
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
};

#endif // GENETICALGO_H
