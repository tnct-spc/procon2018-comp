#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "visualizer.h"
#include "csvio.h"
#include "field.h"
#include "progresdock.h"
#include "geneticalgo/geneticalgo.h"
#include "geneticalgo/geneticagent.h"
#include "qrcode.h"

#include <thread>
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <string>
#include <QFileDialog>

class AlgorithmWrapper;

class GameManager : public QObject
{
    Q_OBJECT
    //Q_DISABLE_COPY(GameManager)

public:
    explicit GameManager(const unsigned int x_size, const unsigned int y_size, bool vis_show = true, const int turn_max = 60, QObject *parent = 0);


    void agentAct(const int turn, const int agent, const std::tuple<int,int,int> tuple_val);

    void resetManager(const unsigned int x_size, const unsigned int y_size, bool v_show = true, const int t_max = 60);

    procon::Field& getField();

    void setFieldCount(const unsigned int number);
    unsigned int getFieldCount();

    void startSimulation(QString my_algo, QString opponent_algo, QString InputMethod);

    int simulationGenetic(const GeneticAgent& agent_1, const GeneticAgent& agent_2, int algo_number, const GeneticAgent& agent_3 = 0, const GeneticAgent& agent_4 = 0);

    unsigned int getFinalTurn();

    void setAutoMode(bool value);

    void changeTurn(bool update = true);

    int getTurnCount();

    void setField(const procon::Field& pro, int now_t, int max_t);

signals:
    void signalAutoMode(bool value);
    void setCandidateMove(const std::vector<std::vector<std::pair<int,int>>>& move);

public slots:
    void changeMove(const std::vector<std::vector<std::pair<int,int>>>& move, std::vector<std::vector<int>> is_delete);



private:

    std::shared_ptr<procon::Field> field;
    std::shared_ptr<Visualizer> visualizer;
    std::vector<std::shared_ptr<procon::Field>> field_vec;

    std::shared_ptr<AlgorithmWrapper> team_1;
    std::shared_ptr<AlgorithmWrapper> team_2;

    std::shared_ptr<ProgresDock> progresdock;


    unsigned int now_field = 0;


    //ここは仕様を変えたり変えなかったりしよう
    const int max_val = 16;
    const int min_val = -16;

    bool vis_show;

    //これがtrueなら自動進行
    bool is_auto = true;

    //行動を保存しておく
    //1:移動 移動方向をintで設定する
    //2:タイル除去 移動方向をintで設定する
    std::vector<std::vector<std::tuple<int,int,int>>> act_stack; //ここは絶対座標での入力なので注意！


    void nextMoveForManualMode();


};
#endif // GAMEMANAGER_H



