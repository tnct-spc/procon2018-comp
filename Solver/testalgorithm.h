#ifndef TESTALGORITHM_H
#define TESTALGORITHM_H

#include "field.h"
#include "algorithmwrapper.h"
#include "gamemanager.h"
#include <memory>
#include <vector>
#include <utility>
#include <random>
#include <algorithm>

class TestAlgorithm : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
};

#endif // TESTALGORITHM_H
