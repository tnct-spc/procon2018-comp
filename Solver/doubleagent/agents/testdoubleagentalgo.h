#ifndef TESTDOUBLEAGENTALGO_H
#define TESTDOUBLEAGENTALGO_H

#include "agentwrapper.h"

class TestDoubleAgentAlgo : public AgentWrapper
{
    using AgentWrapper::AgentWrapper;
public:
    const std::tuple<int,int,int> agentMove(int side);
};

#endif // TESTDOUBLEAGENTALGO_H
