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
                return std::make_pair(std::make_tuple((move % 2) + 1, x_list.at(moves.at(0)), y_list.at(moves.at(0))), std::make_tuple(1 + (int)(move % 4 > 1), x_list.at(moves.at(1)), y_list.at(moves.at(1))));
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

                std::uniform_int_distribution<> rand_t(0, turn_count - 1);

                std::vector<int> turns({rand_t(mt), rand_t(mt)});

                manager_ptr->resetManager(rand_size(mt), rand_size(mt), false, turn_count);

                std::vector<std::string> data;

                for(int count = 0; count < turn_count; ++count){
                    for(int side = 0; side < 2; ++side){
                        std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> move = calc_move(side, manager_ptr);

                        if(turns.at(side) == count){

                            data.emplace_back(procon::BinaryIo::exportToString(manager_ptr->getField()));

                            // 6*2 + 4 = 16(2bit)

                            data.back() += std::bitset<2>(std::get<0>(move.first)).to_string();
                            data.back() += std::bitset<2>(std::get<1>(move.first) + 1 ).to_string();
                            data.back() += std::bitset<2>(std::get<2>(move.first) + 1 ).to_string();
                            data.back() += "00";
                            data.back() += std::bitset<2>(std::get<0>(move.second)).to_string();
                            data.back() += std::bitset<2>(std::get<1>(move.second) + 1).to_string();
                            data.back() += std::bitset<2>(std::get<2>(move.second) + 1).to_string();
                            data.back() += "00";
                        }

                        manager_ptr->agentAct(side, 0, move.first);
                        manager_ptr->agentAct(side, 1, move.second);
                    }
                    manager_ptr->changeTurn();
                }

                std::vector<std::pair<int,int>> points = manager_ptr->getField().getPoints(false);
                // [0.65536) ave 32768
                for(int index = 0; index < 2; ++index){

                    unsigned diff = ((points.at(0).first + points.at(0).second - (points.at(1).first + points.at(1).second)) * (index & 1 ? -1 : 1)) + (1<<15);
                    for(int bit = 15; bit >= 0; --bit)
                        data.at(index) += '0' + ((diff >> bit) & 1);

                }

                for(auto str_data : data){
                    std::string out;
                    for(int index = 0; index < str_data.size() / 8; ++index){
                        unsigned char val = 0;
                        for(int num = 0; num < 8; ++num)
                            val += (str_data[8 * index + num] - '0') << (7 - num);
                        out += val;
                    }

                    logger->info(out);
                }
            }

        }, cpu_index);

    for(auto& th : threads)
        th.join();

    std::cout << "finished" << std::endl;

}
