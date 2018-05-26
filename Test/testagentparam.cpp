#include "testagentparam.h"
#include "geneticalgo/geneticagent.h"

TestAgentParam::TestAgentParam()
{
    cpu_num = std::thread::hardware_concurrency();
    rand_param_diff = 1.0 / (rand_param_count - 1);

    for(int index = 0; index < cpu_num; ++index){
        managers.push_back(new GameManager(12,12,false,60));
    }

}

void TestAgentParam::run(){

    //テストなので複数アルゴリズムへの対応はしません！やった！
    //両方のエージェントに対して同じアルゴリズムを使うよ


    int agent_count_sum = 1;
    for(int count = 0; count < 6; ++count)
        agent_count_sum *= rand_param_count;

    for(int agent_index = 0; agent_index < agent_count_sum; ++agent_index){

        int param_sum = 0;
        std::vector<double> agent_data(6);
        int agent_number = agent_index;
        for(int index = 0; index < 6; ++index){

            agent_data.at(index) = std::min(1.0, rand_param_diff * (agent_number % rand_param_count));
            param_sum += (agent_number % rand_param_count);
            agent_number /= rand_param_count;

            if(param_sum > rand_param_count - 1)
                break;
        }

        if(param_sum == rand_param_count - 1){
            buttle_agents.push_back(GeneticAgent(6, false));
            buttle_agents.back().setData(agent_data);
        }
    }

    agent_count = buttle_agents.size();
    std::cout << agent_count << std::endl;

    win_count.resize(agent_count,0);
    try_count.resize(agent_count,0);

    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<> rand_turn(60, 120);
    std::uniform_int_distribution<> rand_size(8, 12);

    auto buttle = [&](GeneticAgent& agent_1_data, GeneticAgent& agent_2_data, int index){
        //前者が勝ったらtrueを返す

        int turn = rand_turn(mt);
        std::pair<int,int> size = std::make_pair( rand_size(mt), rand_size(mt));

        managers.at(index)->resetManager(size.first, size.second, false, turn);

        int win_num = managers.at(index)->simulationGenetic(agent_1_data, agent_2_data, 3);

        if(win_num == -1)return 0;

        //引き分けなら0,勝ちなら正で負けなら負
        return (win_num ? 1 : -1);
    };

    std::vector<std::thread> threads(cpu_num);

    std::vector<std::pair<int,int>> agent_buttle_cpu(cpu_num);//[開始地点,終了地点)で戦闘に対応するエージェントを記録しておく

    agent_buttle_cpu.at(0) = std::make_pair(0, agent_count / cpu_num + (agent_count % cpu_num > 0));
    for(int cpu = 1; cpu < cpu_num; ++cpu)
        agent_buttle_cpu.at(cpu) = std::make_pair(agent_buttle_cpu.at(cpu - 1).second, agent_buttle_cpu.at(cpu - 1).second + agent_count / cpu_num + (agent_count % cpu_num > cpu));

    for(int rand_index = 0; rand_index < rand_agent_count; ++rand_index){

        std::cout << "rand_agent_number : " << rand_index + 1 << std::endl;

        //乱択で対戦相手を選択
        GeneticAgent rand_agent_data(6, 2);

        for(int cpu = 0; cpu < cpu_num; ++cpu)
            threads.at(cpu) = std::thread([&](int cpu_index){
                std::lock_guard<std::mutex> lock(mtx);

                for(int agent_index = agent_buttle_cpu.at(cpu_index).first; agent_index < agent_buttle_cpu.at(cpu_index).second; ++agent_index){
                    for(int count = 0; count < buttle_count; ++count){

                        int win_flag = buttle(buttle_agents.at(agent_index), rand_agent_data, cpu_index);

                        if(win_flag != 0)
                            ++try_count.at(agent_index);
                        if(win_flag == 1)
                            ++win_count.at(agent_index);

                    }
                }

            }, cpu);

        for(int cpu = 0; cpu < cpu_num; ++cpu)
            threads.at(cpu).join();

        /*
        for(int agent_index = 0; agent_index < agent_count; ++agent_index){

            //ここ変える(連続で勝負させる マルチスレッド)
            for(int count = 0; count < (buttle_count + cpu_num - 1) / cpu_num; ++count){

                for(int cpu = 0; cpu < cpu_num; ++cpu)
                    threads.at(cpu) = std::thread([&](int cpu_index, int index){
                        for(int count = 0; count < (buttle_count + cpu_num - 1) / cpu_num; ++count)

                        std::lock_guard<std::mutex> lock(mtx);

                        int win_flag = buttle(buttle_agents.at(agent_index), rand_agent_data, cpu_index);

                        if(win_flag != 0)
                            ++try_count.at(index);
                        if(win_flag == 1)
                            ++win_count.at(index);

                    }, cpu, agent_index);

                for(int cpu = 0; cpu < cpu_num; ++cpu)
                    threads.at(cpu).join();
            }

        }
        */

        std::ofstream output("../../procon2018-comp/Data/TestAgentParam/learning_data");

        for(int index = 0; index < agent_count; ++index){
            const std::vector<double>& data = buttle_agents.at(index).getData();

            for(int count = 0; count < 6; ++count)
                output << data.at(count) << " , ";

            output << win_count.at(index) << " , " << try_count.at(index) << " , " << 1.0 * win_count.at(index) / try_count.at(index) << std::endl;
            std::cout << index << "  :  " << win_count.at(index) << " / " << try_count.at(index) << "   :   " << 1.0 * win_count.at(index) / try_count.at(index) << std::endl;
        }
        std::cout << std::endl;
        output.close();
    }


}
