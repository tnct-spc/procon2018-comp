#include "mejirodai.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    std::cout << "main: " << std::this_thread::get_id() << std::endl;
    QApplication a(argc, argv);
    Mejirodai w;
    w.show();

    return a.exec();
}
