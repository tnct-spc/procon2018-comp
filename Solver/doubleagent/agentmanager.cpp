#include "agentmanager.h"
#include "agents/testdoubleagentalgo.h"

AgentManager::AgentManager(std::shared_ptr<GameManager> manager_ptr, int side, int algorithm_number) :
    AlgorithmWrapper(manager_ptr)
{

    agents.resize(2);

    std::shared_ptr<TestDoubleAgentAlgo> hoge;
    hoge = std::make_shared<TestDoubleAgentAlgo>(side, 0, manager_ptr);

    if(algorithm_number == 0){
        agents.at(0) = std::make_shared<TestDoubleAgentAlgo>(side, 0, getManagerPtr());
        agents.at(1) = std::make_shared<TestDoubleAgentAlgo>(side, 1, getManagerPtr());
    }

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> AgentManager::agentAct(int side){

    std::vector<std::pair<double,std::tuple<int,int,int>>> move_1 = agents.at(0)->agentMove();
    std::vector<std::pair<double,std::tuple<int,int,int>>> move_2 = agents.at(1)->agentMove();

    //2つのvectorは評価値が高い順にソートされているので、それらのうち「コンフリクトが起きないもの」でペアを組み、利得が最も高いものを選ぶ
    //ここの実装なんかめんどそうじゃない？辛い

    std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> max_move = std::make_pair(std::make_tuple(0, 0, 0), std::make_tuple(0, 0, 0));
    double max_value = -400001;

    std::pair<int,int> old_pos_1 = manager->getField().getAgent(side, 0);
    std::pair<int,int> old_pos_2 = manager->getField().getAgent(side, 1);

    for(auto value_1 : move_1){

        std::pair<int,int> new_pos_1 = old_pos_1;
        new_pos_1.first += std::get<1>(value_1.second);
        new_pos_1.second += std::get<2>(value_1.second);

        for(auto value_2 : move_2){

            //選ばれる事はありえないため
            if(value_1.first + value_2.first <= max_value)
                break;

            std::pair<int,int> new_pos_2 = old_pos_2;
            new_pos_2.first += std::get<1>(value_2.second);
            new_pos_2.second += std::get<2>(value_2.second);

            //移動するのならold_posを参照する必要はない
            if(!( ( std::get<0>(value_1.second) != 1 && old_pos_1 == new_pos_2) || (std::get<1>(value_2.second) != 1 && new_pos_1 == old_pos_2) || new_pos_1 == new_pos_2)){

                max_value = value_1.first + value_2.first;
                max_move = std::make_pair(value_1.second, value_2.second);
            }
        }
    }

    return max_move;

}
