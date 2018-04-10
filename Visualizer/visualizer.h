#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "field.h"
#include "gamemanager.h"
#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>

#include <memory>



namespace Ui {
class Visualizer;
}

class Visualizer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Visualizer(QWidget *parent = 0);
    ~Visualizer();

private:
    Ui::Visualizer *ui;
    void paintEvent(QPaintEvent *event);

    std::shared_ptr<GameManager> manager;

    int window_width;
    int window_height;

    int vertical_margin;
    int horizontal_margin;

    int grid_size;

    //ここはgetterで参照
    unsigned int grid_x = 12;
    unsigned int grid_y = 8;


    //margin*size分の余白を取る
    const double margin = 1.5;


    const QColor font_color = QColor(0,0,0,64);
    const QColor background_color = QColor(245,245,220);
    const QColor grid_color = QColor(220,220,180);
    const QColor team_color_a = QColor(255,0,0);
    const QColor team_color_b = QColor(0,0,255);

};

#endif // MAINWINDOW_H
