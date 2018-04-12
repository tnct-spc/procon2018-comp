#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "field.h"
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>

#include <memory>


namespace Ui {
class Visualizer;
}

class Visualizer : public QWidget
{
    Q_OBJECT

public:
    explicit Visualizer(procon::Field inp_field, QWidget *parent = 0);
    ~Visualizer();

    void setField(procon::Field inp_field);

private:
    Ui::Visualizer *ui;
    void paintEvent(QPaintEvent *event);

    procon::Field field;

    int window_width;
    int window_height;

    int vertical_margin;
    int horizontal_margin;

    int grid_size;

    unsigned int grid_x;
    unsigned int grid_y;


    //margin*size分の余白を取る
    const double margin = 1.5;


    const QColor font_color = QColor(0,0,0,64);
    const QColor background_color = QColor(245,245,220);
    const QColor grid_color = QColor(220,220,180);
    const QColor team_color_a = QColor(255,0,0);
    const QColor team_color_b = QColor(0,0,255);

};

#endif // MAINWINDOW_H
