#include "testagentparam.h"
#include "geneticalgo/geneticagent.h"

TestAgentParam::TestAgentParam()
{
    rand_param_diff = 1.0 / (rand_param_count - 1);

}

void TestAgentParam::run(){

    //テストなので複数アルゴリズムへの対応はしません！やった！
    //両方のエージェントに対して同じアルゴリズムを使うよ

    for(int count = 0; count < rand_agent_count; ++count)
        random_agents.push_back(GeneticAgent(6));

    int agent_count = 1;//std::pow使いたくないです
    for(int count = 0; count < rand_param_count; ++count)
        agent_count *= 6;

    for(int agent_index = 0; agent_index < agent_count; ++agent_index){

        buttle_agents.push_back(GeneticAgent(6, false));
        std::vector<double> agent_data(6);
        int agent_number = agent_index;
        for(int index = 0; index < 6; ++index){

            agent_data.at(index) = std::min(1.0, rand_param_diff * (agent_number % 6));
            agent_number /= 6;
        }
        buttle_agents.back().setData(agent_data);
    }

}
