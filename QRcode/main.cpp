#include "qrcode.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QRCode w;
    w.show();
    return a.exec();
}
