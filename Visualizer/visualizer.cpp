#include "visualizer.h"
#include "ui_visualizer.h"

 Visualizer::Visualizer(procon::Field& inp_field, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Visualizer),
    field(inp_field)
{
    ui->setupUi(this);

    next_grids = std::vector<std::vector<std::pair<int, int>>>(2, std::vector<std::pair<int,int>>(2, std::make_pair(-1, -1)));
    candidate = std::vector<std::vector<std::pair<int, int>>>(2, std::vector<std::pair<int,int>>(2, std::make_pair(-1, -1)));

    is_delete = std::vector<std::vector<int>>(2, std::vector<int>(2, 0));
}

Visualizer::~Visualizer()
{
    delete ui;
}

void Visualizer::setField(const procon::Field& inp_field, int now_turn, int max_t){
    candidate = std::vector<std::vector<std::pair<int, int>>>(2, std::vector<std::pair<int,int>>(2, std::make_pair(-1, -1)));
    field = inp_field;
    turn = now_turn;
    max_turn = max_t;
}

void Visualizer::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    unsigned int grid_x = field.getSize().first;
    unsigned int grid_y = field.getSize().second;

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

                if(!(field.getState(x_pos, y_pos).first == 0)){

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

        painter.setPen(QPen(QBrush(Qt::black),0.5));

        for(unsigned int team = 0; team < 2; ++team){
            for(unsigned int index = 0; index < 2; ++index){

                QColor paint_color = ( team == 0
                           ? team_color_a
                           : team_color_b);
                paint_color.setAlpha(128);

                if(index)
                    paint_color.setGreen(128);

                painter.setBrush(QBrush(paint_color));

                int pos_x = field.getAgents().at(team).at(index).first;
                int pos_y = field.getAgents().at(team).at(index).second;

                QString text = QString::fromStdString("agent" + std::to_string(index + 1));
                painter.setFont(QFont("Decorative", grid_size * 0.2, QFont::Thin)); // text font
                painter.drawEllipse(horizontal_margin + grid_size * (0.1 + pos_x), vertical_margin + grid_size * (0.1 + pos_y), 0.8 * grid_size, 0.8 * grid_size);
                painter.drawText(horizontal_margin + grid_size * (0.1 + pos_x), vertical_margin + grid_size * (0.2 + pos_y), text);

            }
        }
    };

    auto drawAgentMove = [&]{

        for(unsigned int team = 0; team < 2; ++team){
            for(unsigned int index = 0; index < 2; ++index){



                int pos_x = next_grids.at(team).at(index).first;
                int pos_y = next_grids.at(team).at(index).second;

                if(pos_x == -1)continue;

                QColor paint_color = ( team == 0
                           ? checked_color_a
                           : checked_color_b);

                paint_color.setAlpha(60);

                if(is_delete.at(team).at(index) || (field.getState(pos_x, pos_y).first == (team == 0 ? 2 : 1)))
                    paint_color.setAlpha(170);

                painter.setBrush(QBrush(paint_color));

                painter.drawRect(horizontal_margin + grid_size * (0.1 + pos_x), vertical_margin + grid_size * (0.1 + pos_y), 0.8 * grid_size, 0.8 * grid_size);
            }
        }


    };

    //manual時の移動候補表示
    auto drawCandidateMove = [&]{

        for(unsigned int team = 0; team < 2; ++team){
            for(unsigned int index = 0; index < 2; ++index){



                int pos_x = candidate.at(team).at(index).first;
                int pos_y = candidate.at(team).at(index).second;

                if(pos_x == -1 || candidate.at(team).at(index) == field.getAgent(team, index) )continue;

                QColor paint_color = ( team == 0
                           ? checked_color_a
                           : checked_color_b);

                paint_color.setAlpha(100);

                if(is_delete.at(team).at(index) || (field.getState(pos_x, pos_y).first == (team == 0 ? 2 : 1)))
                    paint_color.setAlpha(170);

                painter.setBrush(QBrush(paint_color));

                //角が取れた四角形らしいです
                painter.drawRoundRect(horizontal_margin + grid_size * (0.1 + pos_x), vertical_margin + grid_size * (0.1 + pos_y), 0.8 * grid_size, 0.8 * grid_size );
            }
        }


    };

    // 選択されたエージェントの移動可能先を塗る
    auto drawAroundAgent = [&] {

        // チームごとに配色を変える
        QColor paint_color = ( selected_agent.first == 0
                               ? checked_color_a
                               : checked_color_b);
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

    auto drawClickedGridWhenChangingMode = [&]{

       QColor paint_color = selected_grid_color;
       painter.setBrush(QBrush(paint_color));
       int draw_x = selected_to_change_grid.first;
       int draw_y = selected_to_change_grid.second;
       painter.drawRect(horizontal_margin + draw_x * grid_size, vertical_margin + draw_y * grid_size, grid_size, grid_size);

    };

    auto drawPoints = [&]{

        //とても汚いコピペコードで申し訳NASA
        QPoint side_0_point;
        side_0_point.setX(horizontal_margin);
        side_0_point.setY(window_height  - vertical_margin + grid_size * 1.3);

        painter.setFont(QFont("Decorative", grid_size * 0.6, QFont::Thin)); // text font

        QColor paint_color_0 = team_color_a;
        paint_color_0.setAlpha(100);

        painter.setPen(QPen(QBrush(paint_color_0), 0.3));

        std::vector<std::pair<int, int>> team_points(2);
        team_points.at(0) = field.getPoints(false).at(0);
        team_points.at(1) = field.getPoints(false).at(1);

        std::string side_0_value;
        side_0_value += std::to_string(team_points.at(0).first);
        side_0_value += " + ";
        side_0_value += std::to_string(team_points.at(0).second);
        side_0_value += " : ";
        side_0_value += std::to_string(team_points.at(0).first + team_points.at(0).second);

        painter.drawText(side_0_point,QString::fromStdString(side_0_value));

        QPoint side_1_point;
        side_1_point.setX(window_width - horizontal_margin - grid_size * 5);
        side_1_point.setY(window_height  - vertical_margin + grid_size * 1.3 + grid_size * 0.6);

        painter.setFont(QFont("Decorative", grid_size * 0.6, QFont::Thin)); // text font

        QColor paint_color_1 = team_color_b;
        paint_color_1.setAlpha(100);

        painter.setPen(QPen(QBrush(paint_color_1), 0.3));

        std::string side_1_value;
        side_1_value += std::to_string(team_points.at(1).first);
        side_1_value += " + ";
        side_1_value += std::to_string(team_points.at(1).second);
        side_1_value += " : ";
        side_1_value += std::to_string(team_points.at(1).first + team_points.at(1).second);

        painter.drawText(side_1_point,QString::fromStdString(side_1_value));
    };

    auto drawTurnCount = [&]{

        QPoint text_point;
        text_point.setX(horizontal_margin + (field.getSize().first - 2.5) * grid_size);
        text_point.setY(vertical_margin - 1.0 * grid_size);

        painter.setFont(QFont("Decorative", grid_size * 0.6, QFont::Thin)); // text font
        painter.setPen(QPen(QBrush(QColor(250, 80, 80 , 80)), 0.3));

        std::string str;
        str += std::to_string(turn);
        str += " / ";
        str += std::to_string(max_turn);

        painter.drawText(text_point,QString::fromStdString(str));
    };

    auto drawisEditMode = [&]{
        QPoint text_point;
        text_point.setX(horizontal_margin);
        text_point.setY(vertical_margin - 1.0 * grid_size);

        painter.setFont(QFont("Decorative", grid_size * 0.4, QFont::Thin)); // text font
        painter.setPen(QPen(QBrush(QColor(250, 80, 80, 80)), 0.3));


        std::string str;
        str = is_change_field_mode == true ? "EditMode" : "GameMode";

        std::string recul;
        if (!is_change_field_mode && is_recalcuration) recul = "Calcurating";
        painter.drawText(text_point,QString::fromStdString(str + " " + recul));

        is_recalcuration = false;
    };

    auto drawRegion = [&]{

        QFont text_font;
        text_font.setPixelSize(grid_size * 0.3);
        painter.setFont(text_font);
        QColor Red_team_color = team_color_a;
        QColor Blue_team_color = team_color_b;
        Red_team_color.setAlpha(100);
        Blue_team_color.setAlpha(100);

        std::bitset<288> region = field.getRegions();

        for(unsigned int x_pos = 0; x_pos < grid_x; ++x_pos)
            for(unsigned int y_pos = 0; y_pos < grid_y; ++y_pos){

                if(region[y_pos * 12 + x_pos]){
                QString text = QString::fromStdString( "R" );
                painter.setPen(QPen(Red_team_color));
                painter.drawText(horizontal_margin + grid_size * x_pos + (grid_size * 0.8), vertical_margin + grid_size * y_pos + ( grid_size * 0.9 ) , text);
                }
                if(region[y_pos*12 + x_pos + 144]){
                    QString text = QString::fromStdString( "B" );
                    painter.setPen(QPen(Blue_team_color));
                    painter.drawText(horizontal_margin + grid_size * x_pos + (grid_size * 0.1), vertical_margin + grid_size * y_pos + ( grid_size * 0.9 ) , text);
                }
            }


    };

    auto drawClickedGrid = [&] {

        QColor paint_color;
        std::pair<int, int> state = field.getState(clicked_grid_change.first, clicked_grid_change.second);
        if (state.first == 0) paint_color = background_color;
        else if (state.first == 1) paint_color = team_color_a;
        else paint_color = team_color_b;
        paint_color.setAlpha(200);

        int x_pos = clicked_grid_change.first;
        int y_pos = clicked_grid_change.second;

        painter.setBrush(QBrush(paint_color));
        painter.drawRect(horizontal_margin + grid_size * x_pos, vertical_margin + grid_size * y_pos, grid_size, grid_size);
    };

    auto drawClickedAgent = [&]{

        painter.setPen(QPen(QBrush(Qt::black),0.5));

        QColor paint_color = ( selected_agent.first == 0
                   ? team_color_a
                   : team_color_b);
        paint_color.setAlpha(200);

        painter.setBrush(QBrush(paint_color));

        int pos_x = selected_agent_grid.first;
        int pos_y = selected_agent_grid.second;

        painter.drawEllipse(horizontal_margin + grid_size * (0.1 + pos_x), vertical_margin + grid_size * (0.1 + pos_y), 0.8 * grid_size, 0.8 * grid_size);

    };

    // Gridの左右に番号をふる
    auto writeGridNumber = [&] {

        // フォントの設定
        QFont text_font;
        text_font.setPixelSize(grid_size * 0.5);
        painter.setFont(text_font);

        QColor color = Qt::black;
        color.setAlpha(150);
        painter.setPen(color);

        for (unsigned int x = 0; x < grid_x; x++) {
            painter.drawText(
                        QRectF(horizontal_margin + grid_size * x, vertical_margin - grid_size * 1.1, grid_size, grid_size),
                        Qt::AlignHCenter | Qt::AlignBottom,
                        QString::number(x+1)
                        );
            painter.drawText(
                        QRectF(horizontal_margin + grid_size * x, vertical_margin + grid_size * (grid_y + 0.1), grid_size, grid_size),
                        Qt::AlignHCenter | Qt::AlignTop,
                        QString::number(x+1)
                        );
        }

        for (unsigned int y = 0; y < grid_y; y++) {
            painter.drawText(
                        QRectF(horizontal_margin - grid_size * 1.1, vertical_margin + grid_size * y, grid_size, grid_size),
                        Qt::AlignRight | Qt::AlignVCenter,
                        QString::number(y+1)
                        );
            painter.drawText(
                        QRectF(horizontal_margin + grid_size * (grid_x + 0.1), vertical_margin + grid_size * y, grid_size, grid_size),
                        Qt::AlignLeft | Qt::AlignVCenter,
                        QString::number(y+1)
                        );
        }
    };

    drawBackGround();
    drawTiles();
    drawisEditMode();


    if(is_change_field_mode && is_selected_grid) drawClickedGridWhenChangingMode();

    // AutoModeでなくかつChaneModeではないとき

    if((auto_mode == false) && (change_mode == false) && !is_change_field_mode){
        drawAgentMove();
        drawCandidateMove();
        if (selected) drawAroundAgent();
    }

    if (clicked && !is_change_field_mode) {
        // クリックされたグリッド
        if (change_mode) drawClickedGrid();

        // クリックされたエージェント
        if (change_mode && selected) drawClickedAgent();
    }

    drawValues();
    drawAgents();

    drawPoints();

    drawTurnCount();
    drawRegion();

    writeGridNumber();
}

// メインウィンドウ内のマスをクリックしたときに行われるイベント
// エージェント：エージェントの移動可能先を表示
// エージェントの移動可能域内のマス：エージェントの移動を決定
void Visualizer::mousePressEvent(QMouseEvent *event)
{
    // マスのwindow上の座標を取得
    QPointF point = event->pos();

    if(auto_mode == false){

        // マスの範囲外をクリックしたら何もしない
        if ((point.x() < horizontal_margin) || (point.x() > window_width - horizontal_margin)
                        || (point.y() < vertical_margin) || (point.y() > window_height - vertical_margin)) {
            return;
        }

        //右クリックかどうか
        bool right_flag = (event->button() == Qt::RightButton);

        // クリックされたグリッド座標を保存
        std::pair<int, int> clicked_grid;

        // xを座標からマスへ
        clicked_grid.first = (point.x() - horizontal_margin) / grid_size;

        // yを座標からマスへ
        clicked_grid.second = (point.y() - vertical_margin) / grid_size;

        // 移動をを入力するエージェントが選ばれているか
        // ChangeModeのときは機能しない

        if(is_change_field_mode){
            selected_to_change_grid = clicked_grid;
            is_selected_grid = true;
            this->update();
        }

        if (!change_mode && selected && !is_change_field_mode) {

            // グリッドはエージェントの移動先に含まれているか
            checkClickGrid(clicked_grid, right_flag);

        }else if(selected && is_change_field_mode){
            //すでにagentがいたら早期return
            std::vector<std::vector<std::pair<int, int>>> agents = field.getAgents();
            for (int team = 0; team < 2; team++) {
                for (int agent = 0; agent < 2; agent++) {

                    // クリックされたマスとエージェントの位置が一致したら、チームとエージェントの番号を返す
                    if (clicked_grid == agents.at(team).at(agent))
                        return;
                }
            }

            setGridState(clicked_grid, selected_agent.first + 1);
            setAgentPos(selected_agent, clicked_grid);
            selected = false;
            is_moving_agent = true;

        }else {

            // クリックされたエージェントまたはマスを照合
            checkClickedAgent(clicked_grid);
        }

        if (change_mode) {

            clicked_grid_change = clicked_grid;
            clicked = true;

            update();

            // クリックされたGridのステータスをOperatorに表示
            emit selectChangeGrid(clicked_grid, selected);

        }
        if(!is_moving_agent && is_change_field_mode){
            is_changing_field_grid = true;

        }
        is_moving_agent = false;

    }
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
                // if (decided_agents.at(team).at(agent)) return;

                selected_agent.first = team;
                selected_agent.second = agent;

                // 移動を入力するエージェントのグリッド座標を保存
                selected_agent_grid = mass;

                // エージェントが選択されたことを記録
                selected = true;
                is_moving_agent = true;

                // 選択されたエージェントの移動可能範囲を表示
                this->update();
            }
        }
    }
}

// エージェントの移動先を決定
void Visualizer::checkClickGrid(std::pair<int, int> mass, bool right_flag)
{
    // クリックされた場所がエージェントの移動範囲に含まれているか確認
    if (((selected_agent_grid.first - 1) > mass.first) || ((selected_agent_grid.first + 1) < mass.first)
            || ((selected_agent_grid.second - 1) > mass.second) || ((selected_agent_grid.second + 1) < mass.second)) {

        // 選択を解除
        selected = false;

        // すでに登録されている場合リセット
        if (next_grids.at(selected_agent.first).at(selected_agent.second).first != -1) {

            next_grids.at(selected_agent.first).at(selected_agent.second) = std::make_pair(-1, -1);
            confirm_count--;

        }

        // 初期化
        selected_agent = std::make_pair(INT_MAX, INT_MAX);
        selected_agent_grid = std::make_pair(INT_MAX, INT_MAX);

        this->update();

        return;
    }


    //もう既に選択されている場合は値を増やさない
    if(next_grids.at(selected_agent.first).at(selected_agent.second).first == -1)
        ++confirm_count;

    // 移動先を記録
    next_grids.at(selected_agent.first).at(selected_agent.second) = mass;

    is_delete.at(selected_agent.first).at(selected_agent.second) = right_flag;


    // エージェントの移動先が決定済みであることを記録
    // decided_agents.at(selected_agent.first).at(selected_agent.second) = true;

    // エージェントの選択を解除
    selected = false;

    // windowの描きかえ
    this->update();


    if(confirm_count == 4){

        confirm_count = 0;

        std::vector<std::vector<std::pair<int,int>>> return_val = std::move(next_grids);
        std::vector<std::vector<int>> return_delete_flag = std::move(is_delete);

        next_grids = std::vector<std::vector<std::pair<int, int>>>(2,std::vector<std::pair<int,int>>(2,std::make_pair(-1,-1)));
        candidate = std::vector<std::vector<std::pair<int, int>>>(2,std::vector<std::pair<int,int>>(2,std::make_pair(-1,-1)));

        is_delete = std::vector<std::vector<int>>(2, std::vector<int>(2, 0));
        emit nextMove(return_val, return_delete_flag);
    }
}

void Visualizer::slotAutoMode(bool value){
    auto_mode = value;
}

void Visualizer::candidateMove(const std::vector<std::vector<std::pair<int, int> > > &inp_vec){
    candidate = inp_vec;
    this->update();
}

void Visualizer::setChangeMode(bool value) {

    change_mode = value;

    // もろもろを初期化
    clicked = false;
    selected = false;
    confirm_count = 0;

    if (!change_mode) {

    }

}

void Visualizer::getData(const std::pair<int, int> data, const bool agent) {

    if (agent) {
        // エージェントの場所とそのタイル状況を変更
        if (selected) {
            field.setAgent(selected_agent.first, selected_agent.second, data.first, data.second);
            field.setState(clicked_grid_change.first, clicked_grid_change.second, selected_agent.first + 1);

//            std::pair<int, int> agent = field.getAgent(0,0);

            selected = false;
        }

    } else {

        // グリッドのタイル状況と点数を変更
        field.setState(clicked_grid_change.first, clicked_grid_change.second, data.first);
        field.setGridValue(clicked_grid_change.first, clicked_grid_change.second, data.second);
    }

    update();
}

procon::Field Visualizer::getField()
{
    return field;
}

void Visualizer::setTurns(const std::pair<int, int> turns)
{
    turn = turns.first;
    field.setTurnCount(turn);
    max_turn = turns.second;
    field.setFinalTurn(max_turn);
}

void Visualizer::setAgentPos(const std::pair<int, int> agent, const std::pair<int, int> pos)
{
    field.setAgent(agent.first, agent.second, pos.first, pos.second);
    is_selected_grid = false;
    update();

    emit sendAgentPos(agent, pos);
}

void Visualizer::setGridState(const std::pair<int, int> grid, const int state)
{
    field.setState(grid.first, grid.second, state);
    is_selected_grid = false;
    update();

    emit sendGridState(grid, state);
}

void Visualizer::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_C || event->key() == Qt::Key_E){
        if(is_change_field_mode){
            is_change_field_mode = false;
            is_selected_grid = false;
            selected = false;
            update();
        }else{
            is_change_field_mode = true;
            update();
        }
    }

    if((event->key() == Qt::Key_0 || event->key() == Qt::Key_3) && is_changing_field_grid && !is_moving_agent){
        setGridState(selected_to_change_grid, 0);
    }else if(event->key() == Qt::Key_1 && is_changing_field_grid &&	!is_moving_agent){
        setGridState(selected_to_change_grid, 1);
    }else if(event->key() == Qt::Key_2 && is_changing_field_grid && !is_moving_agent){
        setGridState(selected_to_change_grid, 2);
    } else if ((event->key() == Qt::Key_Escape) && selected && !is_change_field_mode) {
        // 選択したエージェントの移動入力を解除
        checkClickGrid(std::make_pair(-1,-1), false);
    } else if ((event->key() == Qt::Key_R) && !is_change_field_mode) {
        // 現時点でのfieldで再計算
        is_recalcuration = true;
        emit sendRecalculation(std::make_pair(field.getTurnCount(), field.getFinalTurn()));
    } else if ((event->key() == Qt::Key_Escape) && is_change_field_mode && is_selected_grid) {
        // 選択されているマスやエージェントを解除
        is_selected_grid = false;
        is_changing_field_grid = false;
        selected = false;
        this->repaint();
    }


}
