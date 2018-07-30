#include "testgetfielddata.h"

TestGetFieldData::TestGetFieldData() :
    cpu_num(std::thread::hardware_concurrency())
{
    mt = std::mt19937(rnd());

    logger = spdlog::basic_logger_mt("TestGetFieldData", "../../procon2018-comp/Data/TestOptimazationFunction/input.csv");
    logger->set_pattern("%v");


    manager_vec.resize(cpu_num);
    for(auto& ptr : manager_vec)
        ptr = std::make_shared<GameManager>(8, 8, false);
}

void TestGetFieldData::run(){


    std::uniform_int_distribution<> rand_move(0, 323);

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
                return std::make_pair(std::make_tuple(x_list.at(moves.at(0)), y_list.at(moves.at(0)), move % 2), std::make_tuple(x_list.at(moves.at(1)), y_list.at(moves.at(1)), (int)(move % 4 > 1)));
        }

    };

    std::vector<std::thread> threads(cpu_num);

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
                /*
                const std::vector<double>& field_data = manager_ptr->getField().getFeatures();
                // ここに途中の盤面や行動によって定まるデータを入れる
                // [試行回数][相手][行動や盤面の情報] の三次元配列
                std::vector<std::vector<std::vector<double>>> move_data;
                */

                std::vector<std::string> data;

                for(int count = 0; count < turn_count; ++count){
                    // move_data.push_back(std::vector<std::vector<double>>(2));
                    for(int side = 0; side < 2; ++side){
                        std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> move = calc_move(side, manager_ptr);

                        data.emplace_back(procon::BinaryIo::exportToString(manager_ptr->getField()));

                        data.back() += std::to_string((!std::get<0>(move.first)) + 10 * (std::get<0>(move.first) == 1));
                        data.back() += std::to_string((!std::get<1>(move.first)) + 10 * (std::get<1>(move.first) == 1));
                        data.back() += std::to_string((!std::get<2>(move.first)) + 10 * (std::get<2>(move.first) == 1));
                        data.back() += std::to_string((!std::get<0>(move.second)) + 10 * (std::get<0>(move.second) == 1));
                        data.back() += std::to_string((!std::get<1>(move.second)) + 10 * (std::get<1>(move.second) == 1));
                        data.back() += std::to_string((!std::get<2>(move.second)) + 10 * (std::get<2>(move.second) == 1));


                        // move_data.back().at(side) = manager_ptr->getField().calcSituationFeature(move, side);

                        manager_ptr->agentAct(side, 0, move.first);
                        manager_ptr->agentAct(side, 1, move.second);
                    }
                    manager_ptr->changeTurn();
                }

                std::vector<std::pair<int,int>> points = manager_ptr->getField().getPoints(false);
                // [0.65536)
                unsigned diff = (points.at(0).first + points.at(0).second - (points.at(1).first + points.at(1).second)) + (1<<15);
                for(int bit = 15; bit >= 0; --bit)
                    data.back() += '0' + ((diff >> bit) & 1);


                /*
                std::string field_string = "-1";
                for(auto fdata : field_data)
                    field_string += "," + std::to_string(fdata);
                logger->info(field_string);

                // ここに値の格納処理をする
                for(auto data_vec : move_data){
                    for(int side = 0; side < 2; ++side){

                        std::string output_data;
                        for(auto mdata : data_vec.at(side))
                            output_data += std::to_string(mdata) + ",";
                        output_data += std::to_string(diff * (side ? -1 : 1));

                        logger->info(output_data);
                    }
                }
                */

            }

        }, cpu_index);

    for(auto& th : threads)
        th.join();

    std::cout << "finished" << std::endl;

}
