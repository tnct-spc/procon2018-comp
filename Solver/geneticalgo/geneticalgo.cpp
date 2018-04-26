#include "geneticalgo.h"

GeneticAlgo::GeneticAlgo(std::shared_ptr<GameManager> manager_ptr) :
    AlgorithmWrapper(manager_ptr)
{

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> GeneticAlgo::agentAct(int side)
{
    //ここは規定のパラメータに合わせてえいえい
    /*
    double rand = 0.5; //乱択度合い(高いほど乱択)
    double minus = 0.5; //マイナスの回避度(高いほど避ける)
    double next_p = 0.5; //次の手での得点の優先度(高いほど優先)
    double region = 0.5; //領域ポイントの優先度(高いほど優先)
    double removal = 0.5; //タイル除去の優先度(高いほど優先)
    */
    const procon::Field& field = manager->getField();

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::vector<int> can_put_pattern;

    for(int count = 0; count < 81; ++count)
        if(manager->canPut(side, count / 9, count % 9))
            can_put_pattern.emplace_back(count);

    std::vector<double> eval(can_put_pattern.size(), 0);


    //とりあえずここで評価していく
    auto tile_value = [&](std::pair<int,int> pos){
        double value;
        std::pair<int,int> state = field.getState(pos.first, pos.second);

        if(state.first == (side == 0 ? 2 : 1)){//除去の場合

            if(state.second >= 0)value = state.second * (agent_data.removal * 2.0);//除去特有の重み付け
            else value = state.second * ( 0.5 / agent_data.removal );

        }else{

            value = state.second;
        }

        if(state.second < 0)
            value += state.second * ( agent_data.minus * 3.0);//マイナスの時は評価を減らす

        value += state.second * ( agent_data.next_p * 2.0);//得点に合わせて加算

        return value;
    };

    for(unsigned int index = 0; index < can_put_pattern.size(); ++index){

        int count = can_put_pattern.at(index);

        std::vector<std::pair<int,int>> pos(2);

        pos.at(0) = field.getAgent(side, 0);
        pos.at(0).first += x_list.at(count / 9);
        pos.at(0).second += y_list.at(count / 9);

        pos.at(1) = field.getAgent(side, 1);
        pos.at(1).first += x_list.at(count % 9);
        pos.at(1).second += y_list.at(count % 9);

        double value1 = tile_value(pos.at(0));
        double value_2 = tile_value(pos.at(1));

    }


}

void GeneticAlgo::setAgentData(GeneticAgent inp){
    agent_data = inp;
}
