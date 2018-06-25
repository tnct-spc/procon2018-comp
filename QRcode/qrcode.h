#ifndef QRCODE_H
#define QRCODE_H


#include <QMainWindow>
#include <QTimer>

#include <string>

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <zbar.h>

namespace Ui {
class QRCode;
}

class QRCode : public QWidget
{
    Q_OBJECT

public:
    explicit QRCode(QWidget *parent = 0);
    ~QRCode();

private slots:
    void on_btnOk_clicked();
    void ProcessFrame();
    void use_drawRectangle();
    void showresult(QString code);
    void clearAll();
    void InputData(QString code);
    void translateToDoubleArray();

private:
    Ui::QRCode *ui;
    int ScanIDCard(IplImage *src);
    std::string read_only_data;
};

#endif // QRCODE_H
