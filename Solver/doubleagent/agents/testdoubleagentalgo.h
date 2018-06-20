#ifndef TESTDOUBLEAGENTALGO_H
#define TESTDOUBLEAGENTALGO_H

#include "agentwrapper.h"

class TestDoubleAgentAlgo : public AgentWrapper
{
    using AgentWrapper::AgentWrapper;
public:


    const std::vector<std::pair<double, std::tuple<int,int,int>>> agentMove();
    std::pair<double,bool> evaluateMove(int move);

};

#endif // TESTDOUBLEAGENTALGO_H
