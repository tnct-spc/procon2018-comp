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

private:
    Ui::MultipleVisualizer *ui;
};

#endif // MULTIPLEVISUALIZER_H
