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
    explicit Operator(procon::Field& inp_field, QWidget *parent = 0);
    ~Operator();

private:
    Ui::Operator *ui;

    procon::Field field;
};

#endif // OPERATOR_H
