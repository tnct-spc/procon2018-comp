#include "agentwrapper.h"

AgentWrapper::AgentWrapper(int side, int agent, std::shared_ptr<GameManager> manager_ptr) :
    side(side),
    agent(agent),
    manager(manager_ptr)
{

}
