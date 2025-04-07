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

#include "headers/max_color_uf.hpp"

UnionFindColorGrid::UnionFindColorGrid(MatFileHandler &handler) : ColorGrid(handler) {
    maxColorSet.clear();       // track max color codes
    colorRegionsMap.clear();  // track regions for each color
}

int UnionFindColorGrid::calcMaxConnectedColor() {
    return 0;
}

int UnionFindColorGrid::calcMaxConnectedColor(vector<vector<int>> &mat,
                                              bool paint, bool colors,
                                              const string &filepath,
                                              bool crop,
                                              bool visualizerEn) {
    init(mat);

    UnionFind uf(n * m);

    int currentRegionSize = 1;

    // To track the pairs already united
    unordered_set<pair<int, int>, hash_pair> loggedPairs;

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < m; col++) {
            int currCell = row * m + col;
            int currValue = mat[row][col];
            updateValueStats(currValue);
            processAdjCells(row, col, mat, currCell, currValue, uf, loggedPairs);
            int root = uf.find(currCell);
            currentRegionSize = uf.size[root];
            if (currentRegionSize > maxSize) {
                maxSize = currentRegionSize;              
                maxColorSet = { currValue };  // Start a new set with the color code of the new max-size root
                colorRegionsMap.clear();  // Clear old regions map when we find a new maxSize
                colorRegionsMap[currValue].insert(root);
            }
            else if (currentRegionSize == maxSize) {
                // Add color code to the set if it's not already there
                maxColorSet.insert(currValue);
                colorRegionsMap[currValue].insert(root);  // Add the root to the regions map
            }
        }
    }

    maxColor = *maxColorSet.begin();
    
    updateMfhFilename(filepath);

    if (visualizerEn) {
        if (max(m, n) <= VIS_MAT_THR){
            ostringstream oss;
            oss << "Visualizer activated!\n";
            formatTxt(oss, LIGHT_MAGENTA);
            visualizeUF(uf, maxSize, filename);
        }
        else
            formatTxt(getErrMsg(ErrCode::GRAPHVIZ_MAT_LIMIT, ErrorContext()), LIGHT_MAGENTA);
    }

    bool extraInfoFile = false; 

    auto cellCond = [&](int r, int c) -> int {
        return uf.getSize(r * m + c) == maxSize;
    };

    auto getCellValue = [&](int r, int c) -> char {
        return itoc(mat[r][c]);
    };

    if (paint) {
        paintResultsArea(mat, colors, cellCond);                   
    }
    else {
        if (crop) {
            fWriteCropped(filename,
                          uf,
                          mat, 
                          n, m,
                          cellCond,
                          getCellValue
                         );
        }
        else {
            const string fpath = updatePath(filename, maxSize, maxColor, algo);
            fWriteAll(fpath,
                      n, m,
                      cellCond,
                      getCellValue
                     );
        }
        extraInfoFile = true;
    }

    displayMaxSize(algo);

    if (maxColorSet.size() > 1)
        notifyMaxColorRegions(filename, maxSize, maxColorSet, colorRegionsMap, extraInfoFile);

    return maxSize;
}

void UnionFindColorGrid::visualizeUF(UnionFind &uf,
                                     int maxSize,
                                     const string &filename,
                                     const string &ext,
                                     bool show) {
    // Prerequisites: Graphviz, configured as system env variable.

    ostringstream filePath, dotFilePath, outFilePath;

    filePath << fRemoveExt(filename) << "_out_max" << maxSize << "_c" << maxColor;

    string filePathStr = filePath.str();

    outFilePath << filePathStr << "_UFtree." << ext; 

    dotFilePath << filePathStr + "_UFtree.dot";

    string dotFilePathStr = dotFilePath.str();

    string outFilePathStr = outFilePath.str();

    ofstream dotFile(dotFilePathStr);
    dotFile << "digraph UnionFindTree {\n";

    // Node attributes based on their color
    for (int i = 0; i < uf.parent.size(); ++i) {
        int root = uf.find(i);
        const char *color = getRegionColor(root, colorRegionsMap);
        const char *fontcolor = (!strcmp(color, "blue")) ? "white" : "black";

        dotFile << "    " << i << " [style=filled, fillcolor=\"" << color
                << "\", fontcolor=\"" << fontcolor << "\"];\n";
    }

    for (int i = 0; i < uf.parent.size(); ++i) {
        int root = uf.find(i);
        if (root != i) {
            dotFile << "    " << i << " -> " << root << ";\n";
        }
    }

    dotFile << "}\n";
    dotFile.close();

    // Render an image from a dot file
    ostringstream render_cmd;
    render_cmd << "dot -T" << ext << " \"" << dotFilePathStr << "\" -o \"" << outFilePathStr << "\"";
    string renderCmd = render_cmd.str();
    try {
        if (system(renderCmd.c_str()) ) {
            throw runtime_error("Render command failed - \n" + renderCmd);
        }
    }
    catch (const exception &e) {
        handleError(ErrCode::GENERIC_EXCEPTION, e);
        exit(1);
    }

    if (show) {
        ostringstream open_cmd;
        open_cmd << "start \"\" \"" << outFilePathStr << "\"";
        string openCmd = open_cmd.str();
        try { 
            if (system(openCmd.c_str())) {
                throw runtime_error("Failed to execute start command: " + string(openCmd));
            }
        }
        catch (const exception &e) {
            handleError(ErrCode::GENERIC_EXCEPTION, e);
            exit(1);
        }
    }
}

// Notify extra max colors regions (unique method for UF algorithm)
// Useful for multiple equally-sized max regions with different color codes 
void UnionFindColorGrid::notifyMaxColorRegions(const string &filename, int maxSize,
                                               const unordered_set<int> &maxColorSet,
                                               const unordered_map<int, 
                                               unordered_set<int>> &colorRegionsMap, 
                                               bool extraFile) {
    ostringstream oss;
    oss << "Color codes with max size of " << maxSize << ":\n";
    // Iterate through all color codes in maxColorSet
    for (int colorCode : maxColorSet) {
        const auto &regions = colorRegionsMap.at(colorCode);  // Retrieve regions for color code
        oss << "Color code " << colorCode << (regions.size() > 1 ? " (multiple regions)\n" : "\n");
    }
    formatTxt(oss, LIGHT_CYAN);

    if (extraFile)
        fWrite(oss.str(), 
               updatePath(filename, maxSize, algo),
               "additional info file", true);
}

const char *UnionFindColorGrid::getRegionColor(int root, 
                                               const unordered_map<int, unordered_set<int>>& colorRegionsMap) const {
    for (const auto &[colorCode, regions] : colorRegionsMap) {
        if (regions.find(root) != regions.end()) {
            return getGraphvizColorFromKey(colorCode);
        }
    }
    return "white";
}

void UnionFindColorGrid::processAdjCells(int row, int col,
                                         vector<vector<int>> &matrix,
                                         int currCell,                                     
                                         int currValue,                                     
                                         UnionFind &uf,
                                         unordered_set<pair<int, int>, hash_pair> &loggedPairs
                                        ) {
    for (int i = 0; i < dr.size(); i++) {
        int newRow = row + dr[i];
        int newCol = col + dc[i];
        if (isValid(newRow, newCol, n, m) && matrix[newRow][newCol] == currValue) {
            int adjCell = newRow * m + newCol;
            pair<int, int> cellPair =
                currCell < adjCell ? make_pair(currCell, adjCell) : make_pair(adjCell, currCell);
            if (loggedPairs.find(cellPair) == loggedPairs.end()) {
                uf.unite(currCell, adjCell);
                loggedPairs.insert(cellPair);
            }
        }
    }
}

//For --crop option (only write each bounding box of equally-sized max region to separate file)
void UnionFindColorGrid::fWriteCropped(const string &filename,
                                       UnionFind &uf,
                                       vector<vector<int>> &mat,
                                       int n, int m,
                                       const function<bool(int, int)> &cellCondition,
                                       const function<char(int, int)> &getCellValue
                                      ) {

    // Iterate through all color codes in maxColorSet
    for (int colorCode : maxColorSet) {
        // Iterate through all regions corresponding to this color code
        const auto &regions = colorRegionsMap.at(colorCode);
        for (int root : regions) {

            auto regionCond = [&](int r, int c) -> int {
                int cell = r * m + c;
                return uf.find(cell) == root && cellCondition(r, c);
                };

            auto compositeCond = [&](int r, int c) -> bool {
                return regionCond(r, c) && (mat[r][c] == colorCode);
                };

            int minRow = n, maxRow = -1, minCol = m, maxCol = -1;
            calcBoundingBox(minRow, maxRow, minCol,
                            maxCol, n, m, regionCond);

            // Construct a unique file name based on the bounding box coordinates
            const string filepath = updatePath(filename, colorCode, minRow,
                                               alignCol(minCol), maxRow,
                                               alignCol(maxCol), algo);

            fWriteResults(filepath, n, m, compositeCond, getCellValue,
                          true, minRow, maxRow, minCol, maxCol);
        }
    }
}
#pragma warning(pop)