#include "genetic.h"

Genetic::Genetic() :
    mt(rnd())
{
    //乱択で生成する
    for(int count = 0; count < agent_num; ++count)
        agents.emplace_back(GeneticAgent());

}

void Genetic::run(){


}

int Genetic::retRandom(int st, int en){
    std::uniform_int_distribution<> rand(st, en);

    return rand(mt);
}


int Genetic::buttleAgents(GeneticAgent& first, GeneticAgent& second){


    auto buttle = [&]{
        int turn = retRandom(60, 120);
        std::pair<int,int> size = std::make_pair( retRandom(8,12), retRandom(8, 12) );

        //visualizerは表示しない
        GameManager manager(size.first, size.second, false, turn);

        return manager.simulationGenetic(first, second);
    };



    int win_count = 0;

    for(int count = 0; count < buttile_count; ++count)
        win_count += buttle();

    return win_count;
}
