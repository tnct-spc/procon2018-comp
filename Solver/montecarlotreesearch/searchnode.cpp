#include "searchnode.h"

#include "montecarlotreesearch.h"

SearchNode::SearchNode(MonteCarloTreeSearch *tree, SearchNode *parent, std::vector<std::pair<int,int>> agents, int x_inp , int y_inp) :
    agents(agents),
    tree(tree),
    parent(parent)
{
    if(x_inp != -1){
        size_x = x_inp;
        size_y = y_inp;
    }

}

bool SearchNode::canMove(int agent_1_move, int agent_2_move){

    return true;
}
