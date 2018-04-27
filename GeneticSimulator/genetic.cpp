#include "genetic.h"

Genetic::Genetic() :
    mt(rnd())
{
    //乱択で生成する
    while(agents.size() < agent_num)
        agents.insert(GeneticAgent());

}

void Genetic::run(){

}

int Genetic::retRandom(int st, int en){
    std::uniform_int_distribution<> rand(st, en);

    return rand(mt);
}


int Genetic::buttleAgents(GeneticAgent& first, GeneticAgent& second){


    auto buttle = [&](bool flag){//2で割った余りによって反転する(得点が同じ時の処理が順番依存なので一応)
        int turn = retRandom(60, 120);
        std::pair<int,int> size = std::make_pair( retRandom(8,12), retRandom(8, 12) );

        //visualizerは表示しない
        GameManager manager(size.first, size.second, false, turn);

        return (flag ? manager.simulationGenetic(first, second)
                     : manager.simulationGenetic(second, first));
    };



    int win_count = 0;

    for(int count = 0; count < buttile_count; ++count)
        win_count += buttle(count % 2);

    return win_count;
}
