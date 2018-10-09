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
        vis->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(vis, vis_count / 2, vis_count % 2);
        vis_count++;
    }
    this->ui->visWidget->setLayout(layout);
}
