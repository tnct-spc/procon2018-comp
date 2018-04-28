#include "communicator.h"
#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Communicator *communicator = new Communicator();
    std::cout << "end";
    return a.exec();
}
