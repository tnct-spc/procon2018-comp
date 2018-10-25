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

    auto getParams = [&](QObjectList objects) {
        // childの数
        unsigned int count = 0;

        // 自チームのパラメータを取得
        if (!objects.empty()) {
            count = objects.size();
        }

        // パラメータ取得用配列
        std::vector<std::pair<QString, double>> params;

        for (unsigned int i = 0; i < count; i++) {
            // childの型
            QString classname = objects.at(i)->metaObject()->className();

            // 各型ごとにparamsにpush
            if (QString::compare(classname, QString("QDoubleSpinBox")) == 0) {
                QDoubleSpinBox *obj = qobject_cast<QDoubleSpinBox *>(objects.at(i));
                params.push_back(std::make_pair(obj->objectName(), (double)obj->value()));
            } else if (QString::compare(classname, QString("QSpinBox")) == 0) {
                QSpinBox *obj = qobject_cast<QSpinBox *>(objects.at(i));
                params.push_back(std::make_pair(obj->objectName(), (double)obj->value()));
            } else if (QString::compare(classname, QString("QCheckBox")) == 0) {
                QCheckBox *obj = qobject_cast<QCheckBox *>(objects.at(i));
                params.push_back(std::make_pair(obj->objectName(), (double)obj->isChecked()));
            }
        }

        return params;
    };

    // 自チームのアルゴリズム用パラメータ
    std::vector<std::pair<QString, double>> my_params;

    QObjectList objects;

    // childern
    if (ui->my_stackedWidget->widget(ui->selectMyAlgorithmBox->currentIndex())) {
        objects = ui->my_stackedWidget->widget(ui->selectMyAlgorithmBox->currentIndex())->children();
        my_params = getParams(objects);
    }

    // 相手チームのアルゴリズム用パラメータ
    std::vector<std::pair<QString, double>> opponent_params;

    if (ui->opponent_stackedWidget->widget(ui->selectOpponentAlgorithmBox->currentIndex())) {
        objects = ui->opponent_stackedWidget->widget(ui->selectOpponentAlgorithmBox->currentIndex())->children();
        opponent_params = getParams(objects);
    }

//    AlgorithmWrapper my = ui->selectMyAlgorithmBox->currentText().toStdString();
    manager->startSimulation(my, opponnent, InputMethod, my_params, opponent_params);
}

void Mejirodai::goNextState(){
    manager->setFieldCount(manager->getFieldCount() + 1, false);
}

void Mejirodai::goPrevState(){
    manager->setFieldCount(manager->getFieldCount() - 1, false);
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
