#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "field.h"

#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>


#include <memory>
#include <iostream>


namespace Ui {
class Visualizer;
}

class Visualizer : public QWidget
{
    Q_OBJECT

public:
    explicit Visualizer(procon::Field& inp_field, QWidget *parent = 0);
    ~Visualizer();

    void setField(const procon::Field& inp_field, int now_turn, int max_t);

    procon::Field getField();

    void setTurns(const std::pair<int, int> turns);

    void setChangeMode(bool value);

    // エージェントの位置を書き換える
    void setAgentPos(const std::pair<int, int> agent, const std::pair<int, int> pos);

    // グリッドの状態を書き換える
    void setGridState(const std::pair<int, int> grid, const int state);

signals:
    void nextMove(const std::vector<std::vector<std::pair<int,int>>>& inp_vec, std::vector<std::vector<int>> is_delete);
    void selectChangeGrid(const std::pair<int, int> grid, const bool agent);

    // Gamemanagerに変更を反映させる
    void sendAgentPos(const std::pair<int, int> agent, const std::pair<int, int> pos);
    void sendGridState(const std::pair<int, int> grid, const int state);
    void sendRecalculation(const std::pair<int, int> turns);
    void sendRotateField(bool direction);

public slots:
    void slotAutoMode(bool value);
    void candidateMove(const std::vector<std::vector<std::pair<int,int>>>& inp_vec);
    void getData(const std::pair<int, int> data, const bool agent);

    // 指定された移動予定を解除
    void resetConfirm();

private:
    Ui::Visualizer *ui;
    void paintEvent(QPaintEvent *event);

    procon::Field field;

    void mousePressEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void checkClickedAgent(std::pair<int, int> mass);

    void checkClickGrid(std::pair<int, int> mass, bool right_flag);

    // フィールドを回転させた際、他のパラメータも変更する
    void updateToRotateField(bool direction);

    int window_width;
    int window_height;

    int vertical_margin;
    int horizontal_margin;

    int turn = 0;

    int max_turn = 0;

    int grid_size;

    bool auto_mode = true;

    // 移動を入力するエージェントが選択されているか
    bool selected = false;

    // 手動ModeでかつChangeModeのときtrue
    bool change_mode = false;

    bool clicked = false;

    //フィールドの編集モード/プレイモードの判定
    bool is_change_field_mode = false;

    //編集モードでグリッドが選択されているかの判定
    bool is_changing_field_grid = false;

    bool is_moving_agent = false;

    bool is_selected_grid = false;

    // 再計算時に表示
    bool is_recalcuration = false;

    std::pair<int, int> selected_to_change_grid;


    unsigned int confirm_count = 0;

    // 移動を入力するエージェントのチームとエージェント番号
    // first：チーム,second：エージェント
    std::pair<int, int> selected_agent;

    // 移動を入力するエージェントのグリッド座標
    std::pair<int, int> selected_agent_grid;

    // 各エージェントの移動先を記録
    std::vector<std::vector<std::pair<int, int>>> next_grids;

    std::vector<std::vector<int>> is_delete;

    //こっちはmanualmode用
    std::vector<std::vector<std::pair<int, int>>> candidate;

    // クリックされたGrid
    std::pair<int, int> clicked_grid_change;

    //margin*size分の余白を取る
    const double margin = 2;

    const QColor font_color = QColor(0,0,0,64);
    const QColor background_color = QColor(245,245,220);
    const QColor grid_color = QColor(220,220,180);
    const QColor team_color_a = QColor(255,0,0);
    const QColor team_color_b = QColor(0,0,255);
    const QColor checked_color_a = QColor(255,120,0);
    const QColor checked_color_b = QColor(0,120,255);
    const QColor selected_grid_color = QColor(0, 0, 0, 50);

};

#endif // MAINWINDOW_H
