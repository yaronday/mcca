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

#include "Headers/common.hpp"

namespace MyCommon {
    
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    void formatTxt(const string &txt, 
                   const unsigned short &colorCode, 
                   const string &ending) {
        SetConsoleTextAttribute(handle, colorCode);
        ostringstream oss;
        oss << txt << ending;
        cout << oss.str();
        if (colorCode != DEFAULT_COLOR)
            resetConsoleColor();
    }

    void formatTxt(const ostringstream &oss, 
                   const unsigned short &colorCode, 
                   const string &ending) {
        formatTxt(oss.str(), colorCode, ending); // Reuse the primary function
    }

    // Basic error handler (no parametrized context)
    void handleError(ErrCode code, const ErrorContext &context) {
        formatTxt(getErrMsg(code, context), LIGHT_RED);
    }

    //INVALID_CHAR error handler 
    void handleError(ErrCode code, char invalidChar, 
                     int row, int col) {
        ErrorContext context(row, col, invalidChar);
        handleError(code, context);
    }

    //INVALID_CHAR wo specifying row and column 
    void handleError(ErrCode code, char invalidChar) {
        // Create context with only the invalidChar
        ErrorContext context(invalidChar);
        handleError(code, context);
    }

    //INVALID_RANGE handler
    void handleError(ErrCode code, int value, 
                     const string &arg, int min, int max) {
        ErrorContext context(value, arg, min, max);
        handleError(code, context);
    }

    //ROW_LEN_MISMATCH handler
    void handleError(ErrCode code, int row) {
        ErrorContext context(row);
        handleError(code, context);
    }

    //MULTIDIGIT error handler
    void handleError(ErrCode code, int row, int col, int val) {
        ErrorContext context(row, col, val);
        handleError(code, context);
    }

    //NOT_NUMERICAL or ARG related error handler
    void handleError(ErrCode code, const string &val) {
        ErrorContext context(val);
        handleError(code, context);
    }

    //Filesize limit reached
    void handleError(ErrCode code, double size, double limit) {
        ErrorContext context(size, limit);
        handleError(code, context);
    }

    //Generic Exception handler
    void handleError(ErrCode code, const exception &ex) {
        ErrorContext context(ex);
        handleError(code, context);
    }

    void resetConsoleColor() {
        SetConsoleTextAttribute(handle, DEFAULT_COLOR);
    }
    
    // Signal handler to reset terminal color
    void handleSignal(int signal) {
        if (signal == SIGINT) {
            cout << "Aborted by user!\n\n";
            resetConsoleColor(); // Reset color before exiting
            exit(0); // Exit the program
        }
    }
}