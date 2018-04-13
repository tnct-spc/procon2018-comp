#include "csvio.h"
#include <fstream>

/*
 * ***CSVの形式について***
 * データの前に以下の種類識別番号が挿入されます。
 * field及びvalueは、data[0][0], data[0][1], ... , data[0][grid_y], ... , data[grid_x][grid_y]の順で挿入されます。
 * agentsは、agents[0][0]_x, agents[0][0]_y, agents[0][1]_x, agents[0][1]_y, ... , agents[1][1]_yの順で挿入されます。
 *
 * ***種類識別番号***
 * 0: name:FUJISAN    value:{turnCount, grid_x, grid_y}
 * 1: name:TAKAOSAN   value:{field}
 * 2: name:MITAKESAN  value:{agents_x, agents_y}
 * 3: name:HAKUTOUSAN value:{value}
 *
 * ***例***
 * 0, 30, 3, 3, 1, 0, 0, 0, ... , 2, 0, 0, 0, 3, 3, 0, 3, 3, 3, 0, 0, 0, ... , 0
 */

csvIO::csvIO() {

}

procon::Field csvIO::importField(std::string path)
{

}

void csvIO::exportField(procon::Field data, std::string path)
{
    /** getData **/
    int trun_count = data.getTurnCount();
    std::pair<int, int> grid = data.getSize();
    std::vector<std::vector<int>> fields = data.getField();
    std::vector<std::vector<std::pair<int, int>>> agents = data.getAgents();
    std::vector<std::vector<int>> values = data.getValue();

    /** ExportData **/
    std::ofstream output(path);

    auto exportFujisan = [&]() {
        output << FUJISAN << trun_count << grid.first << grid.second << std::endl;
    };

    auto exportTakaosan = [&]() {
        output << TAKAOSAN;
        for(auto axis : fields) {
            for(auto field_data : axis) output << "," << field_data;
        }
        output << std::endl;
    };

    auto exportMitakesan = [&]() {
        output << MITAKESAN;
        for(auto axis : agents) {
            for(auto agent_data : axis) output << "," << agent_data.first
                                               << "," << agent_data.second;
        }
        output << std::endl;
    };

    auto exportHakutousan = [&]() {
        output << HAKUTOUSAN;
        for(auto axis : values) {
            for(auto value_data : axis) output << "," << value_data;
        }
        output << std::endl;
    };

    exportFujisan();
    exportTakaosan();
    exportMitakesan();
    exportHakutousan();
}
