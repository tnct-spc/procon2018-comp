#ifndef PROGRESDOCK_H
#define PROGRESDOCK_H

#include <QWidget>
#include"field.h"
#include"visualizer.h"
#include"minimumvisualizer.h"

namespace Ui {
class ProgresDock;
}

class ProgresDock : public QWidget
{
    Q_OBJECT

public:
    explicit ProgresDock(QWidget *parent = 0);
    ~ProgresDock();

    void addVisuAnswer(procon::Field& field);
    void addMinumuVisu(std::pair<int,int> size, std::vector<std::list<std::pair<int,int>>> route, std::vector<std::vector<std::vector<int>>> color);
    void setValuesToBack(const std::vector<std::vector<double>>& values);

private:
    Ui::ProgresDock *ui;
    MinimumVisualizer* dock_back;
    unsigned int field_count = 0;
//    std::vector<procon::Field> fields;
};

#endif // PROGRESDOCK_H
