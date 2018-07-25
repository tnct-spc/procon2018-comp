#include "operator.h"
#include "ui_operator.h"

Operator::Operator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Operator)
{
    ui->setupUi(this);

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
