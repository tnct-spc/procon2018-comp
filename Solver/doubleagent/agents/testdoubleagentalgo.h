#ifndef TESTDOUBLEAGENTALGO_H
#define TESTDOUBLEAGENTALGO_H

#include "agentwrapper.h"

class TestDoubleAgentAlgo : public AgentWrapper
{
    using AgentWrapper::AgentWrapper;
public:
    double evaluateMove(int move, bool is_delete);

};

#endif // TESTDOUBLEAGENTALGO_H
