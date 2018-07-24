#include "testgetfielddata.h"

TestGetFieldData::TestGetFieldData() :
    cpu_num(std::thread::hardware_concurrency())
{
    mt = std::mt19937(rnd());

    manager_vec.resize(cpu_num);
    for(auto& ptr : manager_vec)
        ptr = std::make_shared<GameManager>(8, 8, false);
}

void TestGetFieldData::run(){

    std::vector<std::thread> threads(cpu_num);
    std::uniform_int_distribution<> rand_move(0, 324);

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    auto calc_move = [&](int side, std::shared_ptr<GameManager> manager_ptr){

        while(1){
            int move = rand_move(mt);
            std::vector<int> moves = {move / 36, move % 9};

            if(!manager_ptr->getField().canPut(side, moves.at(0), moves.at(1)))
                continue;

            bool flag = true;

            for(int agent = 0; agent < 2; ++agent){
                bool is_delete = (agent ? move % 4 > 1 : move % 2);
                std::pair<int,int> now_pos = manager_ptr->getField().getAgent(side, agent);
                std::pair<int,int> new_pos = now_pos;
                new_pos.first += x_list.at(moves.at(agent));
                new_pos.second += y_list.at(moves.at(agent));
                int state = manager_ptr->getField().getState(new_pos.first, new_pos.second).first;
                if((!state && is_delete) || (state == (side ? 1 : 2) && !is_delete)){
                    flag = false;
                    break;
                }
            }
            if(flag)
                return std::vector<std::tuple<int,int,int>>({std::make_tuple(x_list.at(moves.at(0)), y_list.at(moves.at(0)), move % 2), std::make_tuple(x_list.at(moves.at(1)), y_list.at(moves.at(1)), (int)(move % 4 > 1))});
        }

    };

    for(int cpu_index = 0; cpu_index < cpu_num; ++cpu_index)

        threads.at(cpu_index) = std::thread([&](int cpu){

            std::lock_guard<std::mutex> lock(mtx);

            clock_t start = clock();
            std::uniform_int_distribution<> rand_turn(60, 120);
            std::uniform_int_distribution<> rand_size(8, 12);

            while((double)(clock() - start) / CLOCKS_PER_SEC < calc_time){

                int turn_count = rand_turn(mt);
                std::shared_ptr<GameManager> manager_ptr = manager_vec.at(cpu);


                manager_ptr->resetManager(rand_size(mt), rand_size(mt), false, turn_count);

                // ここに初期Fieldによって定まるデータを入れる
                std::vector<double> field_data;
                // ここに途中の盤面や行動によって定まるデータを入れる
                std::vector<std::vector<double>> move_data;

                for(int count = 0; count < turn_count; ++count){
                    for(int side = 0; side < 2; ++side){
                        std::vector<std::tuple<int,int,int>> move = calc_move(side, manager_ptr);

                        for(int agent = 0;agent < 2; ++agent)
                            manager_ptr->agentAct(side, agent, move.at(agent));
                    }
                    manager_ptr->changeTurn();
                }

                std::vector<std::pair<int,int>> points = manager_ptr->getField().getPoints(false);
                int diff = points.at(0).first + points.at(0).second - (points.at(1).first + points.at(1).second);

                std::cout << "diff : " << diff << " , " << cpu << std::endl;

                // ここに値の格納処理をする

            }
        }, cpu_index);

    for(auto& th : threads)
        th.join();

    std::cout << "finished" << std::endl;

}
