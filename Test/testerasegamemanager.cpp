#include "testerasegamemanager.h"

TestEraseGameManager::TestEraseGameManager()
{

}

void TestEraseGameManager::run(){

    std::cerr << "manager_ptr create" << std::endl;

    int cpu_num = std::thread::hardware_concurrency();
    std::vector<std::shared_ptr<GameManager>> man(cpu_num);


    for(auto& manager : man)
        manager = std::make_shared<GameManager>(8, 8, false);

    std::vector<std::thread> th(cpu_num);

    for(int count = 0; count < cpu_num; ++count)
        th.at(count) = std::thread([&](int cpu){

            for(int k = 0; k < 50; ++k){
                man.at(cpu) = std::make_shared<GameManager>(8, 8, false);
                man.at(cpu).reset();
            }
        }, count);

    for(auto& t : th)
        t.join();

    std::cerr << "end" << std::endl;
}
