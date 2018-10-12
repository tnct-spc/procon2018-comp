#include "agentmanager.h"
#include "agents/testdoubleagentalgo.h"
#include "agents/evaluateparam.h"

AgentManager::AgentManager(const procon::Field& field, int turn_max, bool side, int algorithm_number, const GeneticAgent* agent_data_1, const GeneticAgent* agent_data_2) :
    AlgorithmWrapper(field, turn_max, side)
{
    agents.resize(2);

    algo_number = algorithm_number >> 16;

    std::cout << "algo : " << algo_number << std::endl;

    algorithm_number &= (1<<16) - 1;

    if(algorithm_number == 0){
        agents.at(0) = std::make_shared<TestDoubleAgentAlgo>(side, field, final_turn, 0, 6);
        agents.at(1) = std::make_shared<TestDoubleAgentAlgo>(side, field, final_turn, 1, 6);
    }
    else if(algorithm_number == 1){
        agents.at(0) = std::make_shared<EvaluateParam>(side, field, final_turn, 0, 2);
        agents.at(1) = std::make_shared<EvaluateParam>(side, field, final_turn, 1, 2);
    }
    if(agent_data_1 != nullptr)
        setAgentData(*agent_data_1, 0);
    if(agent_data_2 != nullptr)
        setAgentData(*agent_data_2, 1);

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> AgentManager::agentAct(int now_turn){

    if(algo_number == 0)
        return simpleCalc(now_turn);

    else if(algo_number == 1)
        return simpleMC(now_turn);

    else if(algo_number == 2)
        return uniformMC(now_turn);

    else
        return simpleNashAlgo(now_turn);

}

void AgentManager::setParams(std::vector<std::pair<QString, double>> params)
{

}

std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> AgentManager::simpleCalc(int now_turn){

    std::vector<std::pair<double,std::tuple<int,int,int>>> move_1 = agents.at(0)->agentMove(now_turn);
    std::vector<std::pair<double,std::tuple<int,int,int>>> move_2 = agents.at(1)->agentMove(now_turn);

    //2つのvectorは評価値が高い順にソートされているので、それらのうち「コンフリクトが起きないもの」でペアを組み、利得が最も高いものを選ぶ
    //ここの実装なんかめんどそうじゃない？辛い


    std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> max_move = std::make_pair(std::make_tuple(0, 0, 0), std::make_tuple(0, 0, 0));
    double max_value = -400001;

    std::pair<int,int> old_pos_1 = field.getAgent(side, 0);
    std::pair<int,int> old_pos_2 = field.getAgent(side, 1);

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
            if(!( ( std::get<0>(value_1.second) != 1 && old_pos_1 == new_pos_2) || (std::get<0>(value_2.second) != 1 && new_pos_1 == old_pos_2) || new_pos_1 == new_pos_2)){

                max_value = value_1.first + value_2.first;
                max_move = std::make_pair(value_1.second, value_2.second);
            }
        }
    }

    return std::move(max_move);

}

std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> AgentManager::simpleMC(int now_turn){

    SimpleMCForDuble mc(field, side, now_turn, final_turn, agents);

    return mc.calcMove();
}

std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> AgentManager::uniformMC(int now_turn){
    SimpleMCForDuble mc(field, side, now_turn, final_turn, agents);

    return mc.calcMoveUniform();
}

std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> AgentManager::simpleNashAlgo(int now_turn){
    NashEquilibrium nash(field, side, now_turn, final_turn, agents);

    return nash.calcMove();
}

void AgentManager::setAgentData(const GeneticAgent& agent_data, int agent_number){
    agents.at(agent_number)->setAgent(agent_data);
}

void AgentManager::setRandomParams(std::vector<std::pair<QString, double>> params){

}
