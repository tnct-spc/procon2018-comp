#include "majorityrulewithabstdata.h"

MajorityRuleWithAbstData::MajorityRuleWithAbstData(const procon::Field& field, int final_turn, bool side) :
    AlgorithmWrapper(field, final_turn, side)
{
    for(auto param : params){
        GeneticAgent agent(10);
        agent.setData(param);
        agents.emplace_back(std::move(agent));
    }
}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> MajorityRuleWithAbstData::agentAct(int now_turn){
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::map<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>, int> use;
    for(auto agent : agents){
        UseAbstractData algo(field, final_turn, side, agent);
        ++use[algo.agentAct(now_turn)];
    }
    int maxel = 0;
    std::vector<std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>> elements;
    for(auto el : use){
        if(el.second > maxel){
            elements.clear();
            maxel = el.second;
        }
        if(el.second == maxel)
            elements.emplace_back(el.first);
    }
    std::uniform_int_distribution<> rand(0, elements.size() - 1);
    return elements.at(rand(mt));
}

void MajorityRuleWithAbstData::setParams(std::vector<std::pair<QString, double>> params)
{

}

void MajorityRuleWithAbstData::setRandomParams(std::vector<std::pair<QString, double>> params,bool flag){

}
