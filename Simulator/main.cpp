#include "genetic.h"
#include "simulatedannealing.h"

int main()
{

    std::cout << "please select simulator" << std::endl;
    std::cout << "0 : GeneticAlgorithm" << std::endl;
    std::cout << "1 : SimulatedAnnealing" << std::endl;
    std::cout << std::endl;

    int algo_num = -1;
    while(1){
        std::cout << "simulator number : ";
        std::cin >> algo_num;
        if(algo_num >= 0 && algo_num <= 1)
            break;

        std::cout << "incorrect" << std::endl;
        std::cout << std::endl;
    }
    std::cout << std::endl;

    if(algo_num == 0){
        Genetic gen;
        gen.run();
    }
    else if(algo_num == 1){
        SimulatedAnnealing sim;
        sim.run();
    }

    return 0;
}
