#include "progresdoc.h"

ProgresDoc::ProgresDoc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgresDoc)
{
    ui->setupUi(this);
    this->setWindowTitle("ProgresDoc");
}
ProgresDoc::~ProgresDoc()
{
    delete ui;
}
