#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include <memory>
#include <tuple>

class GameManager;

class AlgorithmWrapper
{

public:
    AlgorithmWrapper(std::shared_ptr<GameManager> manager_ptr);

    //turn,agentnumber,type,x,y
    virtual std::tuple<int,int,int,int,int> agentAct();


protected:
    std::shared_ptr<GameManager> manager;
};

#endif // ALGORITHMWRAPPER_H
