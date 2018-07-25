#ifndef MEJIRODAI_H
#define MEJIRODAI_H

#include "gamemanager.h"
#include "csvio.h"

#include <thread>
#include <memory>
#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QCheckBox>

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
    void goNextState();
    void goPrevState();
    void exportFieldtoCSV();
    void runOperatorWindow();

    const unsigned int x_size = 12;
    const unsigned int y_size = 8;

};

#endif // MEJIRODAI_H
