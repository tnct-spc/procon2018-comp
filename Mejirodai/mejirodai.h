#ifndef MEJIRODAI_H
#define MEJIRODAI_H

#include "gamemanager.h"
#include <thread>
#include <memory>
#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>


namespace Ui {
class Mejirodai;
}

class Mejirodai : public QMainWindow
{
    Q_OBJECT

public:
    explicit Mejirodai(QWidget *parent = 0);
    ~Mejirodai();

private slots:

    void on_selectMyAlgorithmBox_currentIndexChanged(int index);

    void on_selectOpponentAlgorithmBox_currentIndexChanged(int index);

private:
    Ui::Mejirodai *ui;

    std::shared_ptr<GameManager> manager;
    std::shared_ptr<Visualizer> visualizer;

    void RunManagerSimulation();
    void goNextState();
    void goPrevState();

    const unsigned int x_size = 12;
    const unsigned int y_size = 8;

};

#endif // MEJIRODAI_H
