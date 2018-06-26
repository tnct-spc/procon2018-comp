#include "testerasegamemanager.h"

TestEraseGameManager::TestEraseGameManager()
{

}

void TestEraseGameManager::run(){

    std::cerr << "manager_ptr create" << std::endl;
    GameManager* manager = new GameManager(8, 8, false);

    std::cerr << "delete" << std::endl;
    delete manager;
    std::cerr << "end" << std::endl;
}
