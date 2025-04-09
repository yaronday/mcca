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

#include <string>
#include <sstream>
#include <optional>
#include "constants.hpp"

using namespace std;


static const string withinRange(const int &minv, const int &maxv);

// Enum for error codes
enum class ErrCode {
    // CLI related
    INVALID_ALGO_SELECTION,
    MISSING_ARG_VALUE,
    INT_EXPECTED,
    UNKNOWN_OPTION,
    UNEXPECTED_ARG,

    // Matrix related
    INVALID_MAT,
    ROW_LEN_MISMATCH,
    MAT_EMPTY,
    NO_MAT_FOUND,
    INVALID_RANGE,
    NOT_NUMERICAL,
    EMPTY_STR,
    INVALID_CHAR,
    MULTIDIGIT,
    GEN_MATRIX_FAILED, 

    // File related
    FILE_OPEN_ERROR,
    INVALID_DIR,
    CDIR_FAILURE,
    INVALID_FILE_EXT,
    EMPTY_DIR,
    OVR_NOT_ENABLED,
    FILESIZE_LIMIT_REACHED,

    // Software items
    GRAPHVIZ_NA,
    GRAPHVIZ_MAT_LIMIT,
    GRAPHVIZ_IMG_FORMAT_ERR,

    GENERIC_EXCEPTION
};

// ErrorContext struct to hold error-specific information
struct ErrorContext {
    optional<int> row;
    optional<int> col;
    optional<char> invalidChar;
	
    optional<int> invalidValue;
    
    optional<string> invalidStr; 

    optional<string> argName;

    optional<int> minValue;
    optional<int> maxValue;
    
    optional<double> size;
    optional<double> sizeLimit;

    optional<string> exceptionMsg; // For storing exception messages
 

    // Default constructor
    ErrorContext() = default;

    // Constructor for INVALID_CHAR error
    ErrorContext(int r, int c, char ch) : invalidChar(ch), row(r), col(c) {}

    // Constructor for INVALID_RANGE error
    ErrorContext(int value, const string &arg, int min, int max) : 
                 invalidValue(value), argName(arg), minValue(min), maxValue(max) {}

    // Constructor for ROW_LEN_MISMATCH error
    ErrorContext(int r) : row(r) {}

    // constructor for MULTIDIGIT error 
    // (basically the whole value could be evaluated, triggering an invalid range error, but this way is simpler and faster) 
    ErrorContext(int r, int c, int value) : row(r), col(c), invalidValue(value) {}

    // constructor for Arg releated or NOT_NUMERICAL error 
    ErrorContext(const string &arg) : argName(arg) {}

    // constructor for filesize limit 
    ErrorContext(double filesize, double maxSize) : size(filesize), sizeLimit(maxSize) {}

    // constructor for image format error
    ErrorContext(string value, const string &arg) : invalidStr(value), argName(arg) {}

    // Constructor for exceptions
    ErrorContext(const exception &ex) : exceptionMsg(ex.what()) {}
};

// Function to generate error messages based on error code and context
inline string getErrMsg(ErrCode code, const ErrorContext &context) {
    ostringstream oss;

    const string OVR_NA_ERR = "Overwrite must be enabled in order to carry out this action";
    const string INVALID_FEXT_ERR = "Invalid file extension. Only 'txt' and 'csv' are supported";
    const string EMPTYDIR_ERR = "No matching files were found in directory.";
    const string ROWLEN_ERR = "Row length mismatch"; 
    const string MULTIDIGIT_ERR = "Multidigit detected"; 
    const string UNEXP_ARG_ERR = "Unexpected Argument";
    const string MISSING_ARGVAL_ERR = "Missing value for argument";
    const string INT_AFTER_ARG_ERR = "There must be a positive integer after argument"; 
    const string ROOTDIR_ERR = "Invalid root directory";
    const string FILESIZE_LIMIT_ERR = "Estimated filesize[MB] required exceeds maximum limit";
    const string FILE_ERR = "Error opening file";
    const string DIR_ERR = "Invalid directory";
    const string CDIR_ERR = "Failed to create directory";
    const string GENERIC_EXCEPTION_ERR = "An exception occurred"; 
    const string UNKNOWN_OPT_ERR = "Unknown option";
    const string NONNUMERIC_ERR = " is either not numerical or of an invalid format";
    const string INV_CHAR_ERR = "Invalid character";
    const string GRAPHVIZ_NA_ERR = "The visualizer must have Graphviz installed and configured as system env variable";
    const string GRAPHVIZ_MAT_LIMIT_ERR = "Visualizer deactivated: Matrix max(m, n) > ";
    const string GRAPHVIZ_IMG_FORMAT_ERR = "Invalid image format:";

    switch (code) {
        case ErrCode::INVALID_CHAR:
            oss << INV_CHAR_ERR;
            if (context.invalidChar && context.row && context.col) {
                oss <<" '" << *context.invalidChar
                   << "' at row " << *context.row 
                   << ", col " << *context.col;
            }
            else if (context.invalidChar) {
                oss << " '" << *context.invalidChar;
            }        
            oss << "!\n";     
            break;

        case ErrCode::INVALID_RANGE:
            if (context.invalidValue && context.argName && 
                context.minValue && context.maxValue) {
                oss << *context.argName << " " << *context.invalidValue 
                    << " is invalid! " << "Value must be "
                    << withinRange(*context.minValue, *context.maxValue)
                    << "!\n";
            } 
            else 
                oss << "Invalid range!\n";           
            break;
        
        case ErrCode::ROW_LEN_MISMATCH:
            oss << ROWLEN_ERR;
            if (context.row) oss << " at row " << *context.row;
            oss << "\n";
            break;
    
        case ErrCode::INVALID_ALGO_SELECTION:
            oss << "Error: Invalid value for --algo option. Must be 'DFS', 'UF', or 'BOTH'!\n";
            break;
    
        case ErrCode::MULTIDIGIT:
            oss << MULTIDIGIT_ERR;
            if (context.row && context.col && context.invalidValue) {
                oss << " at row " << *context.row << ", col "
                    << *context.col << ": " << *context.invalidValue;
            }
            oss << "!\n";
            break;

        case ErrCode::UNEXPECTED_ARG:
            oss << UNEXP_ARG_ERR;
            if (context.argName)
                oss << ": " << *context.argName;
            oss << "!\n";
            break;

        case ErrCode::MISSING_ARG_VALUE:
            oss << MISSING_ARGVAL_ERR;
            if (context.argName)
                oss << ": " << *context.argName;
            oss << "!\n";
            break;

        case ErrCode::INT_EXPECTED:
            oss << INT_AFTER_ARG_ERR;
            if (context.argName)
                oss << ": " << *context.argName;      
            oss << "!\n";
            break;

        case ErrCode::NOT_NUMERICAL:
            if (context.argName)
                oss << *context.argName;
            else
                oss << "Value";
            oss << NONNUMERIC_ERR;
            oss << "!\n";
            break;

        case ErrCode::EMPTY_STR:
            oss << "Empty string!\n";
            break;

        case ErrCode::INVALID_MAT:
            oss << "Invalid Matrix!\n";
            break;

        case ErrCode::MAT_EMPTY:
            oss << "Matrix is empty!\n";
            break;

        case ErrCode::GEN_MATRIX_FAILED:
            oss << "Matrix generation error!\n";
            break;
           
        case ErrCode::NO_MAT_FOUND:
            oss << "No matrix was found!\n";
            break;

        case ErrCode::UNKNOWN_OPTION:
            oss << UNKNOWN_OPT_ERR;
            if (context.argName)
                oss << ": " << *context.argName;
            oss << "!\n";
            break;

        // Graphviz related errors
        case ErrCode::GRAPHVIZ_NA:
            oss << GRAPHVIZ_NA_ERR;
            oss << "!\n";
            break; 

        case ErrCode::GRAPHVIZ_MAT_LIMIT:      
            oss << GRAPHVIZ_MAT_LIMIT_ERR << VIS_MAT_THR << "!\n";
            break;

        case ErrCode::GRAPHVIZ_IMG_FORMAT_ERR:
            oss << GRAPHVIZ_IMG_FORMAT_ERR;
            if (context.invalidStr && context.argName) {
                oss << " '" << *context.invalidStr
                    << "' for " << *context.argName;
            }
            else if (context.invalidStr) {
                oss << " '" << *context.invalidStr;
            }
            oss << "!\n";
            oss << "Please use one of these: " << VIS_IMAGE_FORMATS << ".\n";
            break;


        // File related errors
        case ErrCode::FILE_OPEN_ERROR:
            oss << FILE_ERR;
            if (context.argName)
                oss << ": " << *context.argName;
            oss << "!\n";
            break;

        case ErrCode::INVALID_DIR:
            oss << DIR_ERR;
            if (context.argName)
                oss << ": " << *context.argName;           
            oss << "!\n";
            break;

        case ErrCode::EMPTY_DIR:
            oss << EMPTYDIR_ERR;
            if (context.argName)
                oss << ": " << *context.argName;          
            oss << "!\n";
            break;

        case ErrCode::CDIR_FAILURE:
            oss << CDIR_ERR;
            if (context.argName)
                oss << ": " << *context.argName;
            oss << "!\n";
            break;

        case ErrCode::INVALID_FILE_EXT:
            if (context.argName)
                oss << "'" << *context.argName << "'" << ": ";
            oss << INVALID_FEXT_ERR;
            oss << "!\n";
            break;

        case ErrCode::OVR_NOT_ENABLED:
            if (context.argName)
                oss << "File: " << *context.argName << "\n";        
            oss << OVR_NA_ERR;
            oss << "!\n";
            break;

        case ErrCode::FILESIZE_LIMIT_REACHED:
            oss << FILESIZE_LIMIT_ERR;
            if (context.sizeLimit && context.size)
                oss << *context.size << " > " << *context.sizeLimit;
            oss << "!\n";
            break;

        case ErrCode::GENERIC_EXCEPTION:
            oss << GENERIC_EXCEPTION_ERR;
            if (context.exceptionMsg) {
                oss << ": " << *context.exceptionMsg;
            }        
            oss << "!\n";         
            break;

        default:
            oss << "Unknown error!\n";
            break;
    }
    return oss.str();
}

//valid range notifier
static const string withinRange(const int &minv, const int &maxv) {
    ostringstream val_range;
    val_range << "within [" << minv << ", " << maxv << "]";
    return val_range.str();
}