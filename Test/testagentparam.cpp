#include "testagentparam.h"
#include "geneticalgo/geneticagent.h"

TestAgentParam::TestAgentParam() :
    mt(rnd())
{
    cpu_num = std::thread::hardware_concurrency();
    rand_param_diff = 1.0 / (rand_param_count - 1);

    rand_turn = std::uniform_int_distribution<>(60, 120);
    rand_size = std::uniform_int_distribution<>(8, 12);

    threads.resize(cpu_num);

    for(int index = 0; index < cpu_num; ++index){
        managers.push_back(new GameManager(12,12,false,60));
    }

}

void TestAgentParam::runFix(){

    //テストなので複数アルゴリズムへの対応はしません！やった！
    //両方のエージェントに対して同じアルゴリズムを使うよ

    std::vector<GeneticAgent> buttle_agents;

    std::vector<int> win_count;
    std::vector<int> try_count;

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
    std::cout << std::endl << "started   agent size : " << agent_count << std::endl << std::endl;

    win_count.resize(agent_count,0);
    try_count.resize(agent_count,0);



    std::vector<std::pair<int,int>> agent_buttle_cpu(cpu_num);//[開始地点,終了地点)で戦闘に対応するエージェントを記録しておく

    agent_buttle_cpu.at(0) = std::make_pair(0, agent_count / cpu_num + (agent_count % cpu_num > 0));
    for(int cpu = 1; cpu < cpu_num; ++cpu)
        agent_buttle_cpu.at(cpu) = std::make_pair(agent_buttle_cpu.at(cpu - 1).second, agent_buttle_cpu.at(cpu - 1).second + agent_count / cpu_num + (agent_count % cpu_num > cpu));

    for(int rand_index = 0; rand_index < rand_agent_count; ++rand_index){

        std::cout << "count : " << rand_index + 1 << std::endl;

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


        std::ofstream output("../../procon2018-comp/Data/TestAgentParam/learning_data_fix");

        double min_per = 1.0;
        double max_per = 0.0;

        for(int index = 0; index < agent_count; ++index){

            const std::vector<double>& data = buttle_agents.at(index).getData();

            if(try_count.at(index)){
                min_per = std::min(min_per, 1.0 * win_count.at(index) / try_count.at(index));
                max_per = std::max(max_per, 1.0 * win_count.at(index) / try_count.at(index));
            }

            for(int count = 0; count < 6; ++count)
                output << data.at(count) << " , ";

            output << win_count.at(index) << " , " << try_count.at(index) << " , " << 1.0 * win_count.at(index) / try_count.at(index) << std::endl;
        }
        output.close();

        std::cout << "average value  :  " << std::accumulate(win_count.begin(), win_count.end(), 0.0) / std::accumulate(try_count.begin(), try_count.end(), 0.0) << std::endl;
        std::cout << "min , max  :  " << min_per << " , " << max_per << std::endl;
        std::cout << std::endl;
    }


}

void TestAgentParam::runRand(){

    std::ofstream output("../../procon2018-comp/Data/TestAgentParam/learning_data_rand" , std::ios_base::app);

    int sum_win = 0;
    int sum_try = 0;

    //ここでほとんどの処理を行う
    auto agent_param_check = [&]{
        GeneticAgent agent(6, 2);

        std::vector<std::pair<int,int>> agent_buttle_cpu(cpu_num);
        agent_buttle_cpu.at(0) = std::make_pair(0, one_agent_buttle_count / cpu_num + (one_agent_buttle_count % cpu_num > 0));
        for(int cpu = 1; cpu < cpu_num; ++cpu)
            agent_buttle_cpu.at(cpu) = std::make_pair(agent_buttle_cpu.at(cpu - 1).second, agent_buttle_cpu.at(cpu - 1).second + one_agent_buttle_count / cpu_num + (one_agent_buttle_count % cpu_num > cpu));

        int try_count = 0;
        int win_count = 0;

        for(int cpu = 0; cpu < cpu_num; ++cpu)
            threads.at(cpu) = std::thread([&](int cpu_index){
                std::lock_guard<std::mutex> lock(mtx);

                for(int agent_index = agent_buttle_cpu.at(cpu_index).first; agent_index < agent_buttle_cpu.at(cpu_index).second; ++agent_index){

                    //対戦相手の生成
                    GeneticAgent rand_agent_data(6, 2);

                    for(int count = 0; count < buttle_count; ++count){

                        int win_flag = buttle(agent, rand_agent_data, cpu_index);

                        if(win_flag != 0)
                            ++try_count;
                        if(win_flag == 1)
                            ++win_count;
                    }
                }

            }, cpu);

        for(int cpu = 0; cpu < cpu_num; ++cpu)
            threads.at(cpu).join();


        sum_win += win_count;
        sum_try += try_count;

        const std::vector<double>& data = agent.getData();

        for(int count = 0; count < 6; ++count)
            output << data.at(count) << " , ";

        output << win_count << " , " << try_count << " , " << 1.0 * win_count / try_count << std::endl;


        std::cout << "{ ";
        for(int count = 0; count < 5; ++count)
            std::cout << data.at(count) << " , ";

        std::cout << data.at(5) << " }  :  ";

        std::cout << win_count << " / " << try_count << "  :  " << 1.0 * win_count / try_count << std::endl;

        std::cout << "average value  :  " << 1.0 * sum_win / sum_try << std::endl;

    };


    for(int count = 0; count < rand_agent_count; ++count){
        std::cout << std::endl;
        std::cout << "count : " << count + 1 << std::endl;
        agent_param_check();
    }

    output.close();

}

int TestAgentParam::buttle(GeneticAgent& agent_1_data, GeneticAgent& agent_2_data, int index){

    int turn = rand_turn(mt);
    std::pair<int,int> size = std::make_pair( rand_size(mt), rand_size(mt));

    managers.at(index)->resetManager(size.first, size.second, false, turn);

    int win_num = (index % 2 ? managers.at(index)->simulationGenetic(agent_1_data, agent_2_data, 3) : managers.at(index)->simulationGenetic(agent_2_data, agent_1_data, 3));

    if(win_num == -1)return 0;

    if(!(index % 2))
        return (win_num ? -1 : 1);

    //引き分けなら0,勝ちなら正で負けなら負
    return (win_num ? 1 : -1);
}
