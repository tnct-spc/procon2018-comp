#include "searchnode.h"


SearchNode::SearchNode(SearchNode *parent, int side, int try_count) :
    parent(parent),
    side(side),
    try_count(try_count)
{
    can_move_node_list.resize(81);
}

bool SearchNode::trySimulate(GameSimulator *sim){

}

void SearchNode::expandNode(GameSimulator *sim){

    for(int count = 0; count < 81; ++count){

        int agent_1_move = count / 9;
        int agent_2_move = count % 9;

        if(sim->canPut(side, agent_1_move, agent_2_move)){

            can_move_index_list.push_back(count);
            can_move_node_list.at(count) = new SearchNode(parent,side) ;
        }

    }

}
