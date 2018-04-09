#ifndef MEJIRODAI_H
#define MEJIRODAI_H

#include "gamemanager.h"
#include <memory>
#include <QMainWindow>

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

};

#endif // MEJIRODAI_H
