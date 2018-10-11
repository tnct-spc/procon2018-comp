#include "multiplevisualizer.h"
#include "ui_multiplevisualizer.h"

MultipleVisualizer::MultipleVisualizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultipleVisualizer)
{
    ui->setupUi(this);

//    dock = std::make_shared<ProgresDock>();
//    dock->show();
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

void MultipleVisualizer::setCsv(std::pair<std::string, std::string> csv_path){
//    clearLayout(this->ui->visWidget->layout());
    if (!this->ui->visWidget->layout()) {
        QGridLayout *layout = new QGridLayout;
        vis_count = 0;
        std::pair<Visualizer*, Visualizer*> vis;
        std::pair<procon::Field,procon::Field> field_pair(procon::CsvIo::importField(csv_path.first), procon::CsvIo::importField(csv_path.second));
        vis.first = new Visualizer(field_pair.first);
        vis.second = new Visualizer(field_pair.second);

        vis.first->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        vis.second->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(vis.first, 0, 0);
        layout->addWidget(vis.second, 0, 1);

        this->ui->visWidget->setLayout(layout);

        return;
    }

    std::pair<procon::Field,procon::Field> field_pair(procon::CsvIo::importField(csv_path.first), procon::CsvIo::importField(csv_path.second));

    QObjectList objects = this->ui->visWidget->children();

    Visualizer *vis1 = qobject_cast<Visualizer *>(objects.at(1));
    Visualizer *vis2 = qobject_cast<Visualizer *>(objects.at(2));

    vis1->setField(field_pair.first,0,60);
    vis2->setField(field_pair.second,0,60);

    vis1->update();
    vis1->repaint();
    vis2->update();
    vis2->repaint();


//    this->ui->visWidget->layout()->update();
//    this->update();
//    this->repaint();
//    this->ui->visWidget->update();
//    this->ui->visWidget->repaint();
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
//    this->ui->visWidget->setLayout(layout);
//    this->update();
//    this->repaint();
//    this->ui->visWidget->layout()->update();
//    dock->update();
//    dock->repaint();
    QWidget::update();
}
