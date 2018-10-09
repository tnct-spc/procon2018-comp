#include <QApplication>
#include "multiplevisualizer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MultipleVisualizer m;
    m.show();

    return a.exec();
}
