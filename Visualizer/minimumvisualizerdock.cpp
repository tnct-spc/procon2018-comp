#include "minimumvisualizerdock.h"
#include "ui_minimumvisualizerdock.h"

MinimumVisualizerDock::MinimumVisualizerDock(int width, QWidget *parent) :
    width(width),
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
    visualizers.emplace_back(minimum);
    this->ui->container->addWidget(minimum.get(), count / width, count % width);
    ++count;
}
