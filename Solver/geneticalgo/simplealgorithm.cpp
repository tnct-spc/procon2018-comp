#include "simplealgorithm.h"

SimpleAlgorithm::SimpleAlgorithm(std::shared_ptr<GameManager> manager_ptr, const GeneticAgent& agent) :
    AlgorithmWrapper(manager_ptr),
    agent_data(agent)
{

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> SimpleAlgorithm::agentAct(int side){

}

double SimpleAlgorithm::evaluateMove(int side, std::pair<int, int> move){

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    procon::Field& field = manager->getField();

    std::vector<double>& agent_data = agent_data.getData();

    std::vector<int,int> move_vec(2); //ここの実装くそ
    move_vec.at(0) = move.first;
    move_vec.at(1) = move.second;

    std::vector<std::pair<int,int>> now_pos(2); //移動前
    std::vector<std::pair<int,int>> move_pos(2); //移動後

    for(int index = 0; index < 2; ++index){
        now_pos.at(index) = field.getAgent(side, index);
        move_pos.at(index) = now_pos.at(index);
        move_pos.at(index).first += x_list.at(move_vec.at(index));
        move_pos.at(index).second += y_list.at(move_vec.at(index));
    }

}
