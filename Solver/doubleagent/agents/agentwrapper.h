#ifndef AGENTWRAPPER_H
#define AGENTWRAPPER_H

#include "gamemanager.h"

class AgentWrapper
{
public:
    AgentWrapper(std::shared_ptr<GameManager> manager_ptr);
    virtual const std::tuple<int,int,int> agentMove(int side) = 0;

protected:
    std::shared_ptr<GameManager> manager;
};

#endif // AGENTWRAPPER_H
