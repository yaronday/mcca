/*
 * Copyright (C) 2025 Yaron Dayan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Attribution Notice:
 * The original author, Yaron Dayan, must be credited in all derivative works.
 */

#pragma once

#include "color_grid.hpp"
#include "../../common/headers/matfile_handler.hpp"

using namespace std;

class DfsColorGrid : public ColorGrid {
private:
    const string algo = "DFS";

public:

    DfsColorGrid() = delete;

    explicit DfsColorGrid(MatFileHandler& handler);
      
    virtual int calcMaxConnectedColor() override;

    int calcMaxConnectedColor(vector<vector<int>> &mat, 
                              bool paint = true, 
                              bool colors = true, 
                              const string &filepath = "",
                              bool crop = false);

protected:
    int dfs(vector<vector<int>> &mat, int curr_value,
            int row, int col, vector<vector<bool>> &checked,
            vector<pair<int, int>> &coord_list);
};