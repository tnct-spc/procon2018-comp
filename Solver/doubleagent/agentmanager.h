#ifndef AGENTMANAGERWRAPPER_H
#define AGENTMANAGERWRAPPER_H

#include <vector>

#include "algorithmwrapper.h"
#include "doubleagent/agents/agentwrapper.h"

class AgentManager : public AlgorithmWrapper
{

    //using AlgorithmWrapper::AlgorithmWrapper;

public:
    AgentManager(std::shared_ptr<GameManager> manager_ptr, int side = 0, int algorithm_number = 0);

    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);

    std::vector<std::shared_ptr<AgentWrapper>> agents;//各エージェント
};

#endif // AGENTMANAGERWRAPPER_H

