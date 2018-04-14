#ifndef SIMPLEMONTECARLO_H
#define SIMPLEMONTECARLO_H

#include "algorithmwrapper.h"
#include "simplemontecarlo/gamesimulator.h"
#include "gamesimulator.h"
#include "gamemanager.h"

#include <iostream>



class SimpleMonteCalro : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
private:
    //試行回数 適当に変えてね
    unsigned int trial = 10;

};

#endif // SIMPLEMONTECARLO_H
