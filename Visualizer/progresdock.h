#ifndef PROGRESDOCK_H
#define PROGRESDOCK_H

#include <QWidget>
#include"field.h"
#include"visualizer.h"

namespace Ui {
class ProgresDock;
}

class ProgresDock : public QWidget
{
    Q_OBJECT

public:
    explicit ProgresDock(QWidget *parent = 0);
    ~ProgresDock();

    void addAnswer(procon::Field& field);

private:
    Ui::ProgresDock *ui;
    std::vector<procon::Field> fields;
};

#endif // PROGRESDOCK_H
