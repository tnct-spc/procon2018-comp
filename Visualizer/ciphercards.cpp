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

    // agent1
    // ラベルの数。layoutの文は引く
    int num = ui->agent1Widget->children().size() - 1;

    for (int i = 0; i < num; i++) {
        // 保存していた画像を貼り直す
        QPixmap pix = QPixmap(images.at(0).at(i));

        // objectNameからLabelを探す
        QString object_name = "image0_" + QString::number(i);
        QLabel *label = ui->agent1Widget->findChild<QLabel *>(object_name);

        // Widgetの横幅に合わせる
        label->setPixmap(pix.scaledToWidth(ui->agent1Widget->width() / num - 5));
    }

    // agent2
    // ラベルの数。layoutの文は引く
    num = ui->agent2Widget->children().size() - 1;

    for (int i = 0; i < num; i++) {
        // 保存していた画像を貼り直す
        QPixmap pix = QPixmap(images.at(0).at(i));

        // objectNameからLabelを探す
        QString object_name = "image1_" + QString::number(i);
        QLabel *label = ui->agent2Widget->findChild<QLabel *>(object_name);

        // Widgetの横幅に合わせる
        label->setPixmap(pix.scaledToWidth(ui->agent2Widget->width() / num - 5));
    }
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

    // test用
    std::vector<std::vector<Cipher>> to_draw;

    std::vector<Cipher> cards1;
    cards1.push_back(ciphers.at(0).at(agent_move.at(0)));
    cards1.push_back(ciphers.at(0).at((agent_move.at(0) + 3) % 9));

    std::vector<Cipher> cards2;
    cards2.push_back(ciphers.at(1).at(agent_move.at(1)));

    to_draw.push_back(cards1);
    to_draw.push_back(cards2);

    drawCards(to_draw);
}

void CipherCards::setCipher(unsigned long int agent, unsigned long int pos, Cipher cip)
{
    ciphers.at(agent).at(pos) = cip;
}

CipherCards::Cipher CipherCards::changeIntToCipher(int card)
{
    Cipher cip;

    // 範囲外の数字が来たらerrorを返す
    if ((card < 0) || (card > 53)) {
        std::cout << "argument is not true." << std::endl;
        return cip = {Error, -1};
    }

    cip.mark = static_cast<CardType>(card / 13);
    cip.num = (card - cip.mark * 13) % 13;

    return cip;
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
        case Joker:
            image_name += "x";
            break;
        case Error:
            std::cout << "cardType is not true." << std::endl;
            return NULL;
    }

    // 数（頭悪そう）

    // 範囲外ならNULLを返す
    if ((card.num < 0) || (card.num > 13)) {
        std::cout<< "cardNumber is not true." << std::endl;
        return NULL;
    }

    QString number;
    if (card.num < 9) {
        image_name += "0";
        image_name += number.setNum(card.num+1);
    } else {
        image_name += number.setNum(card.num+1);
    }

    image_name += ".gif";

    return image_name;
}

void CipherCards::drawCards(std::vector<std::vector<Cipher>> cards)
{
    for (unsigned int agent = 0; agent < 2; agent++) {
        // そのエージェントのカード数を取得
        unsigned int card_num = cards.at(agent).size();

        // エージェントごとにWidgetを切り替える
        // かっこ悪いので、あとでどうにかする
        QWidget *wid;
        if (agent == 0) wid = ui->agent1Widget;
        else wid = ui->agent2Widget;

        // path保存用のvector
        std::vector<QString> paths;

        // 各WidgetはQGridLayoutを使用
        QGridLayout *layout = new QGridLayout(wid);

        // marginをなくす
        layout->setMargin(0);
        layout->setSpacing(0);

        for (unsigned int num = 0; num < card_num; num++) {
            // Pathを作成
            QString path = makePath(cards.at(agent).at(num));
            paths.push_back(path);

            // 新しいラベル
            QLabel *label = new QLabel;

            // ラベルにobjectnameを設定する
            QString object_name = "image" + QString::number(agent) + "_" + QString::number(num);
            label->setObjectName(object_name);

            // Labelに画像を貼り付ける
            QPixmap pix = QPixmap(path);
            label->setPixmap(pix.scaledToWidth(wid->width() / card_num));

            // 各Widget内に設置
            layout->addWidget(label, 0, num, Qt::AlignLeft);
        }

        // エージェントのカードのPathを保存
        images.push_back(paths);
    }
}
