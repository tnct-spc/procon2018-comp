#ifndef TESTMULTIPLEVISUALIZER_H
#define TESTMULTIPLEVISUALIZER_H

#include <QAbstractItemModel>
#include <visualizer.h>
#include <multiplevisualizer.h>
#include <field.h>
#include <QFileDialog>
#include "csvio.h"
#include <QWaitCondition>
#include <QMutex>

#include <iomanip>
#include "progresdock.h"

class TestMultipleVisualizer : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TestMultipleVisualizer(QObject *parent = nullptr);

    void run();

    std::pair<std::string, std::string> pathes;

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:

    QMutex mtx;
    QWaitCondition wait_simulator;


    std::shared_ptr<ProgresDock> dock;

    const int turn_max = 30;
};

#endif // TESTMULTIPLEVISUALIZER_H
