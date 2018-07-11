#include "nashequilibrium.h"

NashEquilibrium::NashEquilibrium(const procon::Field& field, bool side, int now_turn, int max_turn, std::vector<std::shared_ptr<AgentWrapper>> agents) :
    field(field),
    agents(agents),
    side(side),
    cpu_num(std::thread::hardware_concurrency()),
    now_turn(now_turn),
    max_turn(max_turn)
{
    mt = std::mt19937(rnd());

    manager_vec.resize(cpu_num);
    for(auto& ptr : manager_vec)
        ptr = std::make_shared<GameManager>(8, 8, false);


}

std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> NashEquilibrium::changeTurn(std::shared_ptr<GameManager> manager_ptr, bool is_eq, int eval_side){

    if(eval_side == -1)
        eval_side = side;

    // そんなコピペコードが許されると思うなよ

    std::vector<std::vector<std::pair<double,std::tuple<int,int,int>>>> can_move_list(2);

    procon::Field& ptr_field = manager_ptr->getField();

    for(int index = 0; index < 2; ++index)
        can_move_list.at(index).push_back(std::make_pair(0.0, std::make_tuple(0, 0, 0)));

    auto agent_move = [&](int agent){

        std::pair<int,int> agent_pos = ptr_field.getAgent(eval_side, agent);

        auto evaluate = [&](int count, bool is_delete){

            double value = agents.at(agent)->evaluateMove(count, is_delete, now_turn);

            if(value > minus_bound)//置けないパターンがあるのでそれを切る
                can_move_list.at(agent).push_back(std::make_pair((is_eq ? 1 : std::pow((value - minus_bound) * value_ratio, value_weight)), std::make_tuple(is_delete + 1, x_list.at(count), y_list.at(count))));

        };

        for(int count = 0; count < 9; ++count){

            std::pair<int,int> new_pos = agent_pos;
            new_pos.first += x_list.at(count);
            new_pos.second += y_list.at(count);

            if(new_pos.first < 0 || new_pos.first >= ptr_field.getSize().first || new_pos.second < 0 || new_pos.second >= ptr_field.getSize().second)
                continue;

            int state = ptr_field.getState(new_pos.first, new_pos.second).first;

            if(state != (eval_side ? 1 : 2))
                evaluate(count, false);
            if(state && count != 8)
                evaluate(count, true);
        }

    };

    // can_move_listに有り得る手を列挙している
    agent_move(0);
    agent_move(1);

    if(!is_eq)
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
        dist.at(index) = (is_eq ? std::uniform_real_distribution<>(0, can_move_list.at(index).size())
                                : std::uniform_real_distribution<>(0, can_move_list.at(index).back().first));

    while(1){
        std::vector<std::tuple<int,int,int>> my_move(2); // 行動の暫定値

        // 二分探索でランダムに求める
        for(int index = 0; index < 2; ++index)
            my_move.at(index) = (is_eq ? can_move_list.at(index).at(std::min(static_cast<int>(can_move_list.at(index).size() - 1), static_cast<int>(dist.at(index)(mt)))).second
                                       : (*std::lower_bound(can_move_list.at(index).begin(), can_move_list.at(index).end(), std::make_pair(dist.at(index)(mt), std::make_tuple(0, 0, 0)))).second);

        std::pair<int,int> old_pos_1 = manager_ptr->getField().getAgent(eval_side, 0);
        std::pair<int,int> old_pos_2 = manager_ptr->getField().getAgent(eval_side, 1);

        std::pair<int,int> new_pos_1 = old_pos_1;
        new_pos_1.first += std::get<1>(my_move.at(0));
        new_pos_1.second += std::get<2>(my_move.at(0));

        std::pair<int,int> new_pos_2 = old_pos_2;
        new_pos_2.first += std::get<1>(my_move.at(1));
        new_pos_2.second += std::get<2>(my_move.at(1));

        // コンフリクトが起きないなら終了
        if(!( ( std::get<0>(my_move.at(0)) != 1 && old_pos_1 == new_pos_2) || (std::get<0>(my_move.at(1)) != 1 && new_pos_1 == old_pos_2) || new_pos_1 == new_pos_2)){
            return std::make_pair(my_move.at(0), my_move.at(1));
        }
    }

}

std::map<std::vector<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>>, std::pair<int,int>> NashEquilibrium::playoutMove(bool is_eq){
    // answer[行動] = (勝数,試行回数)
    // 冗長すぎるのでtypedef使わない？そうした方がよくないですか
    std::map<std::vector<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>>, std::pair<int,int>> answer;

    std::vector<std::future<std::map<std::vector<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>>,std::pair<int,int>>>> threads;

    for(int count = 0; count < cpu_num; ++count)
        threads.push_back(std::async([&](int cpu_index){

            // std::lock_guard<std::mutex> lock(mtx);

            clock_t start = clock();

            // 返却する値
            std::map<std::vector<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>>,std::pair<int,int>> return_map;

            // 一定時間が経つまで
            while((double)(clock() - start) / CLOCKS_PER_SEC < calc_time){

                //ここに書く
                int turn_count = std::min(end_turn, max_turn  - now_turn);

                // managerの生成,初期化
                std::shared_ptr<GameManager> manager_ptr = manager_vec.at(cpu_index);

                manager_ptr->setField(field, now_turn, max_turn);

                // このプレイアウト時の最初の手
                std::vector<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>> first_move;

                for(int count = 0; count < turn_count; ++count){
                    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> my_move = changeTurn(manager_ptr, is_eq, side);

                    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> enemy_move = changeTurn(manager_ptr, is_eq, !side);

                    std::cout << "after " << agents.at(0)->side << std::endl;

                    if(!count)first_move = std::vector<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>>({my_move, enemy_move});

                    manager_ptr->agentAct(side, 0, my_move.first);
                    manager_ptr->agentAct(side, 1, my_move.second);
                    manager_ptr->agentAct(!side, 0, enemy_move.first);
                    manager_ptr->agentAct(!side, 1, enemy_move.second);
                    manager_ptr->changeTurn(false);
                }
                // 得点を計算する
                manager_ptr->getField().updatePoint();
                std::pair<int,int> my_point = manager_ptr->getField().getPoints(side, false);
                std::pair<int,int> enemy_point = manager_ptr->getField().getPoints(!side, false);

                // 引き分けでないなら試行回数を増やしておく
                if(my_point.first + my_point.second != enemy_point.first + enemy_point.second)
                    ++return_map[first_move].second;

                // 勝ったなら勝利数を増やしておく
                if(my_point.first + my_point.second > enemy_point.first + enemy_point.second)
                    ++return_map[first_move].first;
            }

            return std::move(return_map);
        }, count));

    int cnt = 0;
    for(auto &th : threads){
        std::map<std::vector<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>>,std::pair<int,int>> return_val = th.get();
        std::cout << "cnt th" << cnt << std::endl;
        for(auto value : return_val){
            answer[value.first].first += value.second.first;
            answer[value.first].second += value.second.second;
            std::cout << answer[value.first].first << " , " << answer[value.first].second << " hoge "<<std::endl;
            cnt += value.second.second;
        }
        std::cout << "end" << std::endl;
    }
    std::cout << "cnt : " << cnt << std::endl;

    return std::move(answer);
}

std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> NashEquilibrium::calcMove(){

    // 動きに対応するindexの一覧
    std::vector<std::map<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>, int>> move_index(2);

    // {利得,選択確率}の一覧
    std::vector<std::vector<double>> gain_list(2);

    // 利得表への入力(穴開きがある可能性があり、そこをなんとかしたい)
    auto get_data = [&]{
        std::map<std::vector<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>>, std::pair<int,int>> answer = playoutMove(true);
        std::cout<< "playout end" << std::endl;
        for(auto value : answer)
            for(int index = 0; index < 2; ++index)
                if(move_index.at(index).find(value.first.at(index)) == move_index.at(index).end()){
                    int siz = move_index.at(index).size();
                    move_index.at(index).insert(std::make_pair(value.first.at(index),siz));
                }
        std::cout<< "for end" << move_index.at(0).size() << std::endl;
        gain_list = std::vector<std::vector<double>>(move_index.at(0).size(), std::vector<double>(move_index.at(1).size(), 0.5));
        for(auto value : answer)
            gain_list.at(move_index.at(0)[value.first.at(0)]).at(move_index.at(1)[value.first.at(1)]) = 1.0 * value.second.first / value.second.second;
        std::cout<< "end!!!!" << std::endl;
    };
    get_data();

    double max_point = -100000;
    std::pair<int,int> max_pair = {0, 0};
    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> max_move = std::make_pair(std::make_tuple(0, 0, 0), std::make_tuple(0, 0, 0));

    //nash hogehoge

    return max_move;

}
