#include "visualizer.h"
#include "ui_visualizer.h"

Visualizer::Visualizer(procon::Field& inp_field, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Visualizer),
    field(inp_field)
{
    ui->setupUi(this);
    grid_x = field.getSize().first;
    grid_y = field.getSize().second;

}

Visualizer::~Visualizer()
{
    delete ui;
}

void Visualizer::setField(procon::Field& inp_field){
    field = inp_field;
}

void Visualizer::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

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

    // 選択されたエージェントの移動可能先を塗る
    auto drawAroundAgent = [&] {

        // チームごとに配色を変える
        QColor paint_color = ( selected_agent.first == 0
                               ? team_color_a
                               : team_color_b);
        paint_color.setAlpha(100);

        painter.setBrush(QBrush(paint_color));

        // フィールドをはみ出さない範囲で、エージェントの移動可能領域を描く
        for (int x = -1; x < 2; x++) {

            int draw_x = selected_agent_grid.first + x;

            if ((draw_x >= 0) && ((unsigned int)draw_x < grid_x)) {

                for (int y = -1; y < 2; y++) {

                    int draw_y = selected_agent_grid.second + y;

                    if ((draw_y >= 0) && ((unsigned int)draw_y < grid_y)) {

                        painter.drawRect(horizontal_margin + draw_x * grid_size, vertical_margin + draw_y * grid_size, grid_size, grid_size);
                    }
                }
            }
        }

    };

    drawBackGround();
    drawTiles();

    if (selected) drawAroundAgent();

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

    } else {

        // クリックされたエージェントまたはマスを照合
        checkClickedAgent(clicked_grid);
    }

//    std::vector<std::vector<std::pair<int, int>>> box = getNextAgents();
}

// クリックされたエージェントまたはマスを照合
void Visualizer::checkClickedAgent(std::pair<int, int> mass)
{
    // Fieldから現在のエージェントの位置を取得
    std::vector<std::vector<std::pair<int, int>>> agents = field.getAgents();

    // クリックされたマスにエージェントがいるのか確認
    for (int team = 0; team < 2; team++) {
        for (int agent = 0; agent < 2; agent++) {

            // クリックされたマスとエージェントの位置が一致したら、チームとエージェントの番号を返す
            if (mass == agents.at(team).at(agent)) {

                // クリックされたエージェントがすでに変更済みなら何もしない
                if (decided_agents.at(team).at(agent)) return;

                selected_agent.first = team;
                selected_agent.second = agent;

                // 移動を入力するエージェントのグリッド座標を保存
                selected_agent_grid = mass;

                // エージェントが選択されたことを記録
                selected = true;

                // 選択されたエージェントの移動可能範囲を表示
                this->update();
            }
        }
    }
}

// エージェントの移動先を決定
bool Visualizer::checkClickGrid(std::pair<int, int> mass)
{
    // クリックされた場所がエージェントの移動範囲に含まれているか確認
    if (((selected_agent_grid.first - 1) > mass.first) || ((selected_agent_grid.first + 1) < mass.first)
            || ((selected_agent_grid.second - 1) > mass.second) || ((selected_agent_grid.second + 1) < mass.second)) {
        return false;
    }

    // 移動先を記録
    next_grids.at(selected_agent.first).at(selected_agent.second) = mass;

    // エージェントの移動先が決定済みであることを記録
    decided_agents.at(selected_agent.first).at(selected_agent.second) = true;

    // エージェントの選択を解除
    selected = false;

    // windowの描きかえ
    this->update();

    return true;
}

// 決定されたエージェントの移動先を返す
std::vector<std::vector<std::pair<int, int>>> Visualizer::getNextAgents()
{
    // decided_agentsの初期化. Return用のベクターづくり
    std::vector<std::vector<std::pair<int, int>>> next_positions;

    // すべてのエージェントが次の移動先を決定されているか確認
    // 決定されていなかったら空の配列を返す
    for(int team = 0; team < 2; team++) {
        for (int agent = 0; agent < 2; agent++) {
            if (!decided_agents.at(team).at(agent)) {
                return next_positions;
            }
        }
    }

    for(int team = 0; team < 2; team++) {

        std::vector<std::pair<int, int>> agent_grid;

        for (int agent = 0; agent < 2; agent++) {

            decided_agents.at(team).at(agent) = false;
            agent_grid.push_back(next_grids.at(team).at(agent));
        }

        next_positions.push_back(agent_grid);
    }

    return next_positions;
}
