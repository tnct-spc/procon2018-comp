#ifndef DIRECTIONBASEDALGO_H
#define DIRECTIONBASEDALGO_H

#include "agentwrapper.h"

class DirectionBasedAlgo : public AgentWrapper
{
public:
    DirectionBasedAlgo(int side, const procon::Field& field, int final_trn, int agent_num, const GeneticAgent& agent_data = GeneticAgent(1, 2));

    double evaluateMove(int move, bool is_delete, int now_turn, int eval_side = -1);

private:

    std::vector<std::vector<int>> getAbstractBasedAgent(bool eval_side, bool eval_agent);
    const std::vector<std::pair<double, std::tuple<int,int,int>>> agentMove();

    std::vector<std::vector<int>> move_map;


    const std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    const std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};
};

#endif // DIRECTIONBASEDALGO_H
