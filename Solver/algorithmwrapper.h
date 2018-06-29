#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"
#include <memory>
#include <tuple>
#include <utility>

class GameManager;

class AlgorithmWrapper
{
public:
    // AlgorithmWrapper(const std::shared_ptr<GameManager> manager_ptr);
    AlgorithmWrapper(const procon::Field& field, int final_turn, bool side);

    virtual const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> agentAct()=0;

protected:
    const procon::Field& field;
    int final_turn;
    bool side;
};

#endif // ALGORITHMWRAPPER_H
