#include "testdoubleagentalgo.h"

TestDoubleAgentAlgo::TestDoubleAgentAlgo(std::shared_ptr<GameManager> manager_ptr, const GeneticAgent &agent) :
    AgentWrapper(manager_ptr),
    agent_data(agent)
{

}

const std::tuple<int,int,int> TestDoubleAgentAlgo::agentMove(int side){

    procon::Field& field = manager->getField();
    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    double max_value = -200000;
    std::tuple<int,int,int> max_move = std::make_tuple(0, 0, 0);

    for(int count = 0; count < 9; ++count){
        std::pair<double,bool> value = evaluateMove(side, std::make_pair(x_list.at(count), y_list.at(count)));

        if(max_value < value.first){

            max_value = value.first;
            max_move = std::make_tuple(value.second + 1, x_list.at(count), y_list.at(count));
        }
    }

    return max_move;

}

std::pair<double,bool> TestDoubleAgentAlgo::evaluateMove(int side, std::pair<int, int> move){

    //moveはx,yを相対座標のペアで渡す

    //この関数は(評価値,除去すべきかどうか)をpairで返す
    //相手のタイルがある時は(評価値,true)、何もない時は(評価値,false)を返す
    //pairのsecondが意味を持つのは自分のタイル側に移動する場合のみ

    return std::make_pair(-300000,false);
}
