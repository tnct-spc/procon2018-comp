#include "genetic.h"

Genetic::Genetic(int algo_number) :
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
            agents.emplace_back(GeneticAgent(7));
        if(algo_number == 2)
            agents.emplace_back(GeneticAgent(10));
    }
}

void Genetic::run(){

    auto debug_output = [&]{

        // 履歴を残さず、最新のものだけを保存する
        // procon2018-compと同じ階層にbuildディレクトリがある状態でpath書いてます
        std::ofstream output("../../procon2018-comp/Data/GeneticAgent/ga_data");

        for(auto val : agents){


            std::cout << "{ ";
            output << "{";
            int siz = val.size;

            const std::vector<double>& data = val.getData();
            for(int index = 0; index < siz-1; ++index){
                std::cout << data.at(index) << ", ";
                output << data.at(index) << ", ";
            }

            std::cout << data.at(siz - 1) << " }" << std::endl;
            output << data.at(siz - 1) << " }" << std::endl;
        }
        std::cout << std::endl;

        output.close();
    };



    for(int gen = 0; gen < max_gen; ++gen){

        std::cout << "gen " << gen + 1 << " started" << std::endl << std::endl;
        //std::cout << "agent count : " << agents.size() << std::endl;

        //選別してtournament_count体に絞り込む
        startTournament();

        //if(gen == max_gen - 1 || gen % (max_gen / 200) == 0)
        debug_output();

        if(gen != max_gen - 1)
            generateAgents();
        else
            std::cout << "simulation complete" << std::endl;

    }

}

int Genetic::retRandom(int st, int en){
    std::uniform_int_distribution<> rand(st, en);

    return rand(mt);
}

void Genetic::generateAgents(){

    //std::cout << "generate agent" << std::endl << std::endl;

    int generate_count = agent_num - tournament_count;//この数だけ新たに生成する

    auto mutate = [&]{
        int target = retRandom(0,9);
        int siz = agents.at(target).size;
        std::vector<double> target_data = agents.at(target).getData();

        //これだけの数を適当に切り出して乱数で決め直す
        int change_count = retRandom(1, siz / 3);

        std::vector<int> val;
        if(algo_number == 0)
            val.resize(7);
        if(algo_number == 2)
            val.resize(10);

        std::iota(val.begin(), val.end(), 0);

        std::uniform_real_distribution<> rand_double(0.0, 1.0);

        //決め直す所が被らないようにする
        for(int count = 0; count < change_count; ++count){

            int index = (retRandom(0, val.size() - 1));
            target_data.at(index) = rand_double(mt);
            val.erase(std::next(val.begin(), index));
        }

        GeneticAgent new_agent(agents.at(0).size, false);
        new_agent.setData(target_data);

        agents.emplace_back(std::move(new_agent));

    };

    auto crossover = [&]{
        int target_1 = retRandom(0,9);
        int target_2 = target_1;

        while(target_2 != target_1)
            target_2 = retRandom(0,9);

        int siz = agents.at(target_1).size;


        std::vector<double> data_1 = agents.at(target_1).getData();
        std::vector<double> data_2 = agents.at(target_2).getData();

        std::vector<double> agent_data(siz);

        for(int index = 0; index < siz; ++index){

            double data_diff = std::abs(data_1.at(index) - data_2.at(index));
            double min_val = std::min(data_1.at(index), data_2.at(index));

            //はい
            std::uniform_real_distribution<> rand_double(std::max(0.0, min_val - (data_diff * change_alpha_val)), std::min(1.0, min_val + (data_diff * (1.0 + change_alpha_val) )));

            agent_data.at(index) = rand_double(mt);
        }


        /*
        for(int index = 0; index < siz; ++index)
            agent_data.at(index) = (retRandom(0, 1)
                                    ? data_2.at(index)
                                    : data_1.at(index) );
        */


        // 交叉時にも一定確率で変化させる
        for(int index = 0; index < siz; ++index){
            if(retRandom(0,10000) < 10000 * cross_change_per){

                std::uniform_real_distribution<> rand_double(std::max(-cross_change_val, -agent_data.at(index) ), std::min(cross_change_val, 1 - agent_data.at(index)) );

                agent_data.at(index) += rand_double(mt);
                agent_data.at(index) = std::min(agent_data.at(index), 1.0);
                agent_data.at(index) = std::max(agent_data.at(index), 0.0);
            }
        }

        GeneticAgent new_agent(agents.at(0).size, false);
        new_agent.setData(agent_data);

        agents.emplace_back(std::move(new_agent));
    };

    for(int count = 0; count < generate_count; ++count){

        double random_value = 0.00001 * retRandom(0,100000);

        if(random_value < mutate_per){
            //突然変異
            mutate();

        }else{
            //交叉
            crossover();
        }
    }
}

void Genetic::startTournament(){

    std::vector<GeneticAgent> new_agents;

    //ここで最適なチームを出す
    auto buttleTournament = [&](std::vector<GeneticAgent>& input_data){

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

        double max_val = -1;
        std::vector<int> max_index_list;

        for(unsigned int index = 0; index < input_data.size(); ++index){

            double value = 1.0 * input_data.at(index).win_count / input_data.at(index).try_count;

            if(value > max_val){
                max_val = value;
                max_index_list.clear();
                max_index_list.emplace_back(index);

            }else if(value == max_val)
                max_index_list.emplace_back(index);

            //std::cout << value << " ";
        }
        //std::cout << " max_per : " << max_val << std::endl << std::endl;

        int max_index = max_index_list.at(retRandom(0, max_index_list.size() - 1));

        new_agents.push_back(input_data.at(max_index));
        input_data.erase(std::next(input_data.begin(), max_index));

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

        std::cout << "tournament " << count + 1 << " started" << std::endl;

        buttleTournament(tournaments);

    }

    agents = std::move(new_agents);

}


bool Genetic::buttleAgents(GeneticAgent& first, GeneticAgent& second){



    auto buttle = [&](bool flag, int index){//2で割った余りによって反転する(得点が同じ時の処理が順番依存なので一応)
        int turn = retRandom(60, 120);
        std::pair<int,int> size = std::make_pair( retRandom(8,12), retRandom(8, 12) );

        //visualizerは表示しない
        managers.at(index)->resetManager(size.first, size.second, false, turn);

        // std::cout << "buttle" << std::endl;
        //firstが勝ったらtrue
        return (flag ? managers.at(index)->simulationGenetic(first, second, algo_number)
                     : ! managers.at(index)->simulationGenetic(second, first, algo_number));
    };



    int win_count = 0;

    std::vector<std::thread> threads(cpu_num);

    for(int index = 0; index < cpu_num; ++index){

        int thread_buttle_cou = buttle_count / cpu_num + (buttle_count % cpu_num > index);

        threads.at(index) = std::thread( [=,&win_count](int buttle_cou){

            std::lock_guard<std::mutex> lock(mtx);

            int now_count = 0;

            //buttle_count / cpu_num回繰り返す
            while(now_count < buttle_cou){

                int point_flag = buttle(now_count % 2, index);

                if(point_flag == -1)//引き分けの場合
                    continue;

                ++now_count;
                win_count += point_flag;
            }
        }, thread_buttle_cou );

    }
    for(int index = 0; index < cpu_num; ++index){
        threads.at(index).join();
    }

    return win_count * 2 >= buttle_count;
}
