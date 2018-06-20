#ifndef EVALUATEPARAM_H
#define EVALUATEPARAM_H

#include "agentwrapper.h"

class EvaluateParam : public AgentWrapper
{
    using AgentWrapper::AgentWrapper;

public:
    const std::vector<std::pair<double, std::tuple<int,int,int>>> agentMove();
    const std::pair<double,bool> evaluateMove(int move);
};

#endif // EVALUATEPARAM_H
