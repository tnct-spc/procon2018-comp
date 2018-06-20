#ifndef TESTDOUBLEAGENTALGO_H
#define TESTDOUBLEAGENTALGO_H

#include "agentwrapper.h"

class TestDoubleAgentAlgo : public AgentWrapper
{
    using AgentWrapper::AgentWrapper;
public:

    const std::pair<double,bool> evaluateMove(int move);

};

#endif // TESTDOUBLEAGENTALGO_H
