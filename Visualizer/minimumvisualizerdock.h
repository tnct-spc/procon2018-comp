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
    explicit MinimumVisualizerDock(QWidget *parent = nullptr);
    ~MinimumVisualizerDock();

    void addVisualizer(std::shared_ptr<MinimumVisualizer> minimum);

private:
    Ui::MinimumVisualizerDock *ui;
    int count = 0;
};

#endif // MINIMUMVISUALIZERDOCK_H
