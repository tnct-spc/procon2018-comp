#include "searchnode.h"

#include "montecarlotreesearch.h"

SearchNode::SearchNode(MonteCarloTreeSearch *tree, SearchNode *parent, std::vector<std::pair<int,int>> agents, int side) :
    tree(tree),
    parent(parent),
    agents(agents),
    field(tree->getManagerPtr()->getField())
{
}
