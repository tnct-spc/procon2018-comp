#ifndef MINIMUMVISUALIZER_H
#define MINIMUMVISUALIZER_H

#include <utility>
#include <QDialog>
#include <QPainter>
#include <QMouseEvent>

namespace Ui {
class MinimumVisualizer;
}

class MinimumVisualizer : public QDialog
{
    Q_OBJECT

public:
    explicit MinimumVisualizer(std::pair<int,int> size, QWidget *parent = nullptr);
    ~MinimumVisualizer();

    void setSize(std::pair<int,int> siz);
    void setRoute(std::vector<std::list<std::pair<int,int>>>& rout);
    void setValues(std::vector<std::vector<int>>& vec, int rgba);
    void setVal(std::vector<std::vector<double>> dval);

signals:
    void clickField(std::pair<int,int> size, std::vector<std::list<std::pair<int,int>>> routes, std::vector<std::vector<std::vector<int>>> value);

private:
    std::pair<int,int> size;
    Ui::MinimumVisualizer *ui;
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

    std::vector<std::list<std::pair<int,int>>> routes;
    std::vector<std::vector<std::vector<int>>> values;
    std::vector<std::vector<double>> val;
};

#endif // MINIMUMVISUALIZER_H
