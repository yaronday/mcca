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
    bool visualizer = false;
    bool crop = false;

    string algoChoice;

    signal(SIGINT, handleSignal);

    MatFileHandler mfh;

    string matStr;

    DfsColorGrid dfsCG(mfh);
    UnionFindColorGrid ufCG(mfh);

    handleArgs(argc, argv, matStr, mfh,
               algoChoice, paint, colors, crop, visualizer);  // Parse CLI arguments

    algoNotifier(algoChoice);

    visualizerNotifier(algoChoice, visualizer);

    if (matStr.empty())
        processData(algoChoice, dfsCG, ufCG, mfh,
                    paint, colors, crop, visualizer);
    else{
        vector<vector<vector<int>>> matList;
        mfh.overwrite = true;
        matList = mfh.parseMatrices(matStr);
        for (vector<vector<int>> &mat : matList) {
            mfh.currMat = mat; 
            mfh.parsedMatIdx++;
            solveMccg(algoChoice, dfsCG, ufCG, mfh.currMat, 
                      paint, colors, crop, visualizer);     
            mfh.fWriteMat(mfh.filename, true);
        }
    }
    return 0;
}

void solveMccg(const string &algoChoice, DfsColorGrid &dfsCG,
               UnionFindColorGrid &ufCG,
               vector<vector<int>> &mat,
               bool &paint,
               bool &colors, bool crop, bool visualizer,
               const string &filepath) {

    if (algoChoice == "DFS" || algoChoice == "BOTH") {
        dfsCG.calcMaxConnectedColor(mat, paint, colors, filepath, crop);
    }
    if (algoChoice == "UF" || algoChoice == "BOTH") {
        ufCG.calcMaxConnectedColor(mat, paint, colors, filepath, crop, visualizer);
    }
}

void processData(const string &algoChoice,
                 DfsColorGrid &dfsCG, 
                 UnionFindColorGrid &ufCG,
                 MatFileHandler &mfh,
                 bool &paint, bool &colors, bool crop,
                 bool visualizer) {

    vector<pair<string, vector<vector<int>>>> data = mfh.fLoadMatrices();
    const int num_of_matrices = static_cast<int>(data.size());
    if (num_of_matrices > 0) {
        formatTxt("Processing Data...\n", LIGHT_CYAN);
        for (int k = 0; k < num_of_matrices; k++) {
            string filepath = data[k].first;
            mfh.currMat = std::move(data[k].second);
            ostringstream oss;
            oss << "\n" << k + 1 << ")" << filepath << "\n";
            formatTxt(oss, LIGHT_CYAN);
            solveMccg(algoChoice, dfsCG, ufCG, mfh.currMat,
                paint, colors, crop, visualizer, filepath);
        }
    }
}