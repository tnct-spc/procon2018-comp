#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "visualizer.h"
#include "csvio.h"
#include "field.h"
#include "binaryio.h"
#include "progresdock.h"
#include "geneticalgo/geneticalgo.h"
#include "geneticalgo/geneticagent.h"
#include "qrcode.h"
#include "qrconverterfield.h"
#include "operator.h"
#include "ciphercards.h"
#include "minimumvisualizer.h"

#include <thread>
#include "csvio.h"
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
    explicit GameManager(unsigned int x_size, unsigned int y_size, bool vis_show = true, int turn_max = 60, QObject *parent = 0);


    void agentAct(const int turn, const int agent, const std::tuple<int,int,int> tuple_val);

    void resetManager(int x_size, int y_size, bool v_show = true, const int t_max = 60);

    procon::Field& getField();

    void setFieldCount(const unsigned int number);
    unsigned int getFieldCount();

    void startSimulation(QString my_algo, QString opponent_algo, QString InputMethod, std::vector<std::pair<QString, double>> my_params, std::vector<std::pair<QString, double>> opp_params);

    int simulationGenetic(const GeneticAgent& agent_1, const GeneticAgent& agent_2, int algo_number, const GeneticAgent& agent_3 = 0, const GeneticAgent& agent_4 = 0);

    unsigned int getFinalTurn();

    std::vector<int> showAgentAct(bool bside, std::tuple<int,int,int> move, bool hoge = false);

    void setAutoMode(bool value);

    void changeTurn(bool update = true);

    int getTurnCount();

    void setField(const procon::Field& pro, int now_t, int max_t);

    // ChangeModeを開始
    void startupChangeMode();



signals:
    void signalAutoMode(bool value);
    void setCandidateMove(const std::vector<std::vector<std::pair<int,int>>>& move);

    // GridがクリックされたらそのマスのステータスをOperatorに送る
    void sendDataToOperator(const std::pair<int,int> data, const bool agent);

    // OperatorのChangeがクリックされたら、変更された値をFieldとVisualizerに反映
    void sendDataToVisualizer(const std::pair<int,int> data, const bool agen);

    // Runが押されたさい、Visualizerに溜まっている移動予定をリセットする
    void resetField();

public slots:
    void changeMove(const std::vector<std::vector<std::pair<int,int>>>& move, std::vector<std::vector<int>> is_delete);

    // ChangeModeを終了
    void endChangeMode(const std::pair<int, int> turns);

    // agentの位置の変更を反映
    void changeAgentpos(std::pair<int, int> agent, std::pair<int, int> pos);

    // グリッドの状態の変更を反映
    void changeGridState(std::pair<int, int> grid, int state);

    // ChangeModeのときクリックされたGridを受け取る
    void getDataToOperator(const std::pair<int,int> grid, const bool agent);

    // OperatorからのデータをVisualizerに送る
    void getChangeOfData(const std::pair<int, int> data, const bool agent);


private:

    std::shared_ptr<procon::Field> field;
    std::shared_ptr<Visualizer> visualizer;
    std::shared_ptr<Operator> ope;
    std::shared_ptr<CipherCards> ciphercard;
    std::vector<std::shared_ptr<procon::Field>> field_vec;

    std::shared_ptr<AlgorithmWrapper> team_1;
    std::shared_ptr<AlgorithmWrapper> team_2;

    std::shared_ptr<ProgresDock> progresdock;
    std::shared_ptr<MinimumVisualizer> minimum;


    unsigned int now_field = 0;


    //ここは仕様を変えたり変えなかったりしよう
    const int max_val = 16;
    const int min_val = -16;

    bool vis_show;

    //これがtrueなら自動進行
    bool is_auto = true;

    bool use_random_field = true;

    //行動を保存しておく
    //1:移動 移動方向をintで設定する
    //2:タイル除去 移動方向をintで設定する
    std::vector<std::vector<std::tuple<int,int,int>>> act_stack; //ここは絶対座標での入力なので注意！


    void nextMoveForManualMode();

    int red_point_Cumulative = 0;
    int blue_point_Cumulative = 0;
    int match = 0;


};
#endif // GAMEMANAGER_H



