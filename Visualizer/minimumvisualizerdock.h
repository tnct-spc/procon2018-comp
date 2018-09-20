#ifndef MINIMUMVISUALIZERDOCK_H
#define MINIMUMVISUALIZERDOCK_H

#include <QDialog>
#include "minimumvisualizer.h"
#include <memory>

namespace Ui {
class MinimumVisualizerDock;
}

class MinimumVisualizerDock : public QDialog
{
    Q_OBJECT

public:
    explicit MinimumVisualizerDock(int width, QWidget *parent = nullptr);
    ~MinimumVisualizerDock();

    void addVisualizer(std::pair<int,int> size, std::list<std::pair<int,int>> route, std::vector<std::vector<std::vector<int>>> color);

private:
    int width;
    Ui::MinimumVisualizerDock *ui;
    int count = 0;

};

#endif // MINIMUMVISUALIZERDOCK_H
