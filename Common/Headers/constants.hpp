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

// title & version
inline constexpr char COPYRIGHT[] = "\nCopyright (C) Yaron Dayan 2025\n";
inline constexpr char VERSION[] = "v2.0.0";
inline constexpr char TITLE[] = "Max Connected Colors Analyzer (MCCA) ";

// misc
inline constexpr char MATSTR[] = R"({{1, 2, 1}, {2, 3, 2}, {1, 2, 1}}, {{1, 2, 1}, {2, 1, 2}, {1, 2, 3}})";
inline constexpr char DEFAULT_INFO[] = "file";
inline constexpr int VIS_MAT_THR = 15;

// io 
inline constexpr char DEFAULT_ROOTDIR[] = "../Data/matrix_shapes";
inline constexpr bool EN_TXT = true;
inline constexpr bool EN_OVR = false;
inline constexpr bool EN_CONFIRM = false;
inline constexpr int TABWIDTH = 4;
inline constexpr int EXT_LEN = 3; // default file extension length

// mat_files defaults
inline constexpr int DEFAULT_MIN_R = 200;
inline constexpr int DEFAULT_MAX_R = 300;
inline constexpr int DEFAULT_INC = 50;
inline constexpr int DEFAULT_MIN_V = 1;
inline constexpr int DEFAULT_MAX_V = 3;
inline constexpr bool DEFAULT_SQMAT = true;

// File generator defaults; same init values for cols
inline constexpr int ROWS_MIN = 1;       
inline constexpr int ROWS_MAX = 15000;   
inline constexpr int INC_MIN = 1;        
inline constexpr int INC_MAX = ROWS_MAX - 1;
inline constexpr int MIN_VAL = 1;
inline constexpr int MAX_VAL = 9;
inline constexpr double MAX_FILE_SIZE_MB = 500.0;
inline constexpr bool DEFAULT_FILEGEN = false;

// Algo
inline constexpr bool IS_ALGO_SPECIFIED = false;
inline constexpr bool SKIP_ALGO_HANDLER = false;