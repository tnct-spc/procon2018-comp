#ifndef MULTIPLEVISUALIZER_H
#define MULTIPLEVISUALIZER_H

#include <QWidget>
#include <QGridLayout>
#include "visualizer.h"
#include "progresdock.h"

namespace Ui {
class MultipleVisualizer;
}

class MultipleVisualizer : public QWidget
{
    Q_OBJECT

public:
    explicit MultipleVisualizer(QWidget *parent = nullptr);
    ~MultipleVisualizer();

    // 表示するVisualizerの設置
    void setVisualizers(std::vector<Visualizer *> visalizers);

private:
    Ui::MultipleVisualizer *ui;
    int vis_count = 0;

    std::shared_ptr<ProgresDock> dock;
};

#endif // MULTIPLEVISUALIZER_H
