#include "agentwrapper.h"

AgentWrapper::AgentWrapper(int side, int agent_num, std::shared_ptr<GameManager> manager_ptr, const GeneticAgent& agent_data = GeneticAgent(6)) :
    side(side),
    agent(agent_num),
    manager(manager_ptr),
    agent_data(agent_data)
{

}

void AgentWrapper::setAgent(const GeneticAgent &agent){
    agent_data = agent;
}
