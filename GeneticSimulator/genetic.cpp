#include "genetic.h"

Genetic::Genetic() :
    mt(rnd())
{
    //乱択で生成する
    while(agents.size() < agent_num)
        agents.emplace_back(GeneticAgent());

    cpu_num = std::thread::hardware_concurrency();

    for(int index = 0; index < cpu_num; ++index){
        managers.push_back(new GameManager(12,12,false,60));
    }

}

void Genetic::run(){

    for(int gen = 0; gen < max_gen; ++gen){

        //選別して10体に絞り込む
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

        std::shuffle(input_data.begin(), input_data.end(), mt);

        for(auto& val : input_data){
            //初期化する
            val.try_count=0;
            val.win_count=0;
        }

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

            if(values.back().first->try_count >= tournament_buttle)
                values.erase(std::prev(values.end() ) );//末尾削除

            if(values.at(index).first->try_count >= tournament_buttle)
                values.erase(std::next(values.begin(), index) );
        }

        std::sort(input_data.begin(), input_data.end());

        for(unsigned int index = 0; index < input_data.size(); ++index){
            std::cout << 1.0 * input_data.at(index).win_count / input_data.at(index).try_count << " ";
        }
        std::cout << std::endl;

        new_agents.push_back(input_data.back());
        input_data.erase(std::prev(input_data.end() ) );

        for(auto& val : input_data){
            //再度初期化して選ばれなかった物を再投入
            val.try_count=0;
            val.win_count=0;
            agents.emplace_back(val);
        }
    };

    for(int count = 0; count < tournament_count; ++count){

        std::vector<GeneticAgent> tournaments;

        //乱択でトーナメント生成
        for(int rand_count = 0; rand_count < tournament_size;++rand_count){

            int rand_index = retRandom(0, agents.size() - 1);

            tournaments.emplace_back( std::move(agents.at(rand_index)) );

            agents.erase(agents.begin() + rand_index);
        }

        std::cout << "tournament started" << std::endl;

        battleTournament(tournaments);

    }

    agents = std::move(new_agents);

}


bool Genetic::buttleAgents(GeneticAgent& first, GeneticAgent& second){


    auto buttle = [&](bool flag, int index){//2で割った余りによって反転する(得点が同じ時の処理が順番依存なので一応)
        int turn = retRandom(60, 120);
        std::pair<int,int> size = std::make_pair( retRandom(8,12), retRandom(8, 12) );

        //visualizerは表示しない
        managers.at(index)->resetManager(size.first, size.second, false, turn);

        //firstが勝ったらtrue
        return (flag ? managers.at(index)->simulationGenetic(first, second)
                     : ! managers.at(index)->simulationGenetic(second, first));
    };



    int win_count = 0;

    std::vector<std::thread> threads(cpu_num);


    for(int index = 0; index < cpu_num; ++index){

        threads.at(index) = std::thread( [=,&win_count]{
            for(int count = 0;count<buttle_count/cpu_num;++count)win_count += buttle(count % 2, index);
        } );

    }
    for(int index = 0; index < cpu_num; ++index){
        threads.at(index).join();
    }

    return win_count * 2 >= (buttle_count / cpu_num) * cpu_num;
}
