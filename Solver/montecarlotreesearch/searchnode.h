#ifndef SEARCHNODE_H
#define SEARCHNODE_H

#include <vector>

class MonteCarloTreeSearch;

class SearchNode
{
public:

    SearchNode(MonteCarloTreeSearch *tree, SearchNode *parent, std::vector<std::pair<int,int>> agents, int x_inp, int y_inp);

    bool canMove(int agent_1_move, int agent_2_move);

    //UCB定数 sqrt(2)が基本
    static constexpr double ucb_value = 1.41421356;
    static int size_x;
    static int size_y;

    unsigned int win_count = 0;
    unsigned int try_count = 0;

    MonteCarloTreeSearch *tree;
    SearchNode *parent;

    std::vector<std::pair<int,int>> agents;
    std::vector<SearchNode*> childs;

};

#endif // SEARCHNODE_H
