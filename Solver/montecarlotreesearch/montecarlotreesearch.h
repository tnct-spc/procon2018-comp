#ifndef MONTECARLOTREESEARCH_H
#define MONTECARLOTREESEARCH_H

#include <algorithmwrapper.h>
#include "gamesimulator.h"
#include "searchnode.h"
#include "gamemanager.h"

class MonteCarloTreeSearch : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);

    //合計での試行回数
    int trial = 50;
};

#endif // MONTECARLOTREESEARCH_H
