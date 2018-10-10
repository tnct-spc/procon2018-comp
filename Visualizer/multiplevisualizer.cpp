#include "multiplevisualizer.h"
#include "ui_multiplevisualizer.h"

MultipleVisualizer::MultipleVisualizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultipleVisualizer)
{
    ui->setupUi(this);

    dock = std::make_shared<ProgresDock>();
    dock->show();
}

MultipleVisualizer::~MultipleVisualizer()
{
    delete ui;
}

void MultipleVisualizer::clearLayout(QLayout *layout)
{
    if (!layout) return;
    QLayoutItem *item;
    while ((item = layout->takeAt(0))) {
        delete item->widget();
    }
    delete layout;
}

void MultipleVisualizer::setVisualizers(std::vector<Visualizer *> visualizers)
{
    clearLayout(this->ui->visWidget->layout());
    QGridLayout *layout = new QGridLayout;
    vis_count = 0;
    for (auto vis : visualizers) {
        vis->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(vis, vis_count / 2, vis_count % 2);
        vis_count++;

        procon::Field field = vis->getField();
//        dock->addVisuAnswer(field);
    }
    this->ui->visWidget->setLayout(layout);
    this->update();
    this->repaint();
    this->ui->visWidget->layout()->update();
//    dock->update();
//    dock->repaint();
}
