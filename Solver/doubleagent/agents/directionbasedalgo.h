#ifndef DIRECTIONBASEDALGO_H
#define DIRECTIONBASEDALGO_H

#include "agentwrapper.h"

class DirectionBasedAlgo : public AgentWrapper
{
public:
    DirectionBasedAlgo(int side, const procon::Field& field, int final_trn, int agent_num, const GeneticAgent& agent_data = GeneticAgent(1, 2));

    double evaluateMove(int move, bool is_delete, int now_turn, int eval_side = -1);
};

#endif // DIRECTIONBASEDALGO_H
