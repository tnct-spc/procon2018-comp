#include "geneticalgo.h"

GeneticAlgo::GeneticAlgo(std::shared_ptr<GameManager> manager_ptr, const GeneticAgent& agent) :
    AlgorithmWrapper(manager_ptr),
    agent_data(agent)
{


}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> GeneticAlgo::agentAct(int side)
{

    std::vector<double> status = agent_data.getData();
    double rand = status.at(0); //乱択度合い(高いほど乱択)
    double minus = status.at(1); //マイナスの回避度(高いほど避ける)
    double next_p = status.at(2); //次の手での得点の優先度(高いほど優先)
    double region = status.at(3); //領域ポイントの優先度(高いほど優先)
    double removal = status.at(4); //タイル除去の優先度(高いほど優先)
    double nomove = status.at(5); //移動しなかった時のペナルティ(高いほどペナ大)
    double backmove = status.at(6); //もう塗られてる所に行った時のペナルティ(高いほどペナ大)

    const procon::Field& field = manager->getField();

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::vector<int> can_put_pattern;

    for(int count = 0; count < 81; ++count)
        if(manager->canPut(side, count / 9, count % 9))
            can_put_pattern.emplace_back(count);



    //とりあえずここで評価していく
    auto tile_value = [&](std::pair<int,int> pos){
        double value;
        std::pair<int,int> state = field.getState(pos.first, pos.second);

        if(state.first == (side == 0 ? 2 : 1)){//除去の場合

            if(state.second >= 0)value = state.second * (removal * 2.0);//除去特有の重み付け
            else value = state.second * ( 0.5 / removal );

        }else if(state.first == side + 1){

            //戻った時のペナ
            value = - 32.0 * backmove;
        }else{
            //置かれてない時
            value = state.second;
        }

        if(state.second < 0)
            value += state.second * ( minus * 3.0);//マイナスの時は評価を減らす

        value += state.second * ( next_p * 2.0);//得点に合わせて加算

        return value;
    };

    std::vector<std::pair<double,int>> eval;//value,indexでペアを組む

    for(unsigned int index = 0; index < can_put_pattern.size(); ++index){

        int count = can_put_pattern.at(index);

        std::vector<std::pair<int,int>> pos(2);

        pos.at(0) = field.getAgent(side, 0);
        pos.at(0).first += x_list.at(count / 9);
        pos.at(0).second += y_list.at(count / 9);

        pos.at(1) = field.getAgent(side, 1);
        pos.at(1).first += x_list.at(count % 9);
        pos.at(1).second += y_list.at(count % 9);

        double value_1 = tile_value(pos.at(0));
        double value_2 = tile_value(pos.at(1));

        //動かない場合のペナ
        if(count / 9 == 8)
            value_1 -= 16.0 * (1.0 - nomove) * 2.0;
        if(count % 9 == 8)
            value_2 -= 16.0 * (1.0 - nomove) * 2.0;

        eval.emplace_back(std::make_pair(value_1 + value_2, count ));

    }
    sort(eval.begin(), eval.end(), std::greater<std::pair<double,int>>() );

    //負の値なら選ばない
    for(unsigned int count = 0; count < can_put_pattern.size(); ++count)
        eval.at(count).first = std::max(eval.at(count).first, 0.0);

    //累積和配列
    std::vector<std::pair<double,int>> accum(can_put_pattern.size() + 1, std::make_pair(0, 0) );
    for(unsigned int count = 0; count < can_put_pattern.size(); ++count)
        accum.at(count + 1) = std::make_pair(accum.at(count).first + eval.at(count).first, eval.at(count).second );

    //ここからrandを元にどこまで取るかを決める ここから乱数とlower_bound(i,-1)とかで出す
    std::random_device rnd;
    std::mt19937 mt(rnd());

    std::uniform_real_distribution<> rand_double(0.0, rand * (accum.back().first));

    unsigned int index = std::distance(accum.begin(), std::lower_bound(accum.begin(), accum.end(), std::make_pair(rand_double(mt), -1)) );

    int val = eval.at(index).second;

    std::pair<int,int> agent_1_move = std::make_pair( x_list.at( val / 9), y_list.at( val / 9) );
    std::pair<int,int> agent_2_move = std::make_pair( x_list.at( val % 9), y_list.at( val % 9) );

    std::pair<int,int> agent_1_pos = std::make_pair( agent_1_move.first + field.getAgent(side, 0).first, agent_1_move.second + field.getAgent(side, 0).second );
    std::pair<int,int> agent_2_pos = std::make_pair( agent_2_move.first + field.getAgent(side, 1).first, agent_2_move.second + field.getAgent(side, 1).second );

    std::tuple<int,int,int> agent_1_tuple = std::make_tuple( (field.getState(agent_1_pos.first, agent_1_pos.second).first == (side == 0 ? 2 : 1 ) ? 2 : 1 ),
                                                             agent_1_move.first, agent_1_move.second);

    std::tuple<int,int,int> agent_2_tuple = std::make_tuple( (field.getState(agent_2_pos.first, agent_2_pos.second).first == (side == 0 ? 2 : 1 ) ? 2 : 1 ),
                                                             agent_2_move.first, agent_2_move.second);

    /* デバッグ用出力
    std::cout << " ( " << agent_1_move.first << " , " << agent_1_move.second << " ) ";
    std::cout << " ( " << agent_2_move.first << " , " << agent_2_move.second << " ) " << std::endl;
    */

    return std::make_pair(agent_1_tuple, agent_2_tuple);


}
