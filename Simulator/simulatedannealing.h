#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include "gamemanager.h"
#include "geneticalgo/geneticalgo.h"

class SimulatedAnnealing
{
public:
    SimulatedAnnealing(int algo_number);
    void run();

private:
    int algo_number;

    const int max_try = 1e5;

    GeneticAgent* agent;
};

#endif // SIMULATEDANNEALING_H
