#include "searchnode.h"

#include "searchtree.h"

SearchNode::SearchNode(SearchTree *tree, SearchNode *parent, int move) :
    tree(tree),
    parent(parent),
    move(move)
{

}
