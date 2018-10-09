#include "testmultiplevisualizer.h"

TestMultipleVisualizer::TestMultipleVisualizer(QObject *parent)
    : QAbstractItemModel(parent)
{
}

void TestMultipleVisualizer::run()
{
//    std::shared_ptr<procon::Field> field1 = std::make_shared<procon::Field>(x_size, y_size, 16, -16, true);
//    std::shared_ptr<procon::Field> field2 = std::make_shared<procon::Field>(x_size, y_size, 16, -16, true);
//    field1->setFinalTurn(turn_max);
//    field2->setFinalTurn(turn_max);
//    std::shared_ptr<Visualizer> vis1 = std::make_shared<Visualizer>(*field1);
//    std::shared_ptr<Visualizer> vis2 = std::make_shared<Visualizer>(*field2);

    std::string path = QFileDialog::getOpenFileName().toStdString();
//    procon::Field field1(procon::CsvIo::importField(path));

    std::shared_ptr<procon::Field> field1 = std::make_shared<procon::Field>(procon::CsvIo::importField(path));

    path = QFileDialog::getOpenFileName().toStdString();
//    procon::Field field2(procon::CsvIo::importField(path));

    std::shared_ptr<procon::Field> field2 = std::make_shared<procon::Field>(procon::CsvIo::importField(path));

//    std::shared_ptr<Visualizer> vis1 = std::make_shared<Visualizer>(*field1);
//    std::shared_ptr<Visualizer> vis2 = std::make_shared<Visualizer>(*field2);
    Visualizer* vis1 = new Visualizer(*field1);
    Visualizer* vis2 = new Visualizer(*field2);

    std::vector<Visualizer *> visualizers;
    visualizers.push_back(vis1);
    visualizers.push_back(vis2);

    MultipleVisualizer *window = new MultipleVisualizer;
    window->setVisualizers(visualizers);
    window->show();

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
