#ifndef SEARCHNODE_H
#define SEARCHNODE_H

#include <vector>

class SearchTree;

class SearchNode
{
public:
    SearchNode(SearchTree *tree, SearchNode *parent, std::vector<std::pair<int,int>> agents);

    //UCB定数 sqrt(2)が基本
    static constexpr double ucb_value = 1.41421356;

    unsigned int win_count = 0;
    unsigned int try_count = 0;

    std::vector<SearchNode*> childs;

    std::vector<std::pair<int,int>> agents;

    SearchTree *tree;
    SearchNode *parent;
};

#endif // SEARCHNODE_H
