#ifndef OPERATOR_H
#define OPERATOR_H

#include "field.h"
#include "visualizer.h"
#include <QWidget>

namespace Ui {
class Operator;
}

class Operator : public QWidget
{
    Q_OBJECT

public:
    explicit Operator(QWidget *parent = 0);
    ~Operator();

private:
    Ui::Operator *ui;
};

#endif // OPERATOR_H
