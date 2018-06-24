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

void SimpleMCForDuble::changeTurn(GameManager* manager_ptr){

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

    agent_move(0);
    agent_move(1);

}

std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> SimpleMCForDuble::calcMove(){
    // ここがagentActから呼び出される

    auto rand = [&](int max){
        std::uniform_int_distribution<> dist(0, max);
        return dist(mt);
    };

    //answer[行動] = (勝数,試行回数)
    std::map<std::tuple<int,int,int>, std::pair<int,int>> answer;

    std::vector<std::future<std::map<std::tuple<int,int,int>,std::pair<int,int>>>> threads;
    for(int count = 0; count < cpu_num; ++count)
        threads.push_back(std::async([&]{

            clock_t start = clock();

            // 返却する値
            std::map<std::tuple<int,int,int>,std::pair<int,int>> return_map;

            // 一定時間が経つまで
            while((double)(clock() - start) / CLOCKS_PER_SEC < calc_time){

                //ここに書く
                int turn_count = 60 + rand(60);
                GameManager* manager_ptr = new GameManager(8 + rand(4), 8 + rand(4), false, turn_count);

                for(int count = 0; count < turn_count; ++count)
                    changeTurn(manager_ptr);
            }

            return return_map;
        }));

    for(auto &th : threads){
        std::map<std::tuple<int,int,int>,std::pair<int,int>> return_val = th.get();

        for(auto value : return_val){
            answer[value.first].first += value.second.first;
            answer[value.first].second += value.second.second;
        }
    }

}
