#ifndef CIPHERCARDS_H
#define CIPHERCARDS_H

#include <QWidget>

namespace Ui {
class CipherCards;
}

class CipherCards : public QWidget
{
    Q_OBJECT

public:
    explicit CipherCards(QWidget *parent = nullptr);
    ~CipherCards();

private:
    Ui::CipherCards *ui;
};

#endif // CIPHERCARDS_H
