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

    std::vector<int> can_put_pattern;

    for(int count = 0; count < 81; ++count)
        if(manager->canPut(side, count / 9, count % 9))
            can_put_pattern.emplace_back(count);

    std::vector<double> eval(can_put_pattern.size(), 0);

    for(int index = 0; index < can_put_pattern.size(); ++index){

    }


}

void GeneticAlgo::setAgentData(GeneticAgent inp){
    agent_data = inp;
}
