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

        procon::Field field(size.first, size.second, 16, -16);
        GameSimulator sim(field.getValue(), turn );

        sim.resetField(field.getField(), field.getAgents());

        for(int count = 0; count < turn; ++count){

        }

        return true;
    };



    int win_count = 0;

    for(int count = 0; count < buttile_count; ++count)
        win_count += buttle();

    return win_count;
}
