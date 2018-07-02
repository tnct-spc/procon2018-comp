#include "testevaluatespeed.h"


TestEvaluateSpeed::TestEvaluateSpeed()
{
    mt = std::mt19937(rnd());
}

void TestEvaluateSpeed::run(){

    auto rand = [&](int max){
        std::uniform_int_distribution<> dist(0, max);
        return dist(mt);
    };

    std::vector<std::shared_ptr<GameManager>> manager_vec;
    std::vector<std::shared_ptr<AgentManager>> algo_vec;

    for(int count = 0; count < calc_count; ++count){

        // 盤面の自動生成
        procon::Field field(8 + rand(4), 8 + rand(4), 16, -16);

        std::pair<int,int> size = field.getSize();

        for(int x = 0; x < size.first; ++x)
        for(int y = 0; y < size.second; ++y)
            field.setState(x, y, rand(2));

        std::set<std::pair<int,int>> agent_pos;

        for(int side = 0; side < 2; ++side)
        for(int agent = 0; agent < 2; ++agent){

            std::pair<int,int> new_pos = std::make_pair(rand(field.getSize().first - 1), rand(field.getSize().second - 1));
            while(agent_pos.count(new_pos))
            new_pos = std::make_pair(rand(field.getSize().first - 1), rand(field.getSize().second - 1));

            field.setAgent(side, agent, new_pos.first, new_pos.second);
            field.setState(new_pos.first, new_pos.second, side + 1);
            agent_pos.insert(new_pos);
        }

        std::shared_ptr<GameManager> manager = std::make_shared<GameManager>(field.getSize().first, field.getSize().second, false);
        manager_vec.push_back(manager);
        algo_vec.push_back(std::make_shared<AgentManager>(field, rand(120), 0));
    }


    clock_t start = clock();
    for(int count = 0; count < calc_count; ++count)
        algo_vec.at(count)->agentAct(0);

    clock_t end = clock();

    double time = (double)(end - start) / CLOCKS_PER_SEC;

    std::cout << time << " / " << calc_count << "  =  " << time / calc_count << std::endl;

    std::exit(0);

}
