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

#include "headers/main.hpp"
#include "../common/headers/timer.hpp"

using namespace std;


int main(int argc, char *argv[]) {  
    bool paint = false;
    bool colors = true;
    bool crop = false;

    signal(SIGINT, handleSignal);

    MatFileHandler mfh;

    string matStr;

    DfsColorGrid dfsCG(mfh);
    UnionFindColorGrid ufCG(mfh);

    handleArgs(argc, argv, matStr, mfh, paint, colors, crop, ufCG);  // Parse CLI arguments

    algoNotifier(ufCG);

    visualizerNotifier(ufCG);

    if (matStr.empty())
        processData(dfsCG, ufCG, mfh,
                    paint, colors, crop);
    else{
        vector<vector<vector<int>>> matList;
        mfh.overwrite = true;
        matList = mfh.parseMatrices(matStr);
        for (vector<vector<int>> &mat : matList) {
            mfh.currMat = mat; 
            mfh.parsedMatIdx++;
            solveMccg(dfsCG, ufCG, mfh.currMat, 
                      paint, colors, crop);     
            mfh.fWriteMat(mfh.filename, true);
        }
    }
    return 0;
}

void solveMccg(DfsColorGrid &dfsCG,
               UnionFindColorGrid &ufCG,
               vector<vector<int>> &mat,
               bool &paint,
               bool &colors, bool crop,
               const string &filepath) {

    const string& algo = ufCG.algo;
    if (algo == "DFS" || algo == "BOTH") {
        dfsCG.calcMaxConnectedColor(mat, paint, colors, filepath, crop);
    }
    if (algo == "UF" || algo == "BOTH") {
        ufCG.calcMaxConnectedColor(mat, paint, colors, filepath, crop);
    }
}

void processData(DfsColorGrid &dfsCG, 
                 UnionFindColorGrid &ufCG,
                 MatFileHandler &mfh,
                 bool &paint, bool &colors, bool crop) {
    vector<pair<string, vector<vector<int>>>> data = mfh.fLoadMatrices();
    const int num_of_matrices = static_cast<int>(data.size());
    if (num_of_matrices > 0) {
        formatTxt("Processing data...\n", LIGHT_CYAN);
        for (int k = 0; k < num_of_matrices; k++) {
            string filepath = data[k].first;
            mfh.currMat = std::move(data[k].second);
            ostringstream oss;
            oss << "\n" << k + 1 << ")" << filepath << "\n";
            formatTxt(oss, LIGHT_CYAN);
            solveMccg(dfsCG, ufCG, mfh.currMat,
                      paint, colors, crop, filepath);
        }
    }
}