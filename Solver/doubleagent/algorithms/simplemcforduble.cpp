#include "simplemcforduble.h"

SimpleMCForDuble::SimpleMCForDuble(std::shared_ptr<GameManager> manager, std::vector<std::shared_ptr<AgentWrapper>> agents, int side) :
    manager(manager),
    side(side),
    cpu_num(std::thread::hardware_concurrency()),
    field(manager->getField()),
    agents(agents)
{
    mt = std::mt19937(rnd());

}

std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> SimpleMCForDuble::changeTurn(std::shared_ptr<GameManager> manager_ptr){

    // 有効手から重みをつけて結果を返す

    std::vector<std::vector<std::pair<double,std::tuple<int,int,int>>>> can_move_list;

    procon::Field& ptr_field = manager_ptr->getField();

    for(int index = 0; index < 2; ++index)
        can_move_list.at(index).push_back(std::make_pair(0.0, std::make_tuple(0, 0, 0)));

    auto agent_move = [&](int agent){

        std::pair<int,int> agent_pos = ptr_field.getAgent(side, agent);

        auto evaluate = [&](int count, bool is_delete){

            double value = agents.at(agent)->evaluateMove(count, is_delete);

            if(value > minus_bound)//置けないパターンがあるのでそれを切る
                can_move_list.at(agent).push_back(std::make_pair(std::pow(value - minus_bound, value_weight), std::make_tuple(is_delete + 1, x_list.at(count), y_list.at(count))));

        };

        for(int count = 0; count < 9; ++count){

            std::pair<int,int> new_pos = agent_pos;
            new_pos.first += x_list.at(count);
            new_pos.second += y_list.at(count);

            if(new_pos.first < 0 || new_pos.first >= ptr_field.getSize().first || new_pos.second < 0 || new_pos.second >= ptr_field.getSize().second)
                continue;

            int state = ptr_field.getState(new_pos.first, new_pos.second).first;

            if(state != (side ? 1 : 2))
                evaluate(count, false);
            if(state && count != 8)
                evaluate(count, true);
        }

    };

    // can_move_listに有り得る手を列挙している
    agent_move(0);
    agent_move(1);

}

std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> SimpleMCForDuble::calcMove(){
    // ここがagentActから呼び出される

    auto rand = [&](int max){
        std::uniform_int_distribution<> dist(0, max);
        return dist(mt);
    };

    // answer[行動] = (勝数,試行回数)
    // 冗長すぎるのでtypedef使わない？そうした方がよくないですか
    std::map<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>, std::pair<int,int>> answer;

    std::vector<std::future<std::map<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>,std::pair<int,int>>>> threads;
    for(int count = 0; count < cpu_num; ++count)
        threads.push_back(std::async([&]{

            clock_t start = clock();

            // 返却する値
            std::map<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>,std::pair<int,int>> return_map;

            // 一定時間が経つまで
            while((double)(clock() - start) / CLOCKS_PER_SEC < calc_time){

                //ここに書く
                int turn_count = manager->getFinalTurn() - manager->getTurnCount();

                // managerの生成,初期化
                std::shared_ptr<GameManager> manager_ptr = std::make_shared<GameManager>(8, 8, false, 60);
                manager_ptr->setField(field, manager->getTurnCount(), manager->getFinalTurn());

                // 相手のアルゴリズムを決定
                std::shared_ptr<AlgorithmWrapper> enemy_algo = std::make_shared<GeneticAlgo>(manager_ptr);

                // このプレイアウト時の最初の手
                std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> first_move;

                for(int count = 0; count < turn_count; ++count){
                    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> my_move = changeTurn(manager_ptr);

                    if(!count)first_move = my_move;

                    // 相手の動き
                    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> enemy_move = enemy_algo->agentAct(!side);

                    manager_ptr->agentAct(side, 0, my_move.first);
                    manager_ptr->agentAct(side, 1, my_move.second);
                    manager_ptr->agentAct(!side, 0, enemy_move.first);
                    manager_ptr->agentAct(!side, 1, enemy_move.second);
                    manager_ptr->changeTurn();
                }
                // 得点を計算する
                std::pair<int,int> my_point = manager->getField().getPoints(0,false);
                std::pair<int,int> enemy_point = manager->getField().getPoints(1,false);

                // 引き分けでないなら試行回数を増やしておく
                if(my_point.first + my_point.second != enemy_point.first + enemy_point.second)
                    ++return_map[first_move].second;

                // 勝ったなら勝利数を増やしておく
                if(my_point.first + my_point.second > enemy_point.first + enemy_point.second)
                    ++return_map[first_move].first;
            }

            return return_map;
        }));

    for(auto &th : threads){
        std::map<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>,std::pair<int,int>> return_val = th.get();

        for(auto value : return_val){
            answer[value.first].first += value.second.first;
            answer[value.first].second += value.second.second;
        }
    }

}
