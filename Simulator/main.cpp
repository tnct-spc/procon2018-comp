#include "genetic.h"
#include "simulatedannealing.h"
#include "doubleagentgenetic.h"

int main()
{

    std::cout << "please select simulator" << std::endl;
    std::cout << "0 : GeneticAlgorithm" << std::endl;
    std::cout << "1 : SimulatedAnnealing" << std::endl;
    std::cout << "2 : DoubleAgentGenetic" << std::endl;
    std::cout << std::endl;

    int sim_number = -1;
    while(1){
        std::cout << "simulator number : ";
        std::cin >> sim_number;
        if(sim_number>= 0 && sim_number <= 2)
            break;

        std::cout << "incorrect" << std::endl;
        std::cout << std::endl;
    }
    std::cout << std::endl;

    int algo_number = -1;

    if(sim_number == 2){
        //現時点では一つしかないので
        algo_number = 0;

    }else{

        std::cout << "please select algorithm" << std::endl;
        std::cout << "0: GeneticAlgo" << std::endl;
        std::cout << "2: SimpleAlgorithm" << std::endl;

        while(1){
            std::cout << "algorithm number : ";
            std::cin >> algo_number;
            if(algo_number == 0 || algo_number == 2)
                break;
            std::cout << "incorrect" << std::endl;
        }
    }


    if(sim_number == 0){
        Genetic gen(algo_number);
        gen.run();
    }
    else if(sim_number == 1){
        SimulatedAnnealing sim(algo_number);
        sim.run();
    }
    else if(sim_number == 2){
        DoubleAgentGenetic dgen(algo_number);
        dgen.run();
    }

    return 0;
}
