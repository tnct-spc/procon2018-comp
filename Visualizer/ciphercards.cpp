#include "ciphercards.h"
#include "ui_ciphercards.h"

CipherCards::CipherCards(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CipherCards)
{
    ui->setupUi(this);
}

CipherCards::~CipherCards()
{
    delete ui;
}

void CipherCards::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    // 画像をresize
    QPixmap pix1 = QPixmap(image1);
    ui->cards1Label->setPixmap(pix1.scaledToWidth(ui->cards1Label->width()));

    QPixmap pix2 = QPixmap(image2);
    ui->cards2Label->setPixmap(pix2.scaledToWidth(ui->cards2Label->width()));
}

void CipherCards::updata(std::vector<std::pair<int, int>> move)
{
    const std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    const std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::vector<unsigned long int> agent_move;

    // どの方面に動いているか判別
    for (unsigned long int agent = 0; agent < 2; agent++) {
        for (unsigned long int i = 0; i < 9; i++) {
            if ((move.at(agent).first == x_list.at(i)) && (move.at(agent).second == y_list.at(i))) {
                agent_move.push_back(i);
            }
        }
    }

    // Pathを作成
    image1 = makePath(ciphers.at(0).at(agent_move.at(0)));
    image2 = makePath(ciphers.at(1).at(agent_move.at(1)));

    // Labelに画像を貼り付ける
    QPixmap pix1 = QPixmap(image1);
    ui->cards1Label->setPixmap(pix1.scaledToWidth(ui->cards1Label->width()));

    QPixmap pix2 = QPixmap(image2);
    ui->cards2Label->setPixmap(pix2.scaledToWidth(ui->cards2Label->width()));
}

void CipherCards::setCipher(unsigned long int agent, unsigned long int pos, Cipher cip)
{
    ciphers.at(agent).at(pos) = cip;
}

QString CipherCards::makePath(Cipher card)
{
    QString image_name = "./../../procon2018-comp/Visualizer/cards/";

    // 記号
    switch (card.mark) {
        case Heart:
            image_name += "h";
            break;
        case Dia:
            image_name += "d";
            break;
        case Club:
            image_name += "c";
            break;
        case Spade:
            image_name += "s";
            break;
    }

    // 数（頭悪そう）
    QString number;
    if (card.num < 10) {
        image_name += "0";
        image_name += number.setNum(card.num);
    } else {
        image_name += number.setNum(card.num);
    }

    image_name += ".gif";

    return image_name;
}
