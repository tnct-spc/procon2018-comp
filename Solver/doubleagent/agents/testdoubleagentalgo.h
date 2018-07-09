#ifndef TESTDOUBLEAGENTALGO_H
#define TESTDOUBLEAGENTALGO_H

#include "agentwrapper.h"

class TestDoubleAgentAlgo : public AgentWrapper
{
    using AgentWrapper::AgentWrapper;
public:

    TestDoubleAgentAlgo(int side, const procon::Field& field, int final_turn, int agent_num, const GeneticAgent& agent_data = GeneticAgent(6, 2));

    double evaluateMove(int move, bool is_delete, int);

};

#endif // TESTDOUBLEAGENTALGO_H
