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
    QGridLayout *layout = new QGridLayout;
    for (auto vis : visualizers) {
        vis->setFixedSize(300, 300);
        layout->addWidget(vis, vis_count / 2, vis_count % 2);
        vis_count++;
    }
    this->ui->visWidget->setLayout(layout);
//    this->ui->gridLayout->setSizeConstraint(500 * vis_count + 200, 700);
//    this->ui->gridLayout->widget()->setSizeIncrement(500 * vis_count + 200, 700);
}
