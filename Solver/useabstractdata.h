#ifndef USEABSTRACTDATA_H
#define USEABSTRACTDATA_H

#include "algorithmwrapper.h"

class UseAbstractData : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int now_turn);

private:
    std::vector<std::vector<double>> getAbstractBasedAgent(bool side, bool agent);
};

#endif // USEABSTRACTDATA_H
