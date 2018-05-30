#ifndef TESTDOUBLEAGENTALGO_H
#define TESTDOUBLEAGENTALGO_H

#include "agentwrapper.h"

class TestDoubleAgentAlgo : public AgentWrapper
{
    // using AgentWrapper::AgentWrapper;
public:

    TestDoubleAgentAlgo(int side, int agent_num, std::shared_ptr<GameManager> manager_ptr, const GeneticAgent& agent_data = GeneticAgent(6, 2));

    const std::vector<std::pair<double, std::tuple<int,int,int>>> agentMove();
    std::pair<double,bool> evaluateMove(int move);

};

#endif // TESTDOUBLEAGENTALGO_H
