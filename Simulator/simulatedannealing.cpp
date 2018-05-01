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

        updateAgent();

        debug_output();


    }


}

void SimulatedAnnealing::updateAgent(){

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
