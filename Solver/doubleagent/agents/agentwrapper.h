#ifndef AGENTWRAPPER_H
#define AGENTWRAPPER_H

#include "gamemanager.h"

class AgentWrapper
{
public:
    AgentWrapper(int side, int agent, std::shared_ptr<GameManager> manager_ptr);
    virtual const std::tuple<int,int,int> agentMove(int side) = 0;

protected:
    int side,agent;
    std::shared_ptr<GameManager> manager;
};

#endif // AGENTWRAPPER_H
