#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include <memory>

class GameManager;

class AlgorithmWrapper
{

public:
    AlgorithmWrapper(std::shared_ptr<GameManager> manager_ptr);

private:
    std::shared_ptr<GameManager> manager;
};

#endif // ALGORITHMWRAPPER_H
