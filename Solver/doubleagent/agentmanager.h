#ifndef AGENTMANAGERWRAPPER_H
#define AGENTMANAGERWRAPPER_H

#include <vector>

#include "algorithmwrapper.h"
#include "doubleagent/agents/agentwrapper.h"

class AgentManager : public AlgorithmWrapper
{

    //using AlgorithmWrapper::AlgorithmWrapper;

public:
    AgentManager(std::shared_ptr<GameManager> manager_ptr, int side = 0, int algorithm_number = 0, GeneticAgent* agent_data_1 = nullptr, GeneticAgent* agent_data_2 = nullptr);

    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);

    void setAgentData(const GeneticAgent& agent_data, int agent_number);

private:
    std::vector<std::shared_ptr<AgentWrapper>> agents;//各エージェント
};

#endif // AGENTMANAGERWRAPPER_H

