#ifndef PROGRESDOC_H
#define PROGRESDOC_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include"field.h"

namespace Ui {
class ProgresDoc;
}

class ProgresDoc : public QWidget
{

    Q_OBJECT

public:

    explicit ProgresDoc(QWidget *parent = 0);
    ~ProgresDoc();
    void addAnswer(procon::Field const& field);

private:

    Ui::ProgresDoc *ui;
    std::vector<procon::Field> fields;

public slots:

signals:

};

#endif // PROGRESDOC_H
