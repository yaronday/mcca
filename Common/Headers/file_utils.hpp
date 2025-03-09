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

#include <filesystem>
#include <fstream>
#include <cstdlib>

#include "common.hpp"

using namespace std;

using namespace MyCommon;

//create dir if necessary
void createDir(const string &);

bool isValidDir(const string &);

size_t calcDirSize(const string& directoryPath,
				   const string& fnameFilter = "out");

size_t fGetSize(const string &);

// return filename without its extension
string fRemoveExt(const string &filename);

// generate file path, given filename, suffix and txt
string fGenPath(const string &filename,
			    const string &suffix,
			    bool txt = EN_TXT);

//Write to file
void fWrite(const string &contents,
			const string &filepath,
	        const string &info = DEFAULT_INFO,
	        bool overwrite = EN_OVR);

//Write to file directly from buffer 
void fWrite(const vector <char> &buffer,
		    const string &filepath,
	        const string &info = DEFAULT_INFO,
	        bool overwrite = EN_OVR);

void fHandleInfo(const string &filepath,
	             const string &info = DEFAULT_INFO,
				 bool overwrite = EN_OVR);
