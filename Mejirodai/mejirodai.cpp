#include "mejirodai.h"
#include "ui_mejirodai.h"

Mejirodai::Mejirodai(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mejirodai)
{
    ui->setupUi(this);

    manager = std::make_shared<GameManager>(x_size, y_size);
    /*
    visualizer = manager->getVisualizer();
    visualizer->show();
    */

    connect(ui->runButton, &QPushButton::clicked, this, &Mejirodai::RunManagerSimulation);
    connect(ui->goNext, &QPushButton::clicked, this, &Mejirodai::goNextState);
    connect(ui->goPrev, &QPushButton::clicked, this, &Mejirodai::goPrevState);
    connect(ui->exportField, &QPushButton::clicked, this, &Mejirodai::exportFieldtoCSV);
    connect(ui->selectMyAlgorithmBox, SIGNAL(currentIndexChanged(int)), ui->my_stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->selectOpponentAlgorithmBox, SIGNAL(currentIndexChanged(int)), ui->opponent_stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->changeButton, &QPushButton::clicked, this, &Mejirodai::runOperatorWindow);
    connect(ui->ExportFieldBinary, &QPushButton::clicked,this , &Mejirodai::exportFieldtoBinary);
}

Mejirodai::~Mejirodai()
{
    delete ui;
}

void Mejirodai::RunManagerSimulation(){

    runMode = true;

    // 各チームのアルゴリズムの設定
    QString my = ui->selectMyAlgorithmBox->currentText();
    QString opponnent = ui->selectOpponentAlgorithmBox->currentText();

    QString InputMethod = ui->InputMethodSelect->currentText();

    // AutoModeの設定
    manager->setAutoMode(ui->autoMode->isChecked());

//    AlgorithmWrapper my = ui->selectMyAlgorithmBox->currentText().toStdString();
//    manager->startSimulation(my, opponnent, InputMethod);
    manager->startThread(my, opponnent, InputMethod);
    std::cout << "untiii!!!" << std::endl;
}

void Mejirodai::goNextState(){
    manager->setFieldCount(manager->getFieldCount() + 1);
}

void Mejirodai::goPrevState(){
    manager->setFieldCount(manager->getFieldCount() - 1);
}

void Mejirodai::exportFieldtoCSV(){
    procon::Field& exp_field = manager->getField();
    procon::CsvIo::exportField(exp_field, QFileDialog::getSaveFileName(this,tr("Save CSV")).toStdString());
}

void Mejirodai::runOperatorWindow(){

    // Runが押され、かつAutoModeがOffのときのみOperatorのウィンドウが立ち上がる
    if ((!ui->autoMode->isChecked()) && runMode) {
        manager->startupChangeMode();
    }
}

void Mejirodai::exportFieldtoBinary(){
    procon::Field& exp_field = manager->getField();
    procon::BinaryIo::exportField(exp_field, QFileDialog::getSaveFileName(this,tr("Save CSV")).toStdString());
}
