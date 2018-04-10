#include "mejirodai.h"
#include "ui_mejirodai.h"

Mejirodai::Mejirodai(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mejirodai)
{
    ui->setupUi(this);

    manager = std::make_shared<GameManager>(x_size, y_size);

    connect(ui->runButton, &QPushButton::clicked, this, &Mejirodai::RunManagerSimulation);
}

Mejirodai::~Mejirodai()
{
    delete ui;
}

void Mejirodai::RunManagerSimulation(){

    manager->startSimulation();
}
