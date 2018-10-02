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

    ui->selectMyAlgorithmBox->currentIndexChanged(0);
    ui->selectOpponentAlgorithmBox->currentIndexChanged(0);
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

    // 自チームのアルゴリズム用パラメータ
    std::vector<std::pair<QString, double>> my_params;

    // childの数
    unsigned int count;

    // 自チームのパラメータを取得
    if (ui->my_stackedWidget->widget(ui->selectMyAlgorithmBox->currentIndex())->layout()) {
        count = ui->my_stackedWidget->widget(ui->selectMyAlgorithmBox->currentIndex())->layout()->count();
    } else {
        count = 0;
    }

    for (unsigned int i = 0; i < count; i++) {
        // 各childを取得
        QWidget *widget = ui->my_stackedWidget->widget(ui->selectMyAlgorithmBox->currentIndex())->layout()->itemAt(i)->widget();

        // childの型
        QString classname = widget->metaObject()->className();

        if (QString::compare(classname, QString("QDoubleSpinBox")) == 0) {
            my_params.push_back(std::make_pair(widget->objectName(), widget->property("value").toDouble()));
        } else if (QString::compare(classname, QString("QSpinBox")) == 0) {
            my_params.push_back(std::make_pair(widget->objectName(), widget->property("value").toDouble()));
        } else if (QString::compare(classname, QString("QCheckBox")) == 0) {
            my_params.push_back(std::make_pair(widget->objectName(), widget->property("tristate").toDouble()));
        }
    }

    // 相手チームのアルゴリズム用パラメータ
    std::vector<std::pair<QString, double>> opponent_params;

    // 相手チームのパラメータを取得
    if (ui->opponent_stackedWidget->widget(ui->selectOpponentAlgorithmBox->currentIndex())->layout()) {
        count = ui->opponent_stackedWidget->widget(ui->selectOpponentAlgorithmBox->currentIndex())->layout()->count();
    } else {
        count = 0;
    }

    for (unsigned int i = 0; i < count; i++) {
        // 各childを取得
        QWidget *widget = ui->opponent_stackedWidget->widget(ui->selectOpponentAlgorithmBox->currentIndex())->layout()->itemAt(i)->widget();

        // childの型
        QString classname = widget->metaObject()->className();

        if (QString::compare(classname, QString("QDoubleSpinBox")) == 0) {
            opponent_params.push_back(std::make_pair(widget->objectName(), widget->property("value").toDouble()));
        } else if (QString::compare(classname, QString("QSpinBox")) == 0) {
            opponent_params.push_back(std::make_pair(widget->objectName(), widget->property("value").toDouble()));
        } else if (QString::compare(classname, QString("QCheckBox")) == 0) {
            opponent_params.push_back(std::make_pair(widget->objectName(), widget->property("tristate").toDouble()));
        }
    }

//    AlgorithmWrapper my = ui->selectMyAlgorithmBox->currentText().toStdString();
    manager->startSimulation(my, opponnent, InputMethod);
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
