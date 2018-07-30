#ifndef CIPHERCARDS_H
#define CIPHERCARDS_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QDir>

namespace Ui {
class CipherCards;
}

class CipherCards : public QWidget
{
    Q_OBJECT

public:
    explicit CipherCards(QWidget *parent = nullptr);
    ~CipherCards();

    // 移動先が決まったら更新
    void updata(std::vector<std::pair<int, int>> move);

    // 暗号を設定
    void setCipher();

private:
    Ui::CipherCards *ui;

    // カード
    enum CardType {
        Heart,
        Dia,
        Club,
        Spade,
    };

    // 暗号
    struct Cipher {
        CardType mark;
        int num;
    };

    std::vector<std::vector<Cipher>> ciphers = {
        {{Heart, 1}, {Heart, 2}, {Heart, 3}, {Heart, 4}, {Heart, 5}, {Heart, 6}, {Heart, 7}, {Heart, 8}, {Heart, 9}},
        {{Dia, 1}, {Dia, 2}, {Dia, 3}, {Dia, 4}, {Dia, 5}, {Dia, 6}, {Dia, 7}, {Dia, 8}, {Dia, 9}},
    };

    // PathのStringを作成
    QString makePath(Cipher card);
};

#endif // CIPHERCARDS_H
