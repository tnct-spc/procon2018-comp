#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "field.h"
#include "visualizer.h"
#include "progresdock.h"

#include <thread>
#include <vector>
#include <memory>
#include <map>

class AlgorithmWrapper;

class GameManager : public QObject
{
    Q_OBJECT

public:

    explicit GameManager(const unsigned int x_size, const unsigned int y_size, QObject *parent = 0);

    procon::Field& getField();

    void setFieldCount(const unsigned int number);
    unsigned int getFieldCount();

    void startSimulation(std::shared_ptr<Visualizer> vis);

    unsigned int getFinalTurn();

    void setAutoMode(bool value);

signals:
    void signalAutoMode(bool value);

public slots:
    void changeMove(const std::vector<std::vector<std::pair<int,int>>>& move);



private:
    std::shared_ptr<procon::Field> field;
    std::shared_ptr<Visualizer> visualizer;
    std::vector<std::shared_ptr<procon::Field>> field_vec;

    std::shared_ptr<ProgresDock> progresdock;

    std::shared_ptr<AlgorithmWrapper> team_1;
    std::shared_ptr<AlgorithmWrapper> team_2;

    unsigned int now_field = 0;


    //ここは仕様を変えたり変えなかったりしよう
    const int max_val = 16;
    const int min_val = -16;
    const int turn_max = 15;

    //これがtrueなら自動進行
    bool is_auto = true;

    // 人力モード！！！！！！！！ｼｬｷｰﾝｗｗｗｗｗｗｗ
    int humanpower_mode_turn = -1;

    //行動を保存しておく
    //1:移動 移動方向をintで設定する
    //2:タイル除去 移動方向をintで設定する
    std::vector<std::vector<std::tuple<int,int,int>>> act_stack; //ここは絶対座標での入力なので注意！

    void agentAct(const int turn, const int agent, const std::tuple<int,int,int> tuple_val);
    void changeTurn();

};

#endif // GAMEMANAGER_H
