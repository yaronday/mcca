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

#include "helpers.hpp"
#include "file_utils.hpp"

class MatFileHandler {

public:

    int min_rows;
    int max_rows;
    int row_inc;
    int min_cols;
    int max_cols;
    int col_inc;
    int min_v;
    int max_v;
    bool sqmat; 

    string destpath;
    bool confirm;
    bool txt;
    bool overwrite;

    vector<vector<int>> currMat;

    int parsedMatIdx;  // for matString

    vector<int> matInfo; 

    string filename; 

    MatFileHandler();

    void updateMatParams(int min_rows = DEFAULT_MIN_R, int max_rows = DEFAULT_MAX_R,
                         int row_inc = DEFAULT_INC, int min_cols = DEFAULT_MIN_R, int max_cols = DEFAULT_MAX_R,
                         int col_inc = DEFAULT_INC, int min_v = DEFAULT_MIN_V, int max_v = DEFAULT_MAX_V
                        );

    void updateFileParams(string destpath = DEFAULT_ROOTDIR, 
                          bool confirm = EN_CONFIRM, 
                          bool txt = EN_TXT, bool overwrite = EN_OVR);
                         
    const string updatePath(int rows, int cols,
                            int min_value, int max_value) const;

    const string updatePath(const string &filename) const;
    
    string updateStrByIntVect(vector<int>) const;

    void displayMat(const vector<vector<int>> &mat);

    void displayMatrices(const vector<vector<vector<int>>> &mat_vector);

    bool isValidMatSpec(int rows, int cols) const;

    vector<vector<int>> genRndMat(int rows, int cols, mt19937 gen);

    vector<vector<int>> genRndMatWrapper(int rows, int cols, 
                                         optional<unsigned int> seed = nullopt);

    vector<vector<int>> parseMat(const string &matrixStr);

    vector<vector<vector<int>>> parseMatrices(const string &matricesStr);

    void calcSizeSqMatFiles() const;
    
    void calcSizeMatFiles () const;

    bool matLoader(vector<vector<int>> &matrix, const string &filepath);

    vector<pair<string, vector<vector<int>>>> fLoadMatrices();

    void fWriteRndMat(int rows, int cols);
    
    //write matrix -> buffer -> file
    void fWriteMat(const string &filename = "mat_check", bool skipUpdate = false);

    vector<char> fillBuffMat();

    vector <char> fillBuffRndMat(int rows, int cols, mt19937 gen);

    vector<char> fillBuffRndMat(int rows, int cols);

    // Helper function to initialize a buffer
    vector<char> bufferInit(int min_r, int max_r, int min_c, int max_c);

    template <typename ConditionFunc, typename ValueFunc>
    vector<char> fillBuffer(ConditionFunc cellCondition,
                            ValueFunc getValue,
                            int minRow, int maxRow,
                            int minCol, int maxCol) {

        auto buffer = bufferInit(minRow, maxRow, minCol, maxCol);

        const char delimiter = txt ? ' ' : ',';

        auto condValue = [&](int r, int c) {
            return cellCondition(r, c) ? getValue(r, c) : delimiter;
            };

        populateBuffer(buffer, minRow, maxRow, minCol, 
                       maxCol, delimiter, condValue);

        return buffer;
    }

    // General-purpose buffer population function
    template <typename ValueFunc>
    void populateBuffer(vector<char> &buffer,
        int minRow, int maxRow,
        int minCol, int maxCol,
        char delimiter, ValueFunc getValue) {

        char *wPtr = buffer.data();

        for (int r = minRow; r <= maxRow; ++r) {
            for (int c = minCol; c <= maxCol; ++c) {
                *wPtr++ = getValue(r, c); 
                if (c < maxCol) {
                    *wPtr++ = delimiter;
                }
            }
            *wPtr++ = '\r'; 
            *wPtr++ = '\n';
        }
    }

    //Generate N x M matrix files
    void genMatFiles();

    //Generate N x N square matrix files
    void genSqMatFiles();

    void invokeGenMatFiles();
};