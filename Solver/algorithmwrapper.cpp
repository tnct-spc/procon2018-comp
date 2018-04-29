#include "algorithmwrapper.h"
#include "gamemanager.h"

AlgorithmWrapper::AlgorithmWrapper(const std::shared_ptr<GameManager> manager_ptr)
{
    manager = manager_ptr;

}

std::shared_ptr<GameManager> AlgorithmWrapper::getManagerPtr(){
    return manager;
}
