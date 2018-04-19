#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include <memory>
#include <tuple>
#include <utility>

class GameManager;

class AlgorithmWrapper
{
public:
    AlgorithmWrapper(std::shared_ptr<GameManager> manager_ptr);

    //turn,agentnumber,type,x,y
    virtual std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> agentAct(int side)=0;


protected:

    std::shared_ptr<GameManager> manager;
};

#endif // ALGORITHMWRAPPER_H
