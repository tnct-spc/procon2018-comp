#ifndef AGENTWRAPPER_H
#define AGENTWRAPPER_H

#include "gamemanager.h"

class AgentWrapper
{
public:
    AgentWrapper(int side, int agent_num, std::shared_ptr<GameManager> manager_ptr, int param_count, const GeneticAgent& agent_data = GeneticAgent(6, 2));

    //(評価値,移動方法)のvectorを渡してもらう
    const std::vector<std::pair<double, std::tuple<int,int,int>>> agentMove();
    virtual const std::pair<double, bool> evaluateMove(int move) = 0;

    void setAgent(const GeneticAgent& agent);

protected:
    int side,agent;
    std::shared_ptr<GameManager> manager;

    GeneticAgent agent_data;
};

#endif // AGENTWRAPPER_H
