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

#include <vector>
#include <set>
#include <unordered_set>
#include <functional>

#include "../../common/headers/matfile_handler.hpp"
#include "../../common/headers/helpers.hpp"

using namespace std;


class ColorGrid {
protected:
    MatFileHandler &mfh;

    bool isValid(int row, int col, int n, int m);

    string updatePath(const string &filename, int maxSize,
        int maxColor, const string &algo,
        bool txt = EN_TXT);

    string updatePath(const string &filename, int maxSize,
        const string &algo,
        bool txt = EN_TXT);

    string updatePath(const string &filename, int colorCode,
        int minRow, int minCol, int maxRow, int maxCol,
        const string &algo,
        bool txt = EN_TXT);

    //calc bounding box or minimum bounding rectangle (MBR)
    void calcBoundingBox(int &minRow, int &maxRow, int &minCol, int &maxCol,
        int n, int m, const function<bool(int, int)> &cellCondition);

    int alignCol(int &col);

public:

    int n, m, maxSize, maxColor, minVal, maxVal;
    string filename;
    vector<int> dr = { 1, -1, 0, 0, 1, -1, -1, 1 };
    vector<int> dc = { 0, 0, 1, -1, -1, -1, 1, 1 };

    virtual int calcMaxConnectedColor() = 0;
    
    // Default constructor deleted to enforce passing MatFileHandler
    ColorGrid() = delete;

    // Constructor with MatFileHandler initialization
    explicit ColorGrid(MatFileHandler &handler);

    void init(vector<vector<int>> &mat);
    void setFileName(const string &path, vector<int> &matrixParams);

    void paintResultsArea(vector<vector<int>> &matrix,
                          bool colors,
                          function<bool(int, int)> cellCondition) const;

    void updateValueStats(int value);

    void displayMaxSize(const string &) const;

    template <typename ConditionFunc, typename ValueFunc>
    void fWriteAll(const string &filepath,
                   int n, int m,
                   ConditionFunc cellCondition,
                   ValueFunc getCellValue
                   ) {
        fWriteResults(filepath, n, m, cellCondition, getCellValue, false);
    }

    void fWriteCropped(const string &filename,
                       int n, int m,
                       const function<bool(int, int)> &cellCondition,
                       const function<char(int, int)> &getCellValue,
                       const string &algo);

    template <typename ConditionFunc, typename ValueFunc>
    void fWriteResults(const string &filepath,
                       int n, int m,
                       ConditionFunc cellCondition,
                       ValueFunc getValue,
                       bool cropped,
                       int minRow = 0, int maxRow = -1,
                       int minCol = 0, int maxCol = -1) {

        if (cropped) {
            maxRow = (maxRow == -1) ? n - 1 : maxRow;
            maxCol = (maxCol == -1) ? m - 1 : maxCol;
        }
        else {
            minRow = 0;
            maxRow = n - 1;
            minCol = 0;
            maxCol = m - 1;
        }

        auto buffer = mfh.fillBuffer(cellCondition, getValue, minRow, maxRow, minCol, maxCol);
        fWrite(buffer, filepath, DEFAULT_INFO, true);
    }

    //update MatFileHandler filename member
    void updateMfhFilename(const string &filepath);
};







