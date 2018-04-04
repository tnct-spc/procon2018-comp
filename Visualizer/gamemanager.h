#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "field.h"

#include <vector>
#include <memory>

class GameManager
{
public:

    GameManager(unsigned int x_size, unsigned int y_size);

    procon::Field getField();

    bool stayAgent(bool turn, int number);
    bool moveAgent(bool turn, int number, int rotate_num);
    bool deleteTile(bool turn, int number, int rotate_num);

private:
    std::shared_ptr<procon::Field> field;

    //ここは後で仕様を変える
    const int max_val = 16;
    const int min_val = -16;

    //行動を保存しておく
    //0:停留 引数なし
    //1:移動 移動方向をintで設定する
    //2:タイル除去 移動方向をintで設定する
    std::vector<std::vector<std::pair<int,int>>> act_stack;

    bool agentAct(int turn, int agent, int type, int number);
    bool changeTurn();

};

#endif // GAMEMANAGER_H
