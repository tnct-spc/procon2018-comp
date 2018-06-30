#include "montecarlowithalgo.h"

MontecarloWithAlgo::MontecarloWithAlgo(const procon::Field& field, int final_turn, bool side) :
    AlgorithmWrapper(field, final_turn, side),
    mt(rnd())
{
    cpu_num = std::thread::hardware_concurrency();

    mgr.resize(cpu_num);
    threads.resize(cpu_num);

    for(unsigned int index = 0; index < cpu_num; ++index)
        mgr.at(index) = new GameManager(12, 12, false, 60);

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> MontecarloWithAlgo::agentAct(int now_turn){

    int side_r = (side == 0 ? 1 : 0);//敵側

    std::vector<int> win_count;
    std::vector<int> try_count;
    int try_sum = 0;

    auto retRandom = [&](int st,int en){ //[st,en]
        std::uniform_int_distribution<> rand(st, en);

        return rand(mt);
    };

    auto playout = [&](int move, int cpu){

        GameManager* mgr_target = mgr.at(cpu);

        //*mgr_target = *manager;

        std::vector<double> val_1 = values.at(retRandom(0,19));
        std::vector<double> val_2 = values.at(retRandom(0,19));

        GeneticAgent* side_1 = new GeneticAgent(7, false);//ここはアルゴリズムによってサイズが変わる
        GeneticAgent* side_2 = new GeneticAgent(7, false);

        side_1->setData(val_1);
        side_2->setData(val_2);


        //mgr_target->resetManager(retRandom(8,12), retRandom(8,12), false, retRandom(60,120));

        mgr_target->setField(field, now_turn, final_turn);

        std::pair<int,int> agent_1_pos = field.getAgent(side, 0);
        std::pair<int,int> agent_2_pos = field.getAgent(side, 1);

        agent_1_pos.first += x_list.at(move / 9);
        agent_1_pos.second += y_list.at(move / 9);

        agent_2_pos.first += x_list.at(move % 9);
        agent_2_pos.second += y_list.at(move % 9);

        mgr_target->agentAct(side, 0, std::make_tuple( (field.getState(agent_1_pos.first, agent_1_pos.second).first == 2 ? 2 : 1) , x_list.at(move / 9), y_list.at(move / 9)));
        mgr_target->agentAct(side, 1, std::make_tuple( (field.getState(agent_2_pos.first, agent_2_pos.second).first == 2 ? 2 : 1) , x_list.at(move % 9), y_list.at(move % 9)));

        GeneticAlgo algo(field, final_turn, side,*side_2);
        std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> act = algo.agentAct(1);
        mgr_target->agentAct(side_r, 0, act.first);
        mgr_target->agentAct(side_r, 1, act.second);

        mgr_target->changeTurn();

        bool win = mgr_target->simulationGenetic(*side_1, *side_2, 0);

        //反転
        if(side == 1)
            win ^= 1;

        ++try_sum;

        // std::cout << (win ? "yes" : "no" ) << std::endl;

        return win;
    };


    std::vector<int> can_move_list;


    // これだとガバが発生しやすいので他のagentで試した有効手のみを候補に入れる
    /*
    for(int count = 0; count < 81; ++count)
        if(manager->canPut(side, count / 9, count % 9)){

            can_move_list.push_back(count);
            try_count.push_back(1);
            // win_count.push_back( playout(count) );
            win_count.push_back(0);

        }
    */

    //動きの候補を追加する
    auto addMoveList = [&]{
        std::set<std::pair<int,int>> move_list_1,move_list_2;

        //全てのエージェントで動きを試してみる
        for(int agent_num = 0; agent_num < 20; ++agent_num){
            GeneticAgent agent(7);
            agent.setData(values.at(agent_num) );
            GeneticAlgo algo(field, final_turn, side,agent);
            std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> act = algo.agentAct(side);
            move_list_1.insert( std::make_pair(std::get<1>(act.first), std::get<2>(act.first) ) );
            move_list_2.insert( std::make_pair(std::get<1>(act.second), std::get<2>(act.second) ) );

        }

        //クッソ分かりづらい上に無駄なコードだけど雰囲気で読んでください
        std::vector<int> move_list_1_vector;
        std::vector<int> move_list_2_vector;

        for(int index = 0; index < 9; ++index){
            if(move_list_1.count(std::make_pair(x_list.at(index), y_list.at(index)) ))
                move_list_1_vector.push_back(index);
            if(move_list_2.count(std::make_pair(x_list.at(index), y_list.at(index)) ))
                move_list_2_vector.push_back(index);
        }

        for(auto move_1 : move_list_1_vector)
            for(auto move_2 : move_list_2_vector){
                can_move_list.emplace_back(move_1 * 9 + move_2);
                try_count.push_back(1);
                win_count.push_back(0);
            }
    };

    addMoveList();

    //ここでマルチスレッド
    for(unsigned int index = 0; index < (can_move_list.size() + cpu_num - 1) / cpu_num;++index){
        unsigned int thread_count = std::min(static_cast<unsigned int>(can_move_list.size() -  index * cpu_num), cpu_num);


        for(unsigned int cpu_cou = 0; cpu_cou < thread_count; ++cpu_cou)
            threads.at(cpu_cou) = std::thread([&](int cpu){std::lock_guard<std::mutex> lock(mtx);win_count.at(index * cpu_num + cpu) = playout(can_move_list.at(index * cpu_num + cpu), cpu);},cpu_cou);
        for(unsigned int cpu_cou = 0; cpu_cou < thread_count; ++cpu_cou)
            threads.at(cpu_cou).join();
    }


    auto ucb = [&](int index){return 1.0 * win_count.at(index) / try_count.at(index) + ucb_val * sqrt(( 2.0 * std::log(try_sum)) / try_count.at(index));};

    auto simulation = [&](int cpu){

        double ucb_max = -1;
        int max_index = 0;
        //最大要素を確認する
        for(unsigned int index = 0; index < can_move_list.size(); ++index){
            double ucb_index = ucb(index);

            if(ucb_max < ucb_index){
                ucb_max = ucb_index;
                max_index = index;
            }
        }

        //ucbの最大値でシミュレーション
        bool win = playout(can_move_list.at(max_index), cpu);

        ++try_count.at(max_index);
        win_count.at(max_index) += win;
    };

    clock_t start_time = clock();

    int count = can_move_list.size() - 1;
    while(++count){

        //毎回の計測はちょっと遅くなるのでダメ
        if(count % 10 == 0){
            clock_t now_time = clock(); //時間計測
            if(try_time < static_cast<double>(now_time - start_time) )
                break;
        }

        for(unsigned int cpu_cou = 0; cpu_cou < cpu_num; ++cpu_cou){
            threads.at(cpu_cou) = std::thread([&](unsigned int cpu){
                std::lock_guard<std::mutex> lock(mtx);
                simulation(cpu);
            },cpu_cou);
        }

        for(unsigned int cpu_cou = 0; cpu_cou < cpu_num; ++cpu_cou)
            threads.at(cpu_cou).join();

    }


    auto max_itr = std::max_element(try_count.begin(), try_count.end());

    int index = std::distance(try_count.begin(), max_itr);

    int max_move = can_move_list.at(index);

    std::cout << "count : " << count * cpu_num << std::endl;
    std::cout << "move : " << max_move << std::endl;

    for(unsigned int index = 0; index < try_count.size(); ++index){
        // std::cout << try_count.at(index) << " ";
        //std::cout <<  win_count.at(index) << " ";
        std::cout << 1.0 * win_count.at(index) /  try_count.at(index) << " ";
    }
    std::cout << std::endl;

    /*
    for(unsigned int index = 0; index < try_count.size(); ++index){
        std::cout << win_count.at(index) << " ";
    }
    std::cout << std::endl;
    */

    std::pair<int,int> agent_1_move = std::make_pair( x_list.at( max_move / 9), y_list.at( max_move / 9) );
    std::pair<int,int> agent_2_move = std::make_pair( x_list.at( max_move % 9), y_list.at( max_move % 9) );

    std::pair<int,int> agent_1_pos = std::make_pair( agent_1_move.first + field.getAgent(side, 0).first, agent_1_move.second + field.getAgent(side, 0).second );
    std::pair<int,int> agent_2_pos = std::make_pair( agent_2_move.first + field.getAgent(side, 1).first, agent_2_move.second + field.getAgent(side, 1).second );

    std::tuple<int,int,int> agent_1_tuple = std::make_tuple( (field.getState(agent_1_pos.first, agent_1_pos.second).first == (side == 0 ? 2 : 1 ) ? 2 : 1 ),
                                                             agent_1_move.first, agent_1_move.second);

    std::tuple<int,int,int> agent_2_tuple = std::make_tuple( (field.getState(agent_2_pos.first, agent_2_pos.second).first == (side == 0 ? 2 : 1 ) ? 2 : 1 ),
                                                             agent_2_move.first, agent_2_move.second);


    return std::make_pair(agent_1_tuple, agent_2_tuple);

}
