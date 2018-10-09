#include <QApplication>
#include "testmultiplevisualizer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    TestMultipleVisualizer m;
    m.run();

    return a.exec();
}
