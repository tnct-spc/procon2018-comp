#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    manager = std::make_shared<GameManager>(grid_x, grid_y);
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

    auto drawTiles = [&]{
        for(unsigned int x_pos = 0; x_pos < grid_x; ++x_pos)
            for(unsigned int y_pos = 0; y_pos < grid_y; ++y_pos){

                if(manager->getField().isPlaced(x_pos, y_pos) == true){

                    QColor paint_color = ( manager->getField().getState(x_pos, y_pos).first == 1
                                           ? team_color_a
                                           : team_color_b);
                    paint_color.setAlpha(64);

                    painter.setBrush(QBrush(paint_color));
                    painter.drawRect(horizontal_margin + grid_size * x_pos, vertical_margin + grid_size * y_pos, grid_size, grid_size);
                }
            }

    };

    auto drawValues = [&]{

        std::vector<std::vector<int>> field_value = manager->getField().getValue();
        QFont text_font;
        text_font.setPixelSize(grid_size * 0.5);
        painter.setFont(text_font);

        painter.setBrush(QBrush(font_color));
        for(unsigned int x_pos = 0; x_pos < grid_x; ++x_pos)
            for(unsigned int y_pos = 0; y_pos < grid_y; ++y_pos){

                QString text = QString::fromStdString( std::to_string(field_value.at(x_pos).at(y_pos)) );
                painter.drawText(horizontal_margin + grid_size * x_pos + (grid_size * 0.2), vertical_margin + grid_size * y_pos + ( grid_size * 0.7 ) , text);
            }

    };

    auto drawAgents = [&]{
        for(unsigned int team = 0; team < 2; ++team){
            for(unsigned int index = 0; index < 2; ++index){

                QColor paint_color = ( team == 0
                           ? team_color_a
                           : team_color_b);
                paint_color.setAlpha(128);

                painter.setBrush(QBrush(paint_color));

                int pos_x = manager->getField().getAgents().at(team).at(index).first;
                int pos_y = manager->getField().getAgents().at(team).at(index).second;

                painter.drawEllipse(horizontal_margin + grid_size * (0.1 + pos_x), vertical_margin + grid_size * (0.1 + pos_y), 0.8 * grid_size, 0.8 * grid_size);

            }
        }
    };

    drawBackGround();
    drawTiles();
    drawValues();
    drawAgents();

}
