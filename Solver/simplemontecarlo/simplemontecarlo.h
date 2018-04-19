#ifndef SIMPLEMONTECARLO_H
#define SIMPLEMONTECARLO_H

#include "algorithmwrapper.h"
#include "gamesimulator.h"
#include "gamesimulator.h"
#include "gamemanager.h"

#include <iostream>



class SimpleMonteCalro : public AlgorithmWrapper
{
    //using AlgorithmWrapper::AlgorithmWrapper;
public:
    SimpleMonteCalro(std::shared_ptr<GameManager> manager_ptr);
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
private:
    //試行回数 適当に変えてね
    unsigned int trial = 5;
    GameSimulator sim;


};

#endif // SIMPLEMONTECARLO_H
