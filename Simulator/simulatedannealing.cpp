#include "simulatedannealing.h"

SimulatedAnnealing::SimulatedAnnealing(int algo_number) :
    algo_number(algo_number),
    mt(rnd())
{
    cpu_num = std::thread::hardware_concurrency();

    for(int index = 0; index < cpu_num; ++index){
        managers.push_back(new GameManager(12,12,false,60));
    }

    //初期エージェントを選択
    if(algo_number == 0)
        agent = new GeneticAgent(7);

}

void SimulatedAnnealing::run(){


    auto debug_output = [&]{

        // 履歴を残さず、最新のものだけを保存する
        // procon2018-compと同じ階層にbuildディレクトリがある状態でpath書いてます
        std::ofstream output("../../procon2018-comp/Data/GeneticAgent/sa_data");

        std::cout << "{ ";
        output << "{";
        int siz = agent->size;

        const std::vector<double>& data = agent->getData();
        for(int index = 0; index < siz-1; ++index){
            std::cout << data.at(index) << ", ";
            output << data.at(index) << ", ";
        }

        std::cout << data.at(siz - 1) << " }" << std::endl;
        output << data.at(siz - 1) << " }" << std::endl;

        std::cout << std::endl;

        output.close();
    };

    //焼きなましをする
    //評価:乱択相手+遷移前相手で後者の重みを多くする

    for(int count = 0; count < max_try; ++count){

        std::cout << "count " << count + 1 << " started" << std::endl << std::endl;

        updateAgent(count+1);

        debug_output();


    }


}

void SimulatedAnnealing::updateAgent(int now_count){

    GeneticAgent* new_agent;

    if(algo_number==0)
        new_agent = new GeneticAgent(7,false);
    else
        new_agent = new GeneticAgent(7,false);//warning回避のためこの書き方をしているけど、ここが呼ばれる事はありえないです

    std::vector<double> agent_data = agent->getData();

    for(int index = 0; index < new_agent->size; ++index){

        std::uniform_real_distribution<> rand_double(std::max(-max_change_val, -agent_data.at(index) ), std::min(max_change_val, 1 - agent_data.at(index)) );

        agent_data.at(index) += rand_double(mt);
        agent_data.at(index) = std::min(agent_data.at(index), 1.0);
        agent_data.at(index) = std::max(agent_data.at(index), 0.0);
    }

    new_agent->setData(agent_data);

    bool win = buttleAgents((*agent), (*new_agent));

    int old_agent_win = 0;
    int new_agent_win = 0;

    for(int count = 0; count < buttle_rand; ++count){

        GeneticAgent* buttle_agent;
        if(algo_number==0)
            buttle_agent = new GeneticAgent(7,false);
        else
            buttle_agent = new GeneticAgent(7,false);//warning回避のためこの書き方をしているけど、ここが呼ばれる事はありえないです

        old_agent_win += buttleAgents((*agent), (*buttle_agent));
        new_agent_win += buttleAgents((*new_agent), (*buttle_agent));
    }


    int old_val = old_agent_win;
    int new_val = new_agent_win + ( win ? 1 : -1) * buttle_val;

    //二分累乗法で計算している

    auto sq = [](double x){return x*x;};

    std::function<double(double,int)> pow_ = [sq,pow_](double x, int n){//xのn乗
      if(n==0)return 1.0;
      else if(n==1)return x;
      if(n%2==0)return (sq(pow_(x,n/2)));
      else return ((x*sq(pow_(x,n/2))));
    };

    auto temperature = [&]{
        return (1.0 * end_temp +  pow_(const_val, max_try - now_count ) );
    };

    //移動確率
    auto probability = [&]{

        if(new_val >= old_val)
            return 1.0;

        else
            return 1.0 * std::exp(old_val - new_val) / temperature();
    };

    double per = probability();

    std::uniform_real_distribution<> rand_double(0.0,1.0);

    if(rand_double(mt) <= per)
        agent = new_agent;

}


int SimulatedAnnealing::retRandom(int st, int en){
    std::uniform_int_distribution<> rand(st, en);

    return rand(mt);
}


bool SimulatedAnnealing::buttleAgents(GeneticAgent& first, GeneticAgent& second){


    auto buttle = [&](bool flag, int index){//2で割った余りによって反転する(得点が同じ時の処理が順番依存なので一応)
        int turn = retRandom(60, 120);
        std::pair<int,int> size = std::make_pair( retRandom(8,12), retRandom(8, 12) );

        //visualizerは表示しない
        managers.at(index)->resetManager(size.first, size.second, false, turn);

        //firstが勝ったらtrue
        return (flag ? managers.at(index)->simulationGenetic(first, second, algo_number)
                     : ! managers.at(index)->simulationGenetic(second, first, algo_number));
    };



    int win_count = 0;

    std::vector<std::thread> threads(cpu_num);


    for(int index = 0; index < cpu_num; ++index){

        threads.at(index) = std::thread( [=,&win_count]{
            for(int count = 0;count<buttle_count/cpu_num;++count)win_count += buttle(count % 2, index);
        } );

    }
    for(int index = 0; index < cpu_num; ++index){
        threads.at(index).join();
    }

    return win_count * 2 >= (buttle_count / cpu_num) * cpu_num;
}
