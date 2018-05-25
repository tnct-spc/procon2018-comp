#include "testdoubleagentalgo.h"

TestDoubleAgentAlgo::TestDoubleAgentAlgo(int side, int agent_num, std::shared_ptr<GameManager> manager_ptr, const GeneticAgent &agent_data) :
    AgentWrapper(side, agent_num, manager_ptr, agent_data)
{
    if(this->agent_data.size != 6)
        this->agent_data = GeneticAgent(6);
}

const std::vector<std::pair<double, std::tuple<int,int,int>>> TestDoubleAgentAlgo::agentMove(){

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::vector<std::pair<double,std::tuple<int,int,int>>> return_val;

    return_val.push_back(std::make_pair(-200000, std::make_tuple(0, 0, 0)));


    for(int count = 0; count < 9; ++count){
        std::pair<double,bool> value = evaluateMove(count);

        if(value.first > -200000)//置けないパターンがあるのでそれを着る
            return_val.push_back(std::make_pair(value.first, std::make_tuple(value.second + 1, x_list.at(count), y_list.at(count))));
    }
    //昇順ソート
    sort(return_val.begin(), return_val.end(), std::greater<std::pair<double,std::tuple<int,int,int>>>());

    return return_val;

}

std::pair<double,bool> TestDoubleAgentAlgo::evaluateMove(int move){

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    procon::Field field = manager->getField();

    //枝切り
    if(!manager->canPut(side, agent, move, false))
        return std::make_pair(-300000, false);


    //この関数は(評価値,除去すべきかどうか)をpairで返す
    //相手のタイルがある時は(評価値,true)、何もない時は(評価値,false)を返す
    //pairのsecondが意味を持つのは自分のタイル側に移動する場合のみ


    const std::vector<double>& data = agent_data.getData();

    //パラメータの期待値が0.16程度

    //定数 [-950,50]
    double const_back_move = -1.0 * data.at(0) * 1000 + 10;

    //定数 [-950,50] これはconst_back_moveと重複する
    double const_no_move = -1.0 * data.at(1) * 1000 + 10;

    //per_delete_move * -1 * 削除したマスの得点 になる
    //[-300, 980](有利な除去なら)
    double per_delete_move = data.at(2) * 80 - 6;

    //per_region * 囲ったマスの得点合計 になる
    double per_region = data.at(3) * 100 - 2;

    //これは「タイルの得点」を元に計算する
    //per_point * タイル除去による(領域以外の)得点の変動値 になる
    //[-1600, 1600]
    double per_point = data.at(4) * 100;

    //これは「得点の変動値の合計」を元に計算する
    //per_point_sum * 行動をした後の得点の変動値 になる
    //領域なしで[-640,640]、領域を含むと2000点程度が加算されたりする
    double per_point_sum = data.at(5) * 40;

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

        if(tile_color == side + 1 && delete_move == true && tile_value >= 0){
            //これはどのように考えても有効な手にならない
            return -500000.0;
        }

        double return_value = 0.0;

        //移動しないなら
        if(move == 8)
            return_value += const_no_move;
        //自陣に移動するなら
        if(tile_color == side + 1 && delete_move == false){
            return_value += const_back_move;

            //自陣に移動する時は評価をここで打ち切る(現時点の実装ではこうする)
            return return_value;
        }

        //得点の変動値
        int pos_value = tile_value;
        //自分のタイルを除去する場合
        if(delete_move && (tile_color == side + 1))pos_value *= -1;

        return_value += pos_value * per_point;


        if(delete_move)
            return_value += pos_value * per_delete_move;

        //ここに領域ポイントの処理を追加する(未実装ですが！！！！)
        int point_diff = pos_value;//実際はここに + region_valueがされる

        return_value += point_diff * per_point_sum;

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
