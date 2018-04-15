#ifndef SEARCHNODE_H
#define SEARCHNODE_H

#include <vector>
#include "field.h"

class MonteCarloTreeSearch;

class SearchNode
{
public:

    SearchNode(MonteCarloTreeSearch *tree, SearchNode *parent, std::vector<std::pair<int,int>> agents, int side, int x_inp, int y_inp);

    bool canMove(int agent_1_move, int agent_2_move);

    //UCB定数 sqrt(2)が基本
    static constexpr double ucb_value = 1.41421356;
    static int size_x;
    static int size_y;
    const std::vector<int> x_list = { 0, 0, 1, 1, 1, -1, -1, -1, 0};
    const std::vector<int> y_list = { 1, -1, 1, 0, -1, 1, 0, -1, 0};


    unsigned int win_count = 0;
    unsigned int try_count = 0;
    int side;


    MonteCarloTreeSearch *tree;
    SearchNode *parent;

    std::vector<std::pair<int,int>> agents;
    std::vector<SearchNode*> childs;

    const procon::Field& field;

};

#endif // SEARCHNODE_H
