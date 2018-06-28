#ifndef QRCODE_H
#define QRCODE_H

//#include <bits/stdc++.h>
#include <QWidget>
//#include <QTimer>
//#include <string>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgcodecs/imgcodecs.hpp>
//#include "opencv/cv.h"
//#include "opencv/highgui.h"
//#include <zbar.h>

namespace Ui {
class QRCode;
}

class QRCode : public QWidget
{
    Q_OBJECT
public:
    explicit QRCode(QWidget *parent = 0);
    ~QRCode();
    //std::string getdata();
/*
private slots:
    void on_btnOk_clicked();
    void ProcessFrame();
    void use_drawRectangle();
    void showresult(QString code);
    void clearAll();
    void inputdata(QString code);
*/
private:
    Ui::QRCode *ui;
//    int ScanIDCard(IplImage *src);
   // std::string read_only_data;
};

#endif // QRCODE_H
