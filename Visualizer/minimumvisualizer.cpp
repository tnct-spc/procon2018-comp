#include "minimumvisualizer.h"
#include "ui_minimumvisualizer.h"

MinimumVisualizer::MinimumVisualizer(std::pair<int,int> size, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MinimumVisualizer),
    size(size)
{
    ui->setupUi(this);
}

MinimumVisualizer::~MinimumVisualizer()
{
    delete ui;
}

void MinimumVisualizer::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    int size_x, size_y;
    std::tie(size_x, size_y) = size;

    QPainter painter(this);

    int window_width = this->width();
    int window_height = this->height();

    double margin = 1.5;

    int grid_size = std::min(1.0 * window_width / ( (margin * 2) + size_x), 1.0 * window_height / ( (margin * 2) + size_y));

    int horizontal_margin = (window_width - grid_size * size_x) / 2;
    int vertical_margin = (window_height - grid_size * size_y) / 2;

    auto drawBackGround = [&]{

        /*
        painter.setBrush(QBrush(background_color));
        painter.drawRect(0, 0, window_width, window_height);

        painter.setBrush(QBrush(grid_color));
        painter.drawRect(horizontal_margin, vertical_margin, window_width - horizontal_margin * 2, window_height - vertical_margin * 2);
        */

        painter.setPen(QPen(QBrush(Qt::black),0.5));

        for(unsigned int count = 0; count <= size_x; ++count)
            painter.drawLine(horizontal_margin + grid_size * count, vertical_margin, horizontal_margin + grid_size * count, window_height - vertical_margin);

        for(unsigned int count = 0; count <= size_y; ++count)
            painter.drawLine(horizontal_margin,vertical_margin + grid_size * count, window_width - horizontal_margin, vertical_margin + grid_size * count);

    };

    auto drawRoute = [&]{
        for(auto it = route.begin(), it2 = std::next(route.begin()); it2 != route.end(); ++it, ++it2){

            painter.setBrush(QBrush(QColor(255 - values.at((*it).first).at((*it).second), 255, 255)));

            if(it == route.begin())
                painter.drawEllipse(horizontal_margin + grid_size * ((*it).first + 0.2), vertical_margin + grid_size * ((*it).second + 0.2),
                                    0.6 * grid_size, 0.6 * grid_size
                                    );
            else
                painter.drawEllipse(horizontal_margin + grid_size * ((*it).first + 0.3), vertical_margin + grid_size * ((*it).second + 0.3),
                                    0.4 * grid_size, 0.4 * grid_size
                                    );
            painter.drawLine(horizontal_margin + grid_size * ((*it).first + 0.5), vertical_margin + grid_size * ((*it).second + 0.5),
                             horizontal_margin + grid_size * ((*it2).first + 0.5), vertical_margin + grid_size * ((*it2).second + 0.5)
                             );
        }

        auto it = std::prev(route.end());
        painter.drawEllipse(horizontal_margin + grid_size * ((*it).first + 0.3), vertical_margin + grid_size * ((*it).second + 0.3),
                                0.4 * grid_size, 0.4 * grid_size
                                );
    };

    auto drawValues = [&]{
        for(unsigned int pos_x = 0; pos_x < size_x; ++pos_x)
            for(unsigned int pos_y = 0; pos_y < size_y; ++pos_y){
                painter.setBrush(QBrush(QColor(255 - values.at(pos_x).at(pos_y), 255, 255)));
                painter.drawRect(horizontal_margin + grid_size * pos_x, vertical_margin + grid_size * pos_y, grid_size, grid_size);
            }
    };

    drawBackGround();

    if(!values.empty())
        drawValues();

    if(!route.empty())
        drawRoute();

}

void MinimumVisualizer::setSize(std::pair<int,int> siz){
    size = siz;
}

void MinimumVisualizer::setRoute(std::list<std::pair<int,int>>& rout){
    route = rout;
}

void MinimumVisualizer::setValues(std::vector<std::vector<int>>& vec){
    values = vec;
}
