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

void MultipleVisualizer::setVisualizers(std::vector<Visualizer *> visualizers)
{
    for (auto vis : visualizers) {
        vis->setFixedSize(300, 300);
        this->ui->gridLayout->addWidget(vis, vis_count / 2, vis_count % 2);
    }
}
