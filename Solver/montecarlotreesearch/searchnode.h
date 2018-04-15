#ifndef SEARCHNODE_H
#define SEARCHNODE_H

#include <vector>
#include "field.h"

class MonteCarloTreeSearch;

class SearchNode
{
public:

    SearchNode(MonteCarloTreeSearch *tree, SearchNode *parent, std::vector<std::pair<int,int>> agents, int side);


    //UCB定数 sqrt(2)が基本
    static constexpr double ucb_value = 1.41421356;

    //閾値 これ以上試行した場合は葉を展開する
    static constexpr int threshold = 5;

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

    /*

      閾値を設定し、それ以上の場合は葉を展開する
      葉が展開できなくなった場合はそこからプレイアウト

    */

};

#endif // SEARCHNODE_H
