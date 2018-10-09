#include "multiplevisualizer.h"
#include "ui_multiplevisualizer.h"

MultipleVisualizer::MultipleVisualizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultipleVisualizer)
{
    ui->setupUi(this);
}

MultipleVisualizer::~MultipleVisualizer()
{
    delete ui;
}
