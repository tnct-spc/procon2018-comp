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

    agent_count = 1;
    for(int count = 0; count < 6; ++count)
        agent_count *= rand_param_count;

    for(int agent_index = 0; agent_index < agent_count; ++agent_index){

        buttle_agents.push_back(GeneticAgent(6, false));
        std::vector<double> agent_data(6);
        int agent_number = agent_index;
        for(int index = 0; index < 6; ++index){

            agent_data.at(index) = std::min(1.0, rand_param_diff * (agent_number % rand_param_count));
            agent_number /= rand_param_count;
        }
        buttle_agents.back().setData(agent_data);
    }

    win_count.resize(agent_count,0);

    auto buttle = [&](GeneticAgent& agent_1_data, GeneticAgent& agent_2_data){
        //前者が勝ったらtrueを返す

    };

    for(int rand_index = 0; rand_index < rand_agent_count; ++rand_index){

        for(int agent_index = 0; agent_index < agent_count; ++agent_index){

            bool win_flag = buttle(buttle_agents.at(agent_index), random_agents.at(rand_index));
            win_count.at(agent_index) += win_flag;

        }

    }


}
