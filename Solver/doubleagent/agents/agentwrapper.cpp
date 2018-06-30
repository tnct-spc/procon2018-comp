#include "agentwrapper.h"

AgentWrapper::AgentWrapper(int side, const procon::Field& field, int final_turn, int agent_num, const GeneticAgent& agent_data) :
    side(side),
    agent(agent_num),
    final_turn(final_turn),
    field(field),
    agent_data(agent_data)
{

}

void AgentWrapper::setAgent(const GeneticAgent &agent){
    agent_data = agent;
}
