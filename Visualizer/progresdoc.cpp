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
void ProgresDoc::addAnswer(procon::Field const& field)
{
    fields.push_back(field);
    Visualizer* visual = new Visualizer(field, );
    visual->setField(field);
    visual->setFixedSize(300, 300);
}
