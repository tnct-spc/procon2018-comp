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

void ProgresDock::addAnswer(procon::Field& field){
    //fields.push_back(field);
    Visualizer* Visu = new Visualizer(field);
    Visu->setFixedSize(300, 300);

    this->ui->board_continer->addWidget(Visu,field_count/4,field_count%4);
    field_count++;
}