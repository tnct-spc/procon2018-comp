#include "operator.h"
#include "ui_operator.h"

Operator::Operator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Operator)
{
    ui->setupUi(this);

    connect(ui->changeButton, &QPushButton::clicked, this, &Operator::sendPushChange);
    connect(ui->endButton, &QPushButton::clicked, this, &Operator::sendPushEnd);
}

Operator::~Operator()
{
    delete ui;
}

void Operator::sendPushEnd()
{
    emit pushEnd();
}

void Operator::sendPushChange()
{
    if (agent) {
        data = std::make_pair<int, int>(ui->xBox->value(), ui->yBox->value());
    } else {
        data = std::make_pair<int, int>(ui->teamBox->value(), ui->pointBox->value());
    }

    emit pushChange(data, agent);
}

void Operator::changeDataDisplay(const std::pair<int, int> inp_data, const bool inp_bool)
{
    agent = inp_bool;
    data = inp_data;

    if (agent) {
        ui->stackedWidget->setCurrentWidget(ui->agent);
        ui->xBox->setValue(data.first);
        ui->yBox->setValue(data.second);

    } else {
        ui->stackedWidget->setCurrentWidget(ui->grid);
        ui->teamBox->setValue(data.first);
        ui->pointBox->setValue(data.second);
    }

}
