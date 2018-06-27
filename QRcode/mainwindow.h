#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QTimer>
#include <string>
#include "qrcode.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_btnStart_clicked();
    void on_btnExit_clicked();
    void Quit();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
