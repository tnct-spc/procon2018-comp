#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "field.h"

#include <vector>
#include <memory>
#include <map>

class AlgorithmWrapper;
class TestAlgorithm;

class GameManager
{
public:

    GameManager(unsigned int x_size, unsigned int y_size);

    procon::Field getField();

    //ここの関数は既に行動が決まっている引数を渡された時にfalseを返す
    bool stayAgent(bool turn, int number);
    bool moveAgent(bool turn, int number, int x_pos, int y_pos);
    bool deleteTile(bool turn, int number, int x_pos, int y_pos);

private:
    std::shared_ptr<procon::Field> field;

    std::shared_ptr<AlgorithmWrapper> team_1;
    std::shared_ptr<AlgorithmWrapper> team_2;


    //ここは後で仕様を変える
    const int max_val = 16;
    const int min_val = -16;

    //行動を保存しておく
    //1:移動 移動方向をintで設定する
    //2:タイル除去 移動方向をintで設定する
    std::vector<std::vector<std::pair<int,std::pair<int,int>>>> act_stack;

    bool agentAct(int turn, int agent, int type, int x_pos, int y_pos);
    void changeTurn();

};

#endif // GAMEMANAGER_H
