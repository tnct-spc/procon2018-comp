#ifndef EVALUATEPARAM_H
#define EVALUATEPARAM_H

#include "agentwrapper.h"

class EvaluateParam : public AgentWrapper
{
    using AgentWrapper::AgentWrapper;

public:
    const std::vector<std::pair<double, std::tuple<int,int,int>>> agentMove();
    double evaluateMove(int move, bool is_delete);

private:
    // パラメータの数
    const int param_count = 1;
};

#endif // EVALUATEPARAM_H
