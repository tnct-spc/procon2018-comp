#include "csvio.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

/*
 * ***CSVの形式について***
 * データの前に以下の種類識別番号が挿入されます。
 * 
 * field及びvalueは、data[0][0], data[0][1], ... , data[0][grid_y], ... , data[grid_x][grid_y]の順で挿入されます。
 * agentsは、agents[0][0]_x, agents[0][0]_y, agents[0][1]_x, agents[0][1]_y, ... , agents[1][1]_yの順で挿入されます。
 *
 * ***種類識別番号***
 * 0: name:FUJISAN    value:{turnCount, grid_x, grid_y}
 * 1: name:TAKAOSAN   value:{field(state)}
 * 2: name:MITAKESAN  value:{agents_x, agents_y}
 * 3: name:HAKUTOUSAN value:{value}
 *
 * ***例***
 * 0, 30, 3, 3, 1, 0, 0, 0, ... , 2, 0, 0, 0, 3, 3, 0, 3, 3, 3, 0, 0, 0, ... , 0
 */

procon::Field CsvIo::importField(std::string path)
{
    /** ImportData **/
    std::ifstream input(path);
    if(!input.is_open()) {
        std::cerr << "The file does not exist." << std::endl;
        exit(1);
    }
    std::string line_buffer = "";
    std::getline(input, line_buffer);
    std::string data;
    std::istringstream line_stream(line_buffer);
    //read mode
    std::getline(line_stream, data, ',');
    if(std::stoi(data) != 0) {
      std::cerr << "The format of the file is incorrect." << std::endl;
      exit(1);
    }
    //read mode 0
    std::getline(line_stream, data, ',');
    int grid_x = std::stoi(data);
    std::getline(line_stream, data, ',');
    int grid_y = std::stoi(data);
    procon::Field fields(grid_x, grid_y);

    while(std::getline(input, line_buffer)) {
        //read mode
        std::string point_buffer = "";
        std::istringstream line_stream(line_buffer);
        std::getline(line_stream, point_buffer, ',');
        int mode = std::stoi(point_buffer);

        if(mode == 1) {
            std::vector<std::vector<int>> field_data;
            //format the vector field_data[grid_x][grid_y]
            field_data = std::vector<std::vector<int>>(grid_x, std::vector<int>(grid_y, 0));
            for(int i = 0; i < grid_x; ++i) {
                for(int j = 0; j < grid_y; ++j) {
                    std::string data;
                    std::getline(line_stream, data, ',');
                    field_data[i][j] = std::stoi(data);
                }
            }
            fields.setStates(field_data);
        }

        if(mode == 2) {
            for(int i = 0; i < 2; ++i) {
                for(int j = 0; j < 2; ++j) {
                    std::string data;
                    std::getline(line_stream, data, ',');
                    int x = std::stoi(data);
                    std::getline(line_stream, data, ',');
                    int y = std::stoi(data);
                    fields.setAgent(i, j, x, y);
                }
            }
        }

        if(mode == 3) {
            std::vector<std::vector<int>> value_data;
            //format the vector value_data[grid_x][grid_y]
            value_data = std::vector<std::vector<int>>(grid_x, std::vector<int>(grid_y, 0));
            for(int i = 0; i < grid_x; ++i) {
                for(int j = 0; j < grid_y; ++j) {
                    std::string data;
                    std::getline(line_stream, data, ',');
                    value_data[i][j] = std::stoi(data);
                }
            }
            fields.setValue(value_data);
        }
    }

    return fields;

}

void CsvIo::exportField(procon::Field data, std::string path)
{
    /** GetData **/
    std::pair<int, int> grid = data.getSize();
    std::vector<std::vector<int>> fields = data.getField();
    std::vector<std::vector<std::pair<int, int>>> agents = data.getAgents();
    std::vector<std::vector<int>> values = data.getValue();

    /** ExportData **/
    std::ofstream output(path);

    auto exportFujisan = [&]() {
        output << FUJISAN << "," << grid.first << "," << grid.second << std::endl;
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
