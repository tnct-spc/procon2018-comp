#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "field.h"
#include "visualizer.h"
#include "progresdock.h"

#include <vector>
#include <memory>
#include <map>

class AlgorithmWrapper;
class TestAlgorithm;

class GameManager
{
public:

    GameManager(const unsigned int x_size, const unsigned int y_size);

    const procon::Field& getField();

    void setFieldCount(const unsigned int number);
    unsigned int getFieldCount();

    void startSimulation();

private:
    std::shared_ptr<procon::Field> field;
    std::vector<procon::Field> field_vec;
    std::shared_ptr<Visualizer> visualizer;

    std::shared_ptr<ProgresDock> progresdock;

    std::shared_ptr<AlgorithmWrapper> team_1;
    std::shared_ptr<AlgorithmWrapper> team_2;

    unsigned int now_field = 0;


    //ここは仕様を変えたり変えなかったりしよう
    const int max_val = 16;
    const int min_val = -16;
    const int turn_max = 30;

    //行動を保存しておく
    //1:移動 移動方向をintで設定する
    //2:タイル除去 移動方向をintで設定する
    std::vector<std::vector<std::pair<int,std::pair<int,int>>>> act_stack;

    void agentAct(const int turn, const int agent, const int type, const int x_pos, const int y_pos);
    void changeTurn();

};

#endif // GAMEMANAGER_H
