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
