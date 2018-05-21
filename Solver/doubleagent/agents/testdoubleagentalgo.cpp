#include "testdoubleagentalgo.h"

TestDoubleAgentAlgo::TestDoubleAgentAlgo(int side, int agent_num, std::shared_ptr<GameManager> manager_ptr, const GeneticAgent &agent_data) :
    AgentWrapper(side, agent_num, manager_ptr),
    agent_data(agent_data)
{

}

const std::tuple<int,int,int> TestDoubleAgentAlgo::agentMove(){

    procon::Field& field = manager->getField();
    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    double max_value = -200000;
    std::tuple<int,int,int> max_move = std::make_tuple(0, 0, 0);

    for(int count = 0; count < 9; ++count){
        std::pair<double,bool> value = evaluateMove(count);

        if(max_value < value.first){

            max_value = value.first;
            max_move = std::make_tuple(value.second + 1, x_list.at(count), y_list.at(count));
        }
    }

    return max_move;

}

std::pair<double,bool> TestDoubleAgentAlgo::evaluateMove(int move){

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    procon::Field field = manager->getField();

    if(manager->canPut(side, agent, move, false))
        return std::make_pair(-300000, false);


    //この関数は(評価値,除去すべきかどうか)をpairで返す
    //相手のタイルがある時は(評価値,true)、何もない時は(評価値,false)を返す
    //pairのsecondが意味を持つのは自分のタイル側に移動する場合のみ

    std::vector<double>& data = agent_data.getData();

    double per_minus_move = -1.0 * data.at(0) * 100 + 20;

    double per_plus_move =  data.at(1) * 100 - 20;

    double const_back_move = -1.0 * data.at(2) * 100 + 20;

    double const_no_move = -1.0 * data.at(3) * 100 + 20;

    double per_delete_move = data.at(4) * 100 - 20;

    double per_region = data.at(5) * 100 - 20;

    double per_point = data.at(6) * 100;

    double evaluate_val = 0.0;

    std::pair<int,int> old_pos = field.getAgent(side, agent);

    std::pair<int,int> new_pos = old_pos;
    new_pos.first += x_list.at(move);
    new_pos.second += y_list.at(move);

    int tile_value = field.getState(new_pos.first, new_pos.second).second;
    int tile_color = field.getState(new_pos.first, new_pos.second).first;



    return std::make_pair(-300000,false);
}
