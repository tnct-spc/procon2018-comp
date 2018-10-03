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

void MinimumVisualizerDock::addVisualizer(std::pair<int,int> size, std::vector<std::list<std::pair<int,int>>> route, std::vector<std::vector<std::vector<int>>> color){

    MinimumVisualizer* minimum = new MinimumVisualizer(size);
    minimum->setRoute(route);
    for(int index = 0; index < 3; ++index)
        minimum->setValues(color.at(index), index);

    this->ui->container->addWidget(minimum, count / width, count % width);
    ++count;
}
