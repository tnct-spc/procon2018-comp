#ifndef DUMMYALGORITHM_H
#define DUMMYALGORITHM_H

#include "algorithmwrapper.h"
#include <QString>

class DummyAlgorithm : public AlgorithmWrapper
{

    using AlgorithmWrapper::AlgorithmWrapper;

public:
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int);
    void setParams(std::vector<std::pair<QString, double>> params);
    void setRandomParams(std::vector<std::pair<QString, double>> params,bool flag);
};

#endif // DUMMYALGORITHM_H
