#include "visualizer.h"
#include "ui_visualizer.h"

Visualizer::Visualizer(procon::Field inp_field, QWidget *parent) :
    QWidget(parent),
    field(inp_field),
    ui(new Ui::Visualizer)
{
    ui->setupUi(this);
    grid_x = field.getSize().first;
    grid_y = field.getSize().second;


}

Visualizer::~Visualizer()
{
    delete ui;
}

void Visualizer::setField(procon::Field inp_field){
    field = inp_field;
}

void Visualizer::paintEvent(QPaintEvent *event){

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

                if(field.isPlaced(x_pos, y_pos) == true){

                    QColor paint_color = ( field.getState(x_pos, y_pos).first == 1
                                           ? team_color_a
                                           : team_color_b);
                    paint_color.setAlpha(64);

                    painter.setBrush(QBrush(paint_color));
                    painter.drawRect(horizontal_margin + grid_size * x_pos, vertical_margin + grid_size * y_pos, grid_size, grid_size);
                }
            }

    };

    auto drawValues = [&]{

        std::vector<std::vector<int>> field_value = field.getValue();
        QFont text_font;
        text_font.setPixelSize(grid_size * 0.5);
        painter.setFont(text_font);
        painter.setPen(QPen(font_color));

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

                int pos_x = field.getAgents().at(team).at(index).first;
                int pos_y = field.getAgents().at(team).at(index).second;

                painter.drawEllipse(horizontal_margin + grid_size * (0.1 + pos_x), vertical_margin + grid_size * (0.1 + pos_y), 0.8 * grid_size, 0.8 * grid_size);

            }
        }
    };

    drawBackGround();
    drawTiles();
    drawValues();
    drawAgents();

}

// メインウィンドウ内のマスをクリックしたときに行われるイベント
// エージェント：エージェントの移動可能先を表示
// エージェントの移動可能域内のマス：エージェントの移動を決定
void Visualizer::mousePressEvent(QMouseEvent *event)
{
    // マスのwindow上の座標を取得
    QPointF point = event->pos();

    // マスの範囲外をクリックしたら何もしない
    if ((point.x() < horizontal_margin) || (point.x() > window_width - horizontal_margin)
            || (point.y() < vertical_margin) || (point.y() > window_height - vertical_margin)) {
        return;
    }

    // クリックされたグリッド座標を保存
    std::pair<int, int> clicked_grid;

    // xを座標からマスへ
    clicked_grid.first = (point.x() - horizontal_margin) / grid_size;

    // yを座標からマスへ
    clicked_grid.second = (point.y() - vertical_margin) / grid_size;

    // 移動をを入力するエージェントが選ばれているか
    if (selected) {

        // グリッドはエージェントの移動先に含まれているか
        bool checked = checkClickGrid(clicked_grid);

        // エージェントの移動先を入力したら、選択を解除
        if (checked) {
            selected = false;
            std::cout << "OK" << std::endl;
        }

        std::cout << "NO" << std::endl;

    } else {

        // クリックされたエージェントまたはマスを照合
        std::pair<int, int> clicked = checkClickedAgent(clicked_grid);
    }

}

// クリックされたエージェントまたはマスを照合
std::pair<int, int> Visualizer::checkClickedAgent(std::pair<int, int> mass)
{
    // return用のpair
    std::pair<int, int> checked_agent = std::make_pair(2,2);

    // Fieldから現在のエージェントの位置を取得
    std::vector<std::vector<std::pair<int, int>>> agents = field.getAgents();

    // クリックされたマスにエージェントがいるのか確認
    for (int team = 0; team < 2; team++) {
        for (int agent = 0; agent < 2; agent++) {

            // クリックされたマスとエージェントの位置が一致したら、チームとエージェントの番号を返す
            if (mass == agents.at(team).at(agent)) {

                checked_agent.first = team;
                checked_agent.second = agent;

                // 移動を入力するエージェントのグリッド座標を保存
                selected_agent = mass;

                // 移動そ入力されたエージェントが選択されたことを記録
                selected = true;

                return checked_agent;
            }
        }
    }

    return checked_agent;
}

// エージェントの移動先を決定
bool Visualizer::checkClickGrid(std::pair<int, int> mass)
{
    // クリックされた場所がエージェントの移動範囲に含まれているか確認
    // 含まれていない場合、リセット
    if (((selected_agent.first - 1) > mass.first) || ((selected_agent.first + 1) < mass.first)
            || ((selected_agent.second - 1) > mass.second) || ((selected_agent.second + 1) < mass.second)) {
        return false;
    }

    return true;
}
