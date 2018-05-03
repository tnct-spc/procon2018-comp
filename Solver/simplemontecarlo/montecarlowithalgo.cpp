#include "montecarlowithalgo.h"

MontecarloWithAlgo::MontecarloWithAlgo(std::shared_ptr<GameManager> manager_ptr) :
    AlgorithmWrapper(manager_ptr),
    mt(rnd())
{
    mgr = new GameManager(12, 12, false, 60);

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> MontecarloWithAlgo::agentAct(int side){

    const procon::Field& field = manager->getField();

    auto retRandom = [&](int st,int en){ //[st,en]
        std::uniform_int_distribution<> rand(st, en);

        return rand(mt);
    };

    auto playout = [&](int move){

        std::vector<double> val_1 = values.at(retRandom(0,19));
        std::vector<double> val_2 = values.at(retRandom(0,19));

        GeneticAgent* side_1 = new GeneticAgent(7);//ここはアルゴリズムによってサイズが変わる
        GeneticAgent* side_2 = new GeneticAgent(7);


        mgr->resetManager(retRandom(8,12), retRandom(8,12), false, retRandom(60,120));

        mgr->setField(field);

        std::pair<int,int> agent_1_pos = field.getAgent(0, 0);
        std::pair<int,int> agent_2_pos = field.getAgent(0, 1);

        agent_1_pos.first += x_list.at(move / 9);
        agent_1_pos.second += y_list.at(move / 9);

        agent_2_pos.first += x_list.at(move % 9);
        agent_2_pos.second += y_list.at(move % 9);

        mgr->agentAct(0, 0, std::make_tuple( (field.getState(agent_1_pos.first, agent_1_pos.second).first == 2 ? 2 : 1) , x_list.at(move / 9), y_list.at(move / 9)));
        mgr->agentAct(0, 1, std::make_tuple( (field.getState(agent_2_pos.first, agent_2_pos.second).first == 2 ? 2 : 1) , x_list.at(move % 9), y_list.at(move % 9)));

        GeneticAlgo algo(manager,*side_2);
        std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> act = algo.agentAct(1);
        mgr->agentAct(1, 0, act.first);
        mgr->agentAct(1, 1, act.second);

        mgr->changeTurn();

        bool win = mgr->simulationGenetic(*side_1, *side_2, 0);

        ++try_sum;

        return win;
    };

    auto ucb = [&](int index){return 1.0 * win_count.at(index) / try_count.at(index) + ucb_val * sqrt(( 2.0 * std::log(try_sum)) / try_count.at(index));};

    std::vector<int> can_move_list;
    for(int count = 0; count < 81; ++count)
        if(manager->canPut(0, count / 9, count % 9)){

            can_move_list.push_back(count);
            try_count.push_back(1);
            win_count.push_back( playout(count) );
        }


    auto simulation = [&]{

        double ucb_max = -1;
        int max_index = 0;
        //最大要素を確認する
        for(unsigned int index = 0; index < can_move_list.size(); ++index){
            double ucb_index = ucb(can_move_list.at(index) );

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

    }

}
