#include "genetic.h"

Genetic::Genetic() :
    mt(rnd()),
    manager(12,12,false,60)
{
    //乱択で生成する
    while(agents.size() < agent_num)
        agents.emplace_back(GeneticAgent());


}

void Genetic::run(){

    for(int gen = 0; gen < max_gen; ++gen){
        startTournament();

    }

}

int Genetic::retRandom(int st, int en){
    std::uniform_int_distribution<> rand(st, en);

    return rand(mt);
}

void Genetic::startTournament(){

    std::vector<GeneticAgent> new_agents;

    //ここで最適なチームを出す
    auto battleTournament = [&](std::vector<GeneticAgent>& input_data){

        //まだ規定数にたどり着いてないAgentのポインタと、戦った相手のポインタを持つ(可読性酷いね)
        std::vector<std::pair<GeneticAgent*, std::unordered_set<GeneticAgent*> >> values;

        for(unsigned int index = 0; index < input_data.size(); ++index)
            values.emplace_back(std::make_pair(&(input_data.at(index)), std::unordered_set<GeneticAgent*>() ) );//初期ではsetは空

        while(values.size() > 1){
            int index = retRandom(0, values.size() - 2);//配列の最後尾とこのindexを戦わせる
            //戦闘済ならやり直し
            if(values.back().second.count(values.at(index).first))
                continue;

            int data = buttleAgents( *(values.back().first), *(values.at(index).first) );

            (data ? values.back().first->win_count
                  : values.at(index).first->win_count)++;

            values.back().first->try_count++;
            values.at(index).first->try_count++;

        }

        std::sort(input_data.begin(), input_data.end());
        new_agents.push_back(input_data.back());
    };

    while(!agents.empty()){

        //vectorのeraseは効率が悪いのですが、ランダムにアクセスする都合上これでいきます

        std::vector<GeneticAgent> tournaments;
        int siz = std::min(tournament_size, static_cast<int>(agents.size()) );

        //乱択でトーナメント生成
        for(int count = 0; count < siz; ++count){

            int rand_index = retRandom(0, agents.size() - 1);

            tournaments.emplace_back( std::move(agents.at(rand_index)) );

            agents.erase(agents.begin() + rand_index);
        }

        battleTournament(tournaments);

    }

    agents = std::move(new_agents);

}


bool Genetic::buttleAgents(GeneticAgent& first, GeneticAgent& second){


    auto buttle = [&](bool flag){//2で割った余りによって反転する(得点が同じ時の処理が順番依存なので一応)
        int turn = retRandom(60, 120);
        std::pair<int,int> size = std::make_pair( retRandom(8,12), retRandom(8, 12) );

        //visualizerは表示しない
        manager.resetManager(size.first, size.second, false, turn);

        //firstが勝ったらtrue
        return (flag ? manager.simulationGenetic(first, second)
                     : ! manager.simulationGenetic(second, first));
    };



    int win_count = 0;

    for(int count = 0; count < buttle_count; ++count)
        win_count += buttle(count % 2);

    return win_count * 2 >= buttle_count * 2;
}
