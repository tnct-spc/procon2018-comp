#ifndef SEARCHNODE_H
#define SEARCHNODE_H

#include <vector>

class SearchTree;

class SearchNode
{
public:
    SearchNode(SearchTree *tree, SearchNode *parent, int move);

private:
    unsigned int win_count = 0;
    unsigned int try_count = 0;

    std::vector<SearchNode*> childs;

    SearchTree *tree;
    SearchNode *parent;
    int move;
};

#endif // SEARCHNODE_H
