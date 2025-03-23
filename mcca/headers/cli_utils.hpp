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

#include <map>
#include <unordered_set>

#include "../../common/headers/matfile_handler.hpp"

using namespace std;

using namespace MyCommon;

const string algoSelect = "Select the algorithm to use: 'DFS', 'UF', 'BOTH' (default: BOTH).\n";

void mainMenuDisplay();

void cliErrHandler(bool menu_disp = false);

void handleArgs(int argc, char *argv[], string &matStr, 
	            MatFileHandler &mfh,
	            string &algoChoice, bool & paint, bool &colors, 
	            bool &crop, bool &visualizer);

void validateAndAssign(int &variable, const string &arg, int min_value, 
	                   int max_value, map<string, string> &args_map);

bool isFlag(const string &);

bool isInvalidAlgoChoice(const string &algoChoice);

void algoNotifier(const string &algo);

void visualizerNotifier(const string &algo, bool &visualizer);

void title_and_ver();

void handleAlgoSelection(string &algo, const string &value= "uf");

inline void showConditions() {
	title_and_ver();
	ostringstream oss;
	oss << "\n"
		<< "You may copy, modify, and redistribute this program under the terms of the GNU\n"
		<< "General Public License, version 3.0. The license requires that:\n"
		<< "- You include a copy of the GPLv3 license with the program, or provide\n"
		<< "  access to it.\n"
		<< "- Any modified versions of this program must also be distributed under\n" 
		<< "  the GPLv3.\n"
		<< "- You must make the source code of the program available if you distribute it\n"
		<< "  (or provide an offer to provide the source code).\n"
		<< "- **You must provide proper attribution to Yaron Dayan as specified in the\n"
		<< "  ATTRIBUTION.md file when redistributing or using the program.** \n"
		<< "  For more details, see the full text of the GNU General Public License\n"
		<< "  version 3.0.\n";	
	formatTxt(oss, LIGHT_CYAN);
}