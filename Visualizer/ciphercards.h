#ifndef CIPHERCARDS_H
#define CIPHERCARDS_H

#include <iostream>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QDir>
#include <QResizeEvent>
#include <QGridLayout>

namespace Ui {
class CipherCards;
}

namespace procon{
    // カード
    enum CardType {
        Heart,
        Dia,
        Club,
        Spade,
        Joker,
        Error = -1,
    };

    // 暗号
    struct Cipher {
        CardType mark;
        int num;
    };

    // 0~53で渡された数字を対応するカードに変更
    Cipher changeIntToCipher(int card);

    // PathのStringを作成
    QString makePath(procon::Cipher card);


};

class CipherCards : public QWidget
{
    Q_OBJECT

public:
    explicit CipherCards(QWidget *parent = nullptr);
    ~CipherCards();

    // 移動先が決まったら更新
    void updata(std::vector<std::pair<int, int>> move);
    
    // カードを表示
    void drawCards(std::vector<std::vector<procon::Cipher>> cards);

private:
    Ui::CipherCards *ui;
    void resizeEvent(QResizeEvent *event);

    std::vector<std::vector<procon::Cipher>> ciphers = {
        {{procon::Heart, 1}, {procon::Heart, 2}, {procon::Heart, 3}, {procon::Heart, 4}, {procon::Heart, 5}, {procon::Heart, 6}, {procon::Heart, 7}, {procon::Heart, 8}, {procon::Heart, 9}},
        {{procon::Dia, 1}, {procon::Dia, 2}, {procon::Dia, 3}, {procon::Dia, 4}, {procon::Dia, 5}, {procon::Dia, 6}, {procon::Dia, 7}, {procon::Dia, 8}, {procon::Dia, 9}},
    };

    // 暗号を設定
    void setCipher(unsigned long int agent, unsigned long int pos, procon::Cipher cip);

    // Path
    std::vector<std::vector<QString>> images;
};


#endif // CIPHERCARDS_H
