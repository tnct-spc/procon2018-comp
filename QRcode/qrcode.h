#ifndef QRCODE_H
#define QRCODE_H

#include <QWidget>

#include <iostream>
#include <string>
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <zbar.h>

using namespace cv;
using namespace zbar;

namespace Ui{
    class QRCode;
}

class QRCode : public QWidget{

    Q_OBJECT

public:
    explicit QRCode(QWidget *parent = 0);
    ~QRCode();

private slots:
    void decodeQRcode();
    void on_Start_clicked();
    void on_Exit_clicked();
    void clearAll();

private:
    Ui::QRCode *ui;
    std::string read_only_data;
};

#endif // QRCODE_H
