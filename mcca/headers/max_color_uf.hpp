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
#include "union_find.hpp"

#include <fstream>
#include <string>

using namespace std;


class UnionFindColorGrid : public ColorGrid {
private:
    const string algo = "UF";


public:
    bool visualizerEn;  // specific to UF 
    string imageFormat; // for visualizer 

    // Hash function for pairs of integers
    struct hash_pair {
        template <class T1, class T2>
        size_t operator() (const pair<T1, T2> &p) const {
            auto h1 = hash<T1>{}(p.first);
            auto h2 = hash<T2>{}(p.second);
            return h1 ^ (h2 << 1);  // Combine the two hashes, shift to avoid collision
        }
    };

    UnionFindColorGrid() = delete;

    explicit UnionFindColorGrid(MatFileHandler &handler);

    virtual int calcMaxConnectedColor() override;

    int calcMaxConnectedColor(vector<vector<int>> &mat,
                              bool paint = true,
                              bool colors = true,
                              const string &filepath = "", 
                              bool crop = false);

protected:
    unordered_set<int> maxColorSet;  // Specific to UnionFindColorGrid (for O(1) color code lookup)
    unordered_map<int, unordered_set<int>> colorRegionsMap;  // Specific to UnionFindColorGrid (to track regions of each color code)

    void visualizeUF(UnionFind &uf,
                     int maxSize,
                     const string &filename,
                     bool show = false);

    const char *getRegionColor(int root, const unordered_map<int, unordered_set<int>> &colorRegionsMap) const;

    void notifyMaxColorRegions(const string &filename, int maxSize,
                               const unordered_set<int> &maxColorSet,
                               const unordered_map<int, unordered_set<int>> &colorRegionsMap, 
                               bool extraFile = false);

    void processAdjCells(int row, int col,
                         vector<vector<int>> &matrix,
                         int currCell,
                         int currValue,
                         UnionFind &uf,
                         unordered_set<pair<int, int>, hash_pair> &loggedPairs
                        );

    //For --crop option (only write each bounding box of equally-sized max region to separate file)
    void fWriteCropped(const string &filename,
                       UnionFind &uf,
                       vector<vector<int>> &mat,
                       int n, int m,
                       const function<bool(int, int)> &cellCondition,
                       const function<char(int, int)> &getCellValue
                      );
};
