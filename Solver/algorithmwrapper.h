#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"
#include <memory>
#include <tuple>
#include <utility>
#include <QString>

class GameManager;

class AlgorithmWrapper
{
public:
    AlgorithmWrapper(const procon::Field& field, int final_turn, bool side);

    virtual const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> agentAct(int now_turn)=0;
    virtual void setParams(std::vector<std::pair<QString, double>> params) = 0;
    virtual void setRandomParams(std::vector<std::pair<QString, double>> params)=0;

protected:
    const procon::Field& field;
    int final_turn;
    bool side;
};

#endif // ALGORITHMWRAPPER_H
