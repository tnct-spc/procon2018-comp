#ifndef AGENTMANAGERWRAPPER_H
#define AGENTMANAGERWRAPPER_H

#include <vector>

#include "algorithmwrapper.h"
#include "doubleagent/agents/agentwrapper.h"
#include "doubleagent/algorithms/simplemcforduble.h"
#include "doubleagent/algorithms/nashequilibrium.h"

class AgentManager : public AlgorithmWrapper
{

    //using AlgorithmWrapper::AlgorithmWrapper;

public:
    AgentManager(const procon::Field& field, int turn_max, bool side, int algorithm_number = 0, const GeneticAgent* agent_data_1 = nullptr, const GeneticAgent* agent_data_2 = nullptr);

    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int now_turn);
    void setParams(std::vector<std::pair<QString, double>> params);

    void setAgentData(const GeneticAgent& agent_data, int agent_number);

private:
    std::vector<std::shared_ptr<AgentWrapper>> agents;//各エージェント

    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> simpleCalc(int now_turn);

    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> simpleMC(int now_turn);

    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> uniformMC(int now_turn);

    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> simpleNashAlgo(int now_turn);

    int algo_number;
};

#endif // AGENTMANAGERWRAPPER_H

