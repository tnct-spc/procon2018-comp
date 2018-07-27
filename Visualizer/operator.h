#ifndef OPERATOR_H
#define OPERATOR_H

#include "field.h"
#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QStackedWidget>

namespace Ui {
class Operator;
}

class Operator : public QWidget
{
    Q_OBJECT

public:
    explicit Operator(QWidget *parent = 0);
    ~Operator();

    // Turnの値をセット
    void setTurns(const int now_turn, const int max_turn);

signals:

    // 変更を送る
    void pushChange(const std::pair<int, int> out_data, const bool out_bool);

    // Endが押されたら送る
    void pushEnd(const std::pair<int, int> turns);

public slots:

    // Visualizerが押されたとき、Operatorの表示を変更
    void changeDataDisplay(const std::pair<int, int> inp_data, const bool inp_bool);

private:
    Ui::Operator *ui;

    // ボタンが押されたらsignalを送る
    void sendPushEnd();
    void sendPushChange();

    // 扱っているのがAgentならtrue
    bool agent = false;

    std::pair<int, int> data;

};

#endif // OPERATOR_H
