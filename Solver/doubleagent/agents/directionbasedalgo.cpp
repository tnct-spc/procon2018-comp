#include "directionbasedalgo.h"

DirectionBasedAlgo::DirectionBasedAlgo(int side, const procon::Field& field, int final_turn, int agent_num, const GeneticAgent& agent_data) :
    AgentWrapper(side, field, final_turn, agent_num, 1, agent_data)
{

}

double DirectionBasedAlgo::evaluateMove(int move, bool is_delete, int now_turn, int eval_side){

    if(eval_side == -1)
        eval_side = side;
}
