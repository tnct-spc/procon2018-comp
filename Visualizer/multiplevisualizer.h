#ifndef MULTIPLEVISUALIZER_H
#define MULTIPLEVISUALIZER_H

#include <QWidget>
#include "visualizer.h"

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
};

#endif // MULTIPLEVISUALIZER_H
