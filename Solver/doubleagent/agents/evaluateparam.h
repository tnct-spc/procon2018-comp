#ifndef EVALUATEPARAM_H
#define EVALUATEPARAM_H

#include "agentwrapper.h"

class EvaluateParam : public AgentWrapper
{
    using AgentWrapper::AgentWrapper;

public:
    EvaluateParam(int side, procon::Field& field, int final_turn, int agent_num, const GeneticAgent& agent_data = GeneticAgent(1, 2));

    const std::vector<std::pair<double, std::tuple<int,int,int>>> agentMove();
    double evaluateMove(int move, bool is_delete, int now_turn, int eval_side = -1);

    const bool use_region = false;

};

#endif // EVALUATEPARAM_H
