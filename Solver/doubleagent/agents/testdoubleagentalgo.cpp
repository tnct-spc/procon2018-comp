#include "testdoubleagentalgo.h"

TestDoubleAgentAlgo::TestDoubleAgentAlgo(std::shared_ptr<GameManager> manager_ptr, const GeneticAgent &agent) :
    AgentWrapper(manager_ptr),
    agent_data(agent)
{

}

const std::tuple<int,int,int> TestDoubleAgentAlgo::agentMove(int side){



    return std::make_tuple(0,0,0);
}
