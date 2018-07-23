#include "operator.h"
#include "ui_operator.h"

Operator::Operator(procon::Field& inp_field, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Operator),
    field(inp_field)
{
    ui->setupUi(this);

    field = inp_field;
}

Operator::~Operator()
{
    delete ui;
}
