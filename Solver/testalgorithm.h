#ifndef TESTALGORITHM_H
#define TESTALGORITHM_H

#include "field.h"
#include "algorithmwrapper.h"
#include "gamemanager.h"
#include <memory>
#include <vector>
#include <utility>

class TestAlgorithm : public AlgorithmWrapper
{
public:
    AlgorithmWrapper::AlgorithmWrapper;
    std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
};

#endif // TESTALGORITHM_H
