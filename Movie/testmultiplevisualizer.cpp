#include "testmultiplevisualizer.h"

TestMultipleVisualizer::TestMultipleVisualizer(QObject *parent)
    : QAbstractItemModel(parent)
{
}

void TestMultipleVisualizer::run()
{

    dock = std::make_shared<ProgresDock>();
    dock->show();

    pathes.first = QFileDialog::getOpenFileName().toStdString();
    pathes.second = QFileDialog::getOpenFileName().toStdString();

    procon::Field f1 = procon::CsvIo::importField(pathes.first);
    procon::Field f2 = procon::CsvIo::importField(pathes.second);

    Visualizer* vis1;
    Visualizer* vis2;

    dock->addVisuCSV(pathes);

    MultipleVisualizer *window = new MultipleVisualizer;

    window->show();

    for(int count = 0; count < turn_max; ++count){
        std::ostringstream ost;
        ost << std::setfill('0') << std::setw(6) << count;
        pathes.first = pathes.first.substr(0, pathes.first.length() - 6) + ost.str();

        std::ostringstream ost2;
        ost2 << std::setfill('0') << std::setw(6) << count;
        pathes.second = pathes.second.substr(0, pathes.second.length() - 6) + ost.str();

        dock->addVisuCSV(pathes);

        f1 = procon::CsvIo::importField(pathes.first);
        f2 = procon::CsvIo::importField(pathes.second);

        vis1 = new Visualizer(f1);
        vis2 = new Visualizer(f2);
        window->setVisualizers(std::vector<Visualizer*>({vis1, vis2}));
        window->update();
        window->repaint();

        wait_simulator.wait(&mtx, 1000);
    }

    std::cout << "finish" << std::endl;
}

QVariant TestMultipleVisualizer::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex TestMultipleVisualizer::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex TestMultipleVisualizer::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int TestMultipleVisualizer::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int TestMultipleVisualizer::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant TestMultipleVisualizer::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
