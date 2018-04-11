#ifndef PROGRESDOCK_H
#define PROGRESDOCK_H

#include <QWidget>

namespace Ui {
class ProgresDock;
}

class ProgresDock : public QWidget
{
    Q_OBJECT

public:
    explicit ProgresDock(QWidget *parent = 0);
    ~ProgresDock();

private:
    Ui::ProgresDock *ui;
};

#endif // PROGRESDOCK_H
