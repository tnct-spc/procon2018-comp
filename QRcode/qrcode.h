#ifndef QRCODE_H
#define QRCODE_H

#include <iostream>
#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <zbar.h>

class QRCode{

public:
    QRCode();
    ~QRCode();
    std::string decodeQRcode();
};

#endif // QRCODE_H
