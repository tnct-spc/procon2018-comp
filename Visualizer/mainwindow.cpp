#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    field = std::make_shared<procon::Field>(grid_x, grid_y, max_val, min_val);
    manager = std::make_shared<GameManager>();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){

    QPainter painter(this);

    window_width = this->width();
    window_height = this->height();

    grid_size = std::min(1.0 * window_width / ( (margin * 2) + grid_x), 1.0 * window_height / ( (margin * 2) + grid_y));

    horizontal_margin = (window_width - grid_size * grid_x) / 2;
    vertical_margin = (window_height - grid_size * grid_y) / 2;

    auto drawBackGround = [&]{

        painter.setBrush(QBrush(background_color));
        painter.drawRect(0, 0, window_width, window_height);

        painter.setBrush(QBrush(grid_color));
        painter.drawRect(horizontal_margin, vertical_margin, window_width - horizontal_margin * 2, window_height - vertical_margin * 2);


        painter.setPen(QPen(QBrush(Qt::black),0.5));

        for(unsigned int count = 1; count < grid_x; ++count)
            painter.drawLine(horizontal_margin + grid_size * count, vertical_margin, horizontal_margin + grid_size * count, window_height - vertical_margin);

        for(unsigned int count = 1; count < grid_y; ++count)
            painter.drawLine(horizontal_margin,vertical_margin + grid_size * count, window_width - horizontal_margin, vertical_margin + grid_size * count);

    };

    auto drawPieces = [&]{
        for(unsigned int x_pos = 0; x_pos < grid_x; ++x_pos)
            for(unsigned int y_pos = 0; y_pos < grid_y; ++y_pos){

                if(field->isPlaced(x_pos, y_pos) == 0)
                    painter.setBrush(QBrush(grid_color));
                else
                    painter.setBrush(QBrush(((field->getState(x_pos,y_pos)).first == 1
                                            ? team_color_a
                                            : team_color_b
                                            )));
                painter.drawRect(horizontal_margin + grid_size * x_pos, vertical_margin + grid_size * y_pos, grid_size, grid_size);
            }

    };

    drawBackGround();
    drawPieces();

}
