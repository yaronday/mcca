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

#include "Headers/color_grid.hpp"

ColorGrid::ColorGrid(MatFileHandler &handler):
    mfh(handler), n(1), m(1), maxSize(1), maxColor(1),
    minVal(INT_MAX), maxVal(INT_MIN), filename("") {}

void ColorGrid::init(vector<vector<int>> &mat) {
    n = static_cast<int>(mat.size());
    m = static_cast<int>(mat[0].size());
    maxColor = 1;
    maxSize = 1;
}

bool ColorGrid::isValid(int row, int col, int n, int m) {
    return (row >= 0 && row < n && col >= 0 && col < m);
}

// update filename member 
void ColorGrid::setFileName(const string &path, vector<int> &matParams) {
    ostringstream oss;
    oss << string(mfh.destpath) << "/" << mfh.updateStrByIntVect(matParams);
    filename = path.empty() ?  oss.str() : path;
}

string ColorGrid::updatePath(const string &filename, int maxSize, int maxColor, 
                             const string &algo, bool txt) {
    ostringstream suffix;
    suffix << "_out_max" << maxSize << "_c" << maxColor << "_" << algo;
    return fGenPath(filename, suffix.str(), txt);
}

string ColorGrid::updatePath(const string &filename, int maxSize, 
                             const string &algo, bool txt) {
    ostringstream suffix;
    suffix << "_out_max" << maxSize << "_" << algo << "_extra_info";
    return fGenPath(filename, suffix.str(), txt);
}

string ColorGrid::updatePath(const string &filename, int colorCode,
                             int minRow, int minCol, int maxRow, 
                             int maxCol, const string &algo,
                             bool txt) {
    ostringstream suffix, notification;
    suffix << "_out_cropped_" << algo << "_color" << colorCode << "_"
           << "TL(" << minRow << "," << minCol << ")"
           << "_BR(" << maxRow << "," << maxCol << ")";
    notification << "\nRectangular boundaries for " << algo << " cropped max region (coordinates of original matrix):\n"
        << "Top Left: (" << minRow << ", " << minCol << "), "
        << "Bottom Right: (" << maxRow << ", " << maxCol << ").\n";
    formatTxt(notification, LIGHT_MAGENTA);
    return fGenPath(filename, suffix.str(), txt);
}

// Column alignment, assuming a single char delimiter (e.g. blankspace, comma)   
int ColorGrid::alignCol(int &col) {
    return (col % 2) ? 2 * col + 1 : 2 * col;
}

void ColorGrid::paintResultsArea(vector<vector<int>> &matrix,
                                 bool colors,
                                 function<bool(int, int)> cellCondition) const {
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < m; c++) {
            int currValue = matrix[r][c];
            string currValueStr = to_string(currValue);

            if (cellCondition(r, c)) {
                if (colors)
                    formatTxt(currValueStr, getColorFromKey(currValue), " ");
                else
                    formatTxt(string(1, getSymbolFromKey(currValue)), DEFAULT_COLOR, " ");
            }
            else {
                formatTxt(currValueStr, DEFAULT_COLOR, " ");
            }
        }
        cout << endl;
    }
}

//calc bounding box or minimum bounding rectangle (MBR) 
void ColorGrid::calcBoundingBox(int &minRow, int &maxRow, 
                                int &minCol, int &maxCol,
                                int n, int m, 
                                const function<bool(int, int)> &cellCondition) {
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < m; c++) {
            if (cellCondition(r, c)) {
                minRow = min(minRow, r);
                maxRow = max(maxRow, r);
                minCol = min(minCol, c);
                maxCol = max(maxCol, c);
            }
        }
    }
}

void ColorGrid::updateValueStats(int value) {
    minVal = min(value, minVal);
    maxVal = max(value, maxVal);
}

void ColorGrid::displayMaxSize(const string &algo) const {
    ostringstream oss;
    oss << "Max connected colors (" << algo << "): " << maxSize << "\n";
    formatTxt(oss, LIGHT_CYAN);
}

void ColorGrid::fWriteCropped(const string &filename,
                              int n, int m,
                              const function<bool(int, int)> &cellCondition,
                              const function<char(int, int)> &getCellValue,
                              const string &algo) {

    int minRow = n, maxRow = -1, minCol = m, maxCol = -1;
    calcBoundingBox(minRow, maxRow, minCol, maxCol, n, m, cellCondition);

    // Construct a unique file name based on the bounding box coordinates
    string filepath = updatePath(filename, maxColor, minRow, alignCol(minCol), maxRow, alignCol(maxCol), algo);
    fWriteResults(filepath, n, m, cellCondition, getCellValue, true, minRow, maxRow, minCol, maxCol);
}

void ColorGrid::updateMfhFilename(const string &filepath) {
    mfh.matInfo = { n, m, minVal, maxVal };
    setFileName(filepath, mfh.matInfo);
    mfh.filename = this->filename;
}
