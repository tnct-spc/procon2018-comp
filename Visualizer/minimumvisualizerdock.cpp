#include "minimumvisualizerdock.h"
#include "ui_minimumvisualizerdock.h"

MinimumVisualizerDock::MinimumVisualizerDock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MinimumVisualizerDock)
{
    ui->setupUi(this);
}

MinimumVisualizerDock::~MinimumVisualizerDock()
{
    delete ui;
}

void MinimumVisualizerDock::addVisualizer(std::shared_ptr<MinimumVisualizer> minimum){
    minimum->setFixedSize(300, 300);
    this->ui->container->addWidget(minimum.get(), count / 4, count % 4);
    ++count;
}
