#ifndef MONTECARLOTREESEARCH_H
#define MONTECARLOTREESEARCH_H

#include <algorithmwrapper.h>
#include "searchnode.h"
#include "gamemanager.h"

class MonteCarloTreeSearch : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
};

#endif // MONTECARLOTREESEARCH_H
