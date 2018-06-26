#include "simplemcforduble.h"

SimpleMCForDuble::SimpleMCForDuble(std::shared_ptr<GameManager> manager, std::vector<std::shared_ptr<AgentWrapper>> agents, int side) :
    manager(manager),
    side(side),
    cpu_num(std::thread::hardware_concurrency()),
    field(manager->getField()),
    agents(agents)
{
    mt = std::mt19937(rnd());

    manager_vec.resize(4);
    for(auto& ptr : manager_vec)
        ptr = std::make_shared<GameManager>(8, 8, false);


}

std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> SimpleMCForDuble::changeTurn(std::shared_ptr<GameManager> manager_ptr){

    std::cerr << "start" << std::endl;

    // 有効手から重みをつけて結果を返す

    std::vector<std::vector<std::pair<double,std::tuple<int,int,int>>>> can_move_list(2);

    procon::Field& ptr_field = manager_ptr->getField();

    for(int index = 0; index < 2; ++index)
        can_move_list.at(index).push_back(std::make_pair(0.0, std::make_tuple(0, 0, 0)));

    auto agent_move = [&](int agent){

        std::pair<int,int> agent_pos = ptr_field.getAgent(side, agent);

        auto evaluate = [&](int count, bool is_delete){

            double value = agents.at(agent)->evaluateMove(count, is_delete);

            if(value > minus_bound)//置けないパターンがあるのでそれを切る
                can_move_list.at(agent).push_back(std::make_pair(std::pow((value - minus_bound) * value_ratio, value_weight), std::make_tuple(is_delete + 1, x_list.at(count), y_list.at(count))));

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

    for(int index = 0; index < 2; ++index){
        // 重み順にソート
        std::sort(can_move_list.at(index).begin(), can_move_list.at(index).end(), std::greater<std::pair<double,std::tuple<int,int,int>>>());

        // 重みの累積を取る
        for(int count = 1; count < can_move_list.at(index).size(); ++count)
            can_move_list.at(index).at(count).first += can_move_list.at(index).at(count - 1).first;
    }

    // 乱数生成器
    std::vector<std::uniform_real_distribution<>> dist(2);
    for(int index = 0; index < 2; ++index)
        dist.at(index) = std::uniform_real_distribution<>(0, can_move_list.at(index).back().first);

    while(1){
        std::vector<std::tuple<int,int,int>> my_move(2); // 行動の暫定値

        // 二分探索でランダムに求める
        for(int index = 0; index < 2; ++index)
            my_move.at(index) = (*std::lower_bound(can_move_list.at(index).begin(), can_move_list.at(index).end(), std::make_pair(dist.at(index)(mt), std::make_tuple(0, 0, 0)))).second;

        std::pair<int,int> old_pos_1 = manager->getField().getAgent(side, 0);
        std::pair<int,int> old_pos_2 = manager->getField().getAgent(side, 1);

        std::pair<int,int> new_pos_1 = old_pos_1;
        new_pos_1.first += std::get<1>(my_move.at(0));
        new_pos_1.second += std::get<2>(my_move.at(0));

        std::pair<int,int> new_pos_2 = old_pos_2;
        new_pos_2.first += std::get<1>(my_move.at(1));
        new_pos_2.second += std::get<2>(my_move.at(1));

        // コンフリクトが起きないなら終了
        if(!( ( std::get<0>(my_move.at(0)) != 1 && old_pos_1 == new_pos_2) || (std::get<0>(my_move.at(1)) != 1 && new_pos_1 == old_pos_2) || new_pos_1 == new_pos_2)){
            std::cerr << "end" << std::endl;
            return std::make_pair(my_move.at(0), my_move.at(1));
        }
    }

}

std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> SimpleMCForDuble::calcMove(){
    // ここがagentActから呼び出される

    // answer[行動] = (勝数,試行回数)
    // 冗長すぎるのでtypedef使わない？そうした方がよくないですか
    std::map<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>, std::pair<int,int>> answer;

    std::vector<std::future<std::map<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>,std::pair<int,int>>>> threads;

    for(int count = 0; count < cpu_num; ++count)
        threads.push_back(std::async([&](int cpu_index){

            // std::lock_guard<std::mutex> lock(mtx);

            clock_t start = clock();

            // 返却する値
            std::map<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>,std::pair<int,int>> return_map;

            // 一定時間が経つまで
            while((double)(clock() - start) / CLOCKS_PER_SEC < calc_time){

                //ここに書く
                int turn_count = manager->getFinalTurn() - manager->getTurnCount();

                std::cerr << std::this_thread::get_id() << " : " << cpu_index << std::endl;

                // managerの生成,初期化
                std::shared_ptr<GameManager> manager_ptr = manager_vec.at(cpu_index);

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

            return std::move(return_map);
        }, count));

    for(auto &th : threads){
        std::map<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>,std::pair<int,int>> return_val = th.get();

        for(auto value : return_val){
            answer[value.first].first += value.second.first;
            answer[value.first].second += value.second.second;
        }
    }

    double max_point = -100000;
    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> max_move = std::make_pair(std::make_tuple(0, 0, 0), std::make_tuple(0, 0, 0));

    for(auto value : answer){
        // 仮に(勝数 * 勝率)で出してみる
        double point = value.second.first * (1.0 * value.second.first / value.second.second);
        if(max_point < point)
            max_move = value.first;
    }
    return max_move;
}
