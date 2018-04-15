#include "searchnode.h"

#include "searchtree.h"

SearchNode::SearchNode(SearchTree *tree, SearchNode *parent, std::vector<std::pair<int,int>> agents) :
    agents(agents),
    tree(tree),
    parent(parent)
{

}
