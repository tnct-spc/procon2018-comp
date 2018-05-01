#include "simulatedannealing.h"

SimulatedAnnealing::SimulatedAnnealing(int algo_number) :
    algo_number(algo_number)
{

}

void SimulatedAnnealing::run(){


    //初期エージェントを選択
    if(algo_number == 0)
        agent = new GeneticAgent(7);




}
