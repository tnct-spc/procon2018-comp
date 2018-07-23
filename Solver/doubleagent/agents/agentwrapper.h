#ifndef AGENTWRAPPER_H
#define AGENTWRAPPER_H

#include "gamemanager.h"

class AgentWrapper
{
public:
    AgentWrapper(int side, procon::Field& field, int final_turn, int agent_num, int param_count, const GeneticAgent& agent_data = GeneticAgent(6, 2));

    //(評価値,移動方法)のvectorを渡してもらう
    const std::vector<std::pair<double, std::tuple<int,int,int>>> agentMove(int now_turn);
    virtual double evaluateMove(int move, bool is_delete, int now_turn, int eval_side = -1) = 0;

    void setAgent(const GeneticAgent& agent);

    bool side;

protected:

    int agent, final_turn;
    procon::Field& field;

    GeneticAgent agent_data;
};

#endif // AGENTWRAPPER_H
