#ifndef MONTECARLOTREESEARCH_H
#define MONTECARLOTREESEARCH_H

#include <algorithmwrapper.h>
#include <time.h>

#include "gamesimulator.h"
#include "searchnode.h"
#include "gamemanager.h"

class MonteCarloTreeSearch : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
    void setParams(std::vector<std::pair<QString, double>> params);

    //合計での試行回数
    unsigned int try_time = 3 * 1e6;
};

#endif // MONTECARLOTREESEARCH_H
