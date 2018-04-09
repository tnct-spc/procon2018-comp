#ifndef MEJIRODAI_H
#define MEJIRODAI_H

#include <QMainWindow>

namespace Ui {
class Mejirodai;
}

class Mejirodai : public QMainWindow
{
    Q_OBJECT

public:
    explicit Mejirodai(QWidget *parent = 0);
    ~Mejirodai();

private:
    Ui::Mejirodai *ui;
};

#endif // MEJIRODAI_H
