#ifndef MEJIRODAI_H
#define MEJIRODAI_H

#include "gamemanager.h"
#include "csvio.h"
#include "binaryio.h"

#include <thread>
#include <memory>
#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QCheckBox>
#include <QThread>
#include <QObject>

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
    void exportFieldtoBinary();
    void searchThreadTerminator();

    const unsigned int x_size = -1;
    const unsigned int y_size = 8;

    // Runボタンが押されたか
    bool runMode = false;
};

#endif // MEJIRODAI_H
