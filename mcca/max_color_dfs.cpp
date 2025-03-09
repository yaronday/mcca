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

#pragma warning(push)
#pragma warning(disable : 26495)  // Disable uninitialized variable warning (which occured despite a proper init within base class)

#include "Headers/max_color_dfs.hpp"

DfsColorGrid::DfsColorGrid(MatFileHandler &handler) : ColorGrid(handler) {}

int DfsColorGrid::calcMaxConnectedColor() {
    return 0;
}

int DfsColorGrid::calcMaxConnectedColor(vector<vector<int>> &mat, 
                                        bool paint,
                                        bool colors, 
                                        const string &filepath, 
                                        bool crop) {
 
    init(mat);

    int curr_max = 1;
    vector<pair<int, int>> xy_coord_list;

    vector<vector<bool>> checked(n, vector<bool>(m, false));
    set<pair<int, int>> coord_set;

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < m; col++) {
            int currValue = mat[row][col];
            updateValueStats(currValue);
            if (!checked[row][col]) {
                vector<pair<int, int>> curr_xy_coord_list;
                curr_max = dfs(mat, currValue, row, col, checked, curr_xy_coord_list);
                if (maxSize < curr_max) {
                    maxSize = curr_max;
                    xy_coord_list = std::move(curr_xy_coord_list);
                    maxColor = currValue;
                }
            }
        }
    }

    for (const auto &coord : xy_coord_list) {
        coord_set.insert(coord);
    }

    updateMfhFilename(filepath);

    auto cellCond = [&coord_set](int r, int c) -> bool {
        return coord_set.count(make_pair(r, c)) > 0;
        };

    auto getCellValue = [&](int r, int c) -> char {
        return itoc(maxColor);
        };

    if (paint)
        paintResultsArea(mat, colors, cellCond);
    else {
        if (crop) {
            fWriteCropped(filename,
                          n, m, 
                          cellCond,
                          getCellValue,
                          algo
                         );
        }
        else {
            const string filepath = updatePath(filename, maxSize, maxColor, algo);
            fWriteAll(filepath,
                      n, m,
                      cellCond,
                      getCellValue
                     );
        }
    } 
    
    displayMaxSize(algo);

    return maxSize;
}

int DfsColorGrid::dfs(vector<vector<int>> &mat, int curr_value, 
                      int row, int col, vector<vector<bool>> &checked,
                      vector<pair<int, int>> &coord_list) {
    if (!isValid(row, col, n, m) || checked[row][col] || mat[row][col] != curr_value) {
        return 0;
    }
    checked[row][col] = true;
    coord_list.emplace_back(row, col);
    int res = 1;
    for (int i = 0; i < dr.size(); i++) {
        int new_row = row + dr[i];
        int new_col = col + dc[i];
        res += dfs(mat, curr_value, new_row, new_col, checked, coord_list);
    }
    return res;
}

#pragma warning(pop)