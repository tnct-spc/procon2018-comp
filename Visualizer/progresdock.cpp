#include "progresdock.h"
#include "ui_progresdock.h"

ProgresDock::ProgresDock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgresDock)
{
    ui->setupUi(this);
}

ProgresDock::~ProgresDock()
{
    delete ui;
}

void ProgresDock::addVisuAnswer(procon::Field& field){
    //fields.push_back(field);
    Visualizer* Visu = new Visualizer(field);
    Visu->setFixedSize(300, 300);

    this->ui->board_continer->addWidget(Visu,field_count/4,field_count%4);
    field_count++;
}

void ProgresDock::addMinumuVisu(std::pair<int,int> size, std::vector<std::list<std::pair<int,int>>> route, std::vector<std::vector<std::vector<int>>> color){

    MinimumVisualizer* minimum = new MinimumVisualizer(size);
    dock_back = minimum;
    minimum->setRoute(route);
    for(int index = 0; index < 3; ++index)
        minimum->setValues(color.at(index), index);

    minimum->setFixedSize(300, 300);

    connect(minimum, &MinimumVisualizer::clickField, this, &ProgresDock::clickedAnswer);

    this->ui->board_continer->addWidget(minimum,field_count/4,field_count%4);
    field_count++;
}

void ProgresDock::setValuesToBack(const std::vector<std::vector<double>>& values){
    dock_back->setVal(values);
}

void ProgresDock::clickedAnswer(std::pair<int,int> size, std::vector<std::list<std::pair<int,int>>> routes, std::vector<std::vector<std::vector<int>>> value) {
    MinimumVisualizer *minimum_new = new MinimumVisualizer(size);
    minimum_new->setRoute(routes);
    for (int index = 0; index < 3; index++) minimum_new->setValues(value.at(index), index);
    minimum_new->setFixedSize(500,500);
    minimum_new->show();
}

//void ProgresDock::mousePressEvent(QMouseEvent *event) {

////    QSize size = this->ui->board_continer
//    QPointF point = event->pos();
//    printf("OK");
//}

//void Visualizer::mousePressEvent(QMouseEvent *event) {
//    std::cout << "OK Vis" << std::endl;
//}
