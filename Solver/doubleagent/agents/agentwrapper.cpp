#include "agentwrapper.h"

AgentWrapper::AgentWrapper(int side, int agent_num, std::shared_ptr<GameManager> manager_ptr) :
    side(side),
    agent(agent_num),
    manager(manager_ptr)
{

}
