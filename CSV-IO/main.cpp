#include "csvio.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    csvIO w;
    w.show();

    return a.exec();
}
