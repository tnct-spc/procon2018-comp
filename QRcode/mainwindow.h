#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <zbar.h>

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
    void on_btnOk_clicked();
    void ProcessFrame();
    void use_drawRectangle();
    void showresult(QString code);
    void clearAll();
    void translateToDoubleArray();

private:
    Ui::MainWindow *ui;
    int ScanIDCard(IplImage *src);
};

#endif // MAINWINDOW_H
