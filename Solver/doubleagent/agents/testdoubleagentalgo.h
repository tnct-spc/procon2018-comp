#ifndef TESTDOUBLEAGENTALGO_H
#define TESTDOUBLEAGENTALGO_H

#include "agentwrapper.h"

class TestDoubleAgentAlgo : public AgentWrapper
{
    //using AgentWrapper::AgentWrapper;
public:

    TestDoubleAgentAlgo(std::shared_ptr<GameManager> manager_ptr, const GeneticAgent& agent = GeneticAgent(7));

    const std::tuple<int,int,int> agentMove(int side);
    std::pair<double,bool> evaluateMove(int side,std::pair<int,int> move);

private:

    GeneticAgent agent_data;
};

#endif // TESTDOUBLEAGENTALGO_H
