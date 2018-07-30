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

    ui->mark1->setNum(ciphers.at(0).at(agent_move.at(0)).mark);
    ui->num1->setNum(ciphers.at(0).at(agent_move.at(0)).num);
    ui->mark2->setNum(ciphers.at(1).at(agent_move.at(1)).mark);
    ui->num2->setNum(ciphers.at(1).at(agent_move.at(1)).num);
}

void CipherCards::setCipher()
{

}
