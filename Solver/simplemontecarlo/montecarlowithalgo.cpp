#include "montecarlowithalgo.h"

MontecarloWithAlgo::MontecarloWithAlgo(std::shared_ptr<GameManager> manager_ptr) :
    AlgorithmWrapper(manager_ptr),
    mt(rnd())
{
    mgr = new GameManager(12, 12, false, 60);

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> MontecarloWithAlgo::agentAct(int side){

    int side_r = (side == 0 ? 1 : 0);//敵側

    const procon::Field& field = manager->getField();

    auto retRandom = [&](int st,int en){ //[st,en]
        std::uniform_int_distribution<> rand(st, en);

        return rand(mt);
    };

    auto playout = [&](int move){

        std::vector<double> val_1 = values.at(retRandom(0,19));
        std::vector<double> val_2 = values.at(retRandom(0,19));

        GeneticAgent* side_1 = new GeneticAgent(7, false);//ここはアルゴリズムによってサイズが変わる
        GeneticAgent* side_2 = new GeneticAgent(7, false);

        side_1->setData(val_1);
        side_2->setData(val_2);


        mgr->resetManager(retRandom(8,12), retRandom(8,12), false, retRandom(60,120));

        mgr->setField(field);

        std::pair<int,int> agent_1_pos = field.getAgent(side, 0);
        std::pair<int,int> agent_2_pos = field.getAgent(side, 1);

        agent_1_pos.first += x_list.at(move / 9);
        agent_1_pos.second += y_list.at(move / 9);

        agent_2_pos.first += x_list.at(move % 9);
        agent_2_pos.second += y_list.at(move % 9);

        mgr->agentAct(side, 0, std::make_tuple( (field.getState(agent_1_pos.first, agent_1_pos.second).first == 2 ? 2 : 1) , x_list.at(move / 9), y_list.at(move / 9)));
        mgr->agentAct(side, 1, std::make_tuple( (field.getState(agent_2_pos.first, agent_2_pos.second).first == 2 ? 2 : 1) , x_list.at(move % 9), y_list.at(move % 9)));

        GeneticAlgo algo(manager,*side_2);
        std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> act = algo.agentAct(1);
        mgr->agentAct(side_r, 0, act.first);
        mgr->agentAct(side_r, 1, act.second);

        mgr->changeTurn();

        bool win = mgr->simulationGenetic(*side_1, *side_2, 0);

        //反転
        if(side == 1)
            win ^= 1;

        ++try_sum;

        return win;
    };

    auto ucb = [&](int index){return 1.0 * win_count.at(index) / try_count.at(index) + ucb_val * sqrt(( 2.0 * std::log(try_sum)) / try_count.at(index));};

    std::vector<int> can_move_list;
    for(int count = 0; count < 81; ++count)
        if(manager->canPut(side, count / 9, count % 9)){

            can_move_list.push_back(count);
            try_count.push_back(1);
            win_count.push_back( playout(count) );
        }


    auto simulation = [&]{

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
        bool win = playout(can_move_list.at(max_index));

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

        simulation();

    }

    auto max_itr = std::max_element(try_count.begin(), try_count.end());

    int index = std::distance(try_count.begin(), max_itr);

    int max_move = can_move_list.at(index);

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
