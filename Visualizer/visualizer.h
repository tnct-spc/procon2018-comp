#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "field.h"
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>

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

    void setField(procon::Field& inp_field);

    std::vector<std::vector<std::pair<int, int>>> getNextAgents();

    std::vector<std::vector<std::pair<int,int>>> clickWait(std::vector<std::vector<std::pair<int,int>>> val);

private:
    Ui::Visualizer *ui;
    void paintEvent(QPaintEvent *event);

    procon::Field& field;

    void mousePressEvent(QMouseEvent *event);

    void checkClickedAgent(std::pair<int, int> mass);

    bool checkClickGrid(std::pair<int, int> mass);

    int window_width;
    int window_height;

    int vertical_margin;
    int horizontal_margin;

    int grid_size;

    unsigned int grid_x;
    unsigned int grid_y;

    // 移動を入力するエージェントが選択されているか
    bool selected = false;

    // 移動を入力するエージェントのチームとエージェント番号
    // first：チーム,second：エージェント
    std::pair<int, int> selected_agent;

    // 移動を入力するエージェントのグリッド座標
    std::pair<int, int> selected_agent_grid;

    // 各エージェントが移動先を決定しているかどうか
    // std::array<std::array<bool, 2>, 2> decided_agents = {false, false, false, false};

    // 各エージェントの移動先を記録
    std::vector<std::vector<std::pair<int, int>>> next_grids;

    //margin*size分の余白を取る
    const double margin = 1.5;

    const QColor font_color = QColor(0,0,0,64);
    const QColor background_color = QColor(245,245,220);
    const QColor grid_color = QColor(220,220,180);
    const QColor team_color_a = QColor(255,0,0);
    const QColor team_color_b = QColor(0,0,255);
    const QColor checked_color_a = QColor(255,120,0);
    const QColor checked_color_b = QColor(0,120,255);

};

#endif // MAINWINDOW_H
