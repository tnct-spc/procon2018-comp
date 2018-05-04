#include "genetic.h"
#include "simulatedannealing.h"

int main()
{

    std::cout << "please select simulator" << std::endl;
    std::cout << "0 : GeneticAlgorithm" << std::endl;
    std::cout << "1 : SimulatedAnnealing" << std::endl;
    std::cout << std::endl;

    int sim_number = -1;
    while(1){
        std::cout << "simulator number : ";
        std::cin >> sim_number;
        if(sim_number>= 0 && sim_number <= 1)
            break;

        std::cout << "incorrect" << std::endl;
        std::cout << std::endl;
    }
    std::cout << std::endl;

    int algo_number = -1;

    std::cout << "please select algorithm" << std::endl;
    std::cout << "0: GeneticAlgo" << std::endl;

    while(1){
        std::cout << "algorithm number : ";
        std::cin >> algo_number;
        if(algo_number >= 0 && algo_number <= 0)
            break;
        std::cout << "incorrect" << std::endl;
    }


    if(sim_number == 0){
        Genetic gen(algo_number);
        gen.run();
    }
    else if(sim_number == 1){
        SimulatedAnnealing sim(algo_number);
        sim.run();
    }

    return 0;
}
