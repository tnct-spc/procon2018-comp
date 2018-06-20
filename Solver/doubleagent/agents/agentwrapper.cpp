#include "agentwrapper.h"

AgentWrapper::AgentWrapper(int side, int agent_num, std::shared_ptr<GameManager> manager_ptr, int param_count, const GeneticAgent& agent_data) :
    side(side),
    agent(agent_num),
    manager(manager_ptr),
    agent_data(agent_data)
{
    if(this->agent_data.size != param_count)
        this->agent_data = GeneticAgent(param_count, 2);

}

void AgentWrapper::setAgent(const GeneticAgent &agent){
    agent_data = agent;
}
