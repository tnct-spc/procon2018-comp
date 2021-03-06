#include "testdoubleagentalgo.h"

TestDoubleAgentAlgo::TestDoubleAgentAlgo(int side, const procon::Field& field, int final_turn, int agent_num, const GeneticAgent &agent_data) :
    AgentWrapper(side, field, final_turn, agent_num, 6, agent_data)
{

}

double TestDoubleAgentAlgo::evaluateMove(int move, bool is_delete, int, int eval_side){

    if(eval_side == -1)
        eval_side = side;

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    //ここのコストヤバいけど
    procon::Field copy_field = field;

    //枝切り
    if(!field.canPut(eval_side, agent, move, false))
        return -300000;


    //この関数は(評価値,除去すべきかどうか)をpairで返す
    //相手のタイルがある時は(評価値,true)、何もない時は(評価値,false)を返す
    //pairのsecondが意味を持つのは自分のタイル側に移動する場合のみ


    const std::vector<double>& data = agent_data.getData();

    //パラメータの期待値が0.16程度

    //定数
    double const_back_move = -1.0 * data.at(0) * 1000;

    //定数 これはconst_back_moveと重複する
    double const_no_move = -1.0 * data.at(1) * 1000;

    //per_delete_move * 削除したマスの得点 になる
    double per_delete_move = data.at(2) * 120;

    //per_region * 囲ったマスの得点合計 になる
    double per_region = data.at(3) * 80;

    //これは「タイルの得点」を元に計算する
    //per_point * タイル除去による(領域以外の)得点の変動値 になる

    double per_point = data.at(4) * 100;

    //これは「得点の変動値の合計」を元に計算する
    //per_point_sum * 行動をした後の得点の変動値 になる


    double per_point_sum = data.at(5) * 40;

    //移動後の位置
    std::pair<int,int> new_pos = copy_field.getAgent(eval_side, agent);
    new_pos.first += x_list.at(move);
    new_pos.second += y_list.at(move);

    //移動先のタイル状況
    std::pair<int,int> tile_data = copy_field.getState(new_pos.first, new_pos.second);
    int tile_value = tile_data.second;
    int tile_color = tile_data.first;

    auto calc = [&](bool delete_move){

        if(tile_color == eval_side + 1 && delete_move == true && tile_value >= 0){
            //これはどのように考えても有効な手にならない
            return -500000.0;
        }

        double return_value = 0.0;

        //移動しないなら
        if(move == 8)
            return_value += const_no_move;
        //自陣に移動するなら
        if(tile_color == eval_side + 1 && delete_move == false){
            return_value += const_back_move;

            //自陣に移動する時は評価をここで打ち切る(現時点の実装ではこうする)
            return return_value;
        }

        std::vector<std::pair<int,int>> before_point(2);

        before_point.at(0)= copy_field.getPoints(false).at(side);
        before_point.at(1) = copy_field.getPoints(false).at(!side);

        //仮に移動させてしまう
        copy_field.setState(new_pos.first, new_pos.second, (delete_move ? 0 : eval_side + 1) );


        //得点の変動値
        int pos_value = tile_value;
        //自分のタイルを除去する場合
        if(delete_move && (tile_color == eval_side + 1))pos_value *= -1;

        return_value += pos_value * per_point;


        if(delete_move)
            return_value += pos_value * per_delete_move;


        //ここの計算し直しが非効率的
        if(is_update)
            copy_field.updatePoint();



        if(is_update){

            std::vector<std::pair<int,int>> after_point(2);

            after_point.at(0)= copy_field.getPoints(false).at(side);
            after_point.at(1) = copy_field.getPoints(false).at(!side);

            //領域ポイントの変化量
            int region_diff = (after_point.at(0).second - before_point.at(0).second) - (after_point.at(1).second - before_point.at(1).second);

            return_value += region_diff * per_region;


        //(自分の変化量 - 相手の変化量)
            int point_diff = ((after_point.at(0).first + after_point.at(0).second) - (before_point.at(0).first + before_point.at(0).second))
                   - ((after_point.at(1).first + after_point.at(1).second) - (before_point.at(1).first + before_point.at(1).second));

            return_value += point_diff * per_point_sum;
        }

        return return_value;
    };


    return calc(is_delete);

}
