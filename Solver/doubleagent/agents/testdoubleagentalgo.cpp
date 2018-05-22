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

    //枝切り
    if(manager->canPut(side, agent, move, false))
        return std::make_pair(-300000, false);


    //この関数は(評価値,除去すべきかどうか)をpairで返す
    //相手のタイルがある時は(評価値,true)、何もない時は(評価値,false)を返す
    //pairのsecondが意味を持つのは自分のタイル側に移動する場合のみ


    std::vector<double>& data = agent_data.getData();

    double const_back_move = -1.0 * data.at(0) * 100 + 5;

    double const_no_move = -1.0 * data.at(1) * 100 + 5;

    double per_delete_move = data.at(2) * 100 - 20;

    double per_region = data.at(3) * 100 - 20;

    double per_point = data.at(4) * 100;

    double evaluate_val = 0.0;
    bool is_delete = false;

    //移動後の位置
    std::pair<int,int> new_pos = field.getAgent(side, agent);
    new_pos.first += x_list.at(move);
    new_pos.second += y_list.at(move);

    //移動先のタイル状況
    std::pair<int,int> tile_data = field.getState(new_pos.first, new_pos.second);
    int tile_value = tile_data.second;
    int tile_color = tile_data.first;

    auto calc = [&](bool delete_move){

        double return_value = 0.0;

        //移動しないなら
        if(move == 8)
            return_value += const_no_move;
        //自陣に移動するなら
        if(tile_value == side + 1 && !delete_move){
            return_value += const_back_move;

            //自陣に移動する時は評価をここで打ち切る(現時点の実装ではこうする)
            return return_value;
        }

        //得点の変動値
        int pos_value = tile_value;
        if(delete_move)pos_value *= -1;

        return_value += pos_value * per_point;

        if(delete_move)
            return_value += pos_value * per_delete_move;

        //ここに領域ポイントの処理を追加する(未実装ですが！！！！)

        return return_value;
    };

    //自分の色で塗られている場合
    if(tile_color == side + 1){
        double not_delete_val = calc(false);
        double delete_val = calc(true);

        if(not_delete_val < delete_val)
            is_delete = true;
        evaluate_val = std::max(not_delete_val, delete_val);
    }
    //相手の色で塗られている場合
    else if(tile_color){
        is_delete = true;
        evaluate_val = calc(true);
    }
    //塗られていない場合
    else evaluate_val = calc(false);

    return std::make_pair(evaluate_val, is_delete);

}
