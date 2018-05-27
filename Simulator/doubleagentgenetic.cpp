#include "doubleagentgenetic.h"

DoubleAgentGenetic::DoubleAgentGenetic(int algo_number) :
    algo_number(algo_number),
    mt(rnd())
{

    cpu_num = std::thread::hardware_concurrency();

    for(int index = 0; index < cpu_num; ++index){
        managers.push_back(new GameManager(12,12,false,60));
    }

    //乱択で生成する
    while(agents.size() < agent_num){

        if(algo_number == 0) // この辺個別処理
            agents.emplace_back(GeneticAgent(6));
    }
}

void DoubleAgentGenetic::run(){

}

int DoubleAgentGenetic::retRandom(int st, int en){

    std::uniform_int_distribution<> rand(st, en);

    return rand(mt);
}
