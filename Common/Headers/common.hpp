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

#include <iomanip>
#include <iostream>
#include <csignal>
#include <string>

#include "colors.hpp"
#include "error_handler.hpp"

using namespace std;

namespace MyCommon {

#ifdef _WIN32
    
    #include <Windows.h>
    extern HANDLE handle;

#elif __APPLE__

    #include <unistd.h>  // For POSIX signal handling on macOS

#endif
    
    void resetConsoleColor();

    //Basic error handler (no parametrized context)
    void handleError(ErrCode code, const ErrorContext &context);

    //INVALID_CHAR error handler
    void handleError(ErrCode code, char invalidChar, int row, int col);

    //INVALID_CHAR wo specifying row and column
    void handleError(ErrCode code, char invalidChar);

    //INVALID_RANGE handler
    void handleError(ErrCode code, int value, const string &arg, int min, int max);

    //ROW_LEN_MISMATCH handler
    void handleError(ErrCode code, int row);

    //MULTIDIGIT error handler
    void handleError(ErrCode code, int row, int col, int val);

    //NOT_NUMERICAL or Arg related error handler
    void handleError(ErrCode code, const string &val);

    //Filesize limit reached
    void handleError(ErrCode code, double size, double limit);

    //Generic Exception handler
    void handleError(ErrCode code, const exception &ex);

    void handleSignal(int);

    void formatTxt(const string &txt, const unsigned short &colorCode = DEFAULT_COLOR, 
                    const string &ending = "\n");

    void formatTxt(const ostringstream &oss, const unsigned short &colorCode = DEFAULT_COLOR, 
                    const string &ending = "\n");
}