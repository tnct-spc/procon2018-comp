#include "mejirodai.h"
#include "ui_mejirodai.h"

Mejirodai::Mejirodai(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mejirodai)
{
    ui->setupUi(this);
}

Mejirodai::~Mejirodai()
{
    delete ui;
}
