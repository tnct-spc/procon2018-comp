#include "montecarlowithalgo.h"

MontecarloWithAlgo::MontecarloWithAlgo(std::shared_ptr<GameManager> manager_ptr) :
    AlgorithmWrapper(manager_ptr),
    mt(rnd())
{
    mgr = new GameManager(12, 12, false, 60);

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> MontecarloWithAlgo::agentAct(int side){

    const procon::Field& field = manager->getField();

    win_count.resize(81, 0);
    try_count.resize(81, 0);

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

    };

}
