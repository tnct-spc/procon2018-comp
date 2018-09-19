#ifndef MINIMUMVISUALIZER_H
#define MINIMUMVISUALIZER_H

#include <utility>
#include <QDialog>
#include <QPainter>

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

private:
    std::pair<int,int> size;
    Ui::MinimumVisualizer *ui;
    void paintEvent(QPaintEvent *event);
};

#endif // MINIMUMVISUALIZER_H
