#ifndef OPERATOR_H
#define OPERATOR_H

#include "field.h"
#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>

namespace Ui {
class Operator;
}

class Operator : public QWidget
{
    Q_OBJECT

public:
    explicit Operator(QWidget *parent = 0);
    ~Operator();

signals:
    void pushChange();
    void pushEnd();

public slots:
    void changeGridDisplay(const std::pair<int, int> state);

private:
    Ui::Operator *ui;

    void sendPushEnd();
};

#endif // OPERATOR_H
