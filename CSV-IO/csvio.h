#ifndef CSVIO_H
#define CSVIO_H

#include <QMainWindow>
#include "field.h"

class csvIO : public QMainWindow
{
    Q_OBJECT

public:
    csvIO(QWidget *parent = 0);
    ~csvIO();

    procon::Field importField(std::string path);
    void exportField(procon::Field field);
};

#endif // CSVIO_H
