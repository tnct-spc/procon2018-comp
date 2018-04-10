#ifndef MEJIRODAI_H
#define MEJIRODAI_H

#include "gamemanager.h"
#include <memory>
#include <QMainWindow>
#include <QPushButton>
#include <iostream>

namespace Ui {
class Mejirodai;
}

class Mejirodai : public QMainWindow
{
    Q_OBJECT

public:
    explicit Mejirodai(QWidget *parent = 0);
    ~Mejirodai();

private:
    Ui::Mejirodai *ui;

    std::shared_ptr<GameManager> manager;

    void RunManagerSimulation();

    int x_size = 12;
    int y_size = 8;

};

#endif // MEJIRODAI_H
