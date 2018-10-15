#ifndef SIMPLEMONTECARLO_H
#define SIMPLEMONTECARLO_H

#include "algorithmwrapper.h"
#include "gamesimulator.h"
#include "gamemanager.h"

#include <iostream>
#include <time.h>



class SimpleMonteCalro : public AlgorithmWrapper
{
    //using AlgorithmWrapper::AlgorithmWrapper;
public:
    SimpleMonteCalro(std::shared_ptr<GameManager> manager_ptr);
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
    void setParams(std::vector<std::pair<QString, double>> params);

private:

    //合計での試行回数
    unsigned int try_time = 3 * 1e6;
    GameSimulator sim;


};

#endif // SIMPLEMONTECARLO_H
