#include "testmultiplevisualizer.h"

TestMultipleVisualizer::TestMultipleVisualizer(QObject *parent)
    : QAbstractItemModel(parent)
{
    window = std::make_shared<MultipleVisualizer>();

    connect(this, &TestMultipleVisualizer::sendCsv, window.get(), &MultipleVisualizer::setCsv);

    pathes.first = QFileDialog::getOpenFileName().toStdString();
    pathes.second = QFileDialog::getOpenFileName().toStdString();

    window->setCsv(std::make_pair(pathes.first, pathes.second));
    window->show();

    window->update();
    window->repaint();

}

void TestMultipleVisualizer::run()
{

    for(int count = 0; count < turn_max; ++count){
        std::cout << "count : " << count << std::endl;
        std::ostringstream ost;
        ost << std::setfill('0') << std::setw(6) << count;
        pathes.first = pathes.first.substr(0, pathes.first.length() - 6) + ost.str();

        std::ostringstream ost2;
        ost2 << std::setfill('0') << std::setw(6) << count;
        pathes.second = pathes.second.substr(0, pathes.second.length() - 6) + ost.str();

        emit sendCsv(std::make_pair(pathes.first, pathes.second));


        window->update();
        window->repaint();

        std::string a = QFileDialog::getOpenFileName().toStdString();


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
