#include "mejirodai.h"
#include "ui_mejirodai.h"

Mejirodai::Mejirodai(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mejirodai)
{
    ui->setupUi(this);

    manager = std::make_shared<GameManager>(x_size, y_size);
    visualizer = manager->getVisualizer();
    visualizer->show();


    connect(ui->runButton, &QPushButton::clicked, this, &Mejirodai::RunManagerSimulation);
    connect(ui->goNext, &QPushButton::clicked, this, &Mejirodai::goNextState);
    connect(ui->goPrev, &QPushButton::clicked, this, &Mejirodai::goPrevState);
}

Mejirodai::~Mejirodai()
{
    delete ui;
}

void Mejirodai::RunManagerSimulation(){

    QString my = ui->selectMyAlgorithmBox->currentText();
    QString opponnent = ui->selectOpponentAlgorithmBox->currentText();

//    AlgorithmWrapper my = ui->selectMyAlgorithmBox->currentText().toStdString();
    manager->startSimulation(my, opponnent);
}

void Mejirodai::goNextState(){
    manager->setFieldCount(manager->getFieldCount() + 1);
}

void Mejirodai::goPrevState(){
    manager->setFieldCount(manager->getFieldCount() - 1);
}
