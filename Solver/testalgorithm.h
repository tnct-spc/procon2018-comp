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

#include <iostream>

class TestAlgorithm : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
};
#endif // TESTALGORITHM_H
