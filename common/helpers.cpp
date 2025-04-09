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

#include "headers/helpers.hpp"
#include "headers/common.hpp"

using namespace MyCommon;


//in-place string mods
string strToUpper(string s) {
    for (char &c : s) {
        c = toupper(static_cast<unsigned char>(c)); // Ensure correct type
    }
    return s;
}

string strToLower(string s) {
    for (char &c : s) {
        c = tolower(static_cast<unsigned char>(c)); // Ensure correct type
    }
    return s;
}

// Function to check whether a specific substr exists within str
static bool hasSubstr(const string &str, const string &substr) {
    return str.find(substr) != string::npos;
}

bool isValidValue(const string &str) {
    if (str.empty()) {
        return false;
    }
    if (!isNumeric(str))
        return false;
    if (!isWithinRange(stoi(str), MIN_VAL, MAX_VAL))
        return false; 
    return true;
}

bool isWithinRange(const int &value, int min_val, int max_val) {
    if ((value < min_val) || (value > max_val)) {
        handleError(ErrCode::INVALID_RANGE, value, "-->", min_val, max_val);
        return false;
    }
    return true;
}

bool isNumeric(const string &str) {
    if (str.empty()){
        handleError(ErrCode::EMPTY_STR, ErrorContext());
        return false;  // Early exit for empty strings
    }

    auto it = str.begin();  // Iterator to the first character

    // If there's a minus sign at the beginning, skip it
    if (*it == '-') {
        if (str.size() == 1){
            handleError(ErrCode::NOT_NUMERICAL, str);
            return false;  // Just "-" is not valid
        }
        ++it;  // Skip the minus sign
    }

    // Check if all remaining characters are digits
    for (char ch : string(it, str.end())) {
        if (!isdigit(ch)){
            handleError(ErrCode::NOT_NUMERICAL, str);
            return false;  // Early exit on first invalid character
        }
    }
    return true;  // All characters are digits
}

int evalNum(size_t idx, vector<char> &buff) {
    int res = 0;
    while (idx < buff.size() && isdigit(buff[idx])) {
        res = res * 10 + (buff[idx] - '0');
        ++idx;
    }
    return res;
}

void delimFix(string &line, const string &delimiters, const char &replacer) {
    // Trim leading and trailing whitespace
    line.erase(0, line.find_first_not_of(" \t"));
    line.erase(line.find_last_not_of(" \t") + 1);

    // Replace delimiters with replacer char
    for (char &ch : line) {
        if (delimiters.find(ch) != string::npos) {
            ch = replacer;
        }
    }
}

string removeSpaces(string_view input) {
    string result;
    result.reserve(input.size());

    for (char ch : input) {
        if (!isspace(static_cast<unsigned char>(ch))) {
            result += ch;
        }
    }
    return result;
}

unordered_set<string> split2UnorderedSet(string_view input, char delimiter) {
    // convert str to unordered_set

    unordered_set<string> result;

    size_t start = 0;
    while (start < input.size()) {
        size_t end = input.find(delimiter, start);
        if (end == string_view::npos)
            end = input.size();

        string token = removeSpaces(input.substr(start, end - start));
        if (!token.empty()) {
            result.emplace(move(token));
        }
        start = end + 1;
    }
    return result;
}

vector<int> validateRow(const string &line) {
    if (!isValidStr(line)) {
        return { -1 };
    }

    vector<int> row;
    istringstream iss(line);
    int element;

    while (iss >> element) {
        row.emplace_back(element);
    }

    return row;
}

bool isValidStr(const string &str) {
    istringstream iss(str);
    string token;

    // Tokenize the string by whitespace
    while (iss >> token) {
        if (!isValidValue(token)) {
            return false;
        }
    }
    return true;
}

vector<string> strSplit(const string &input) {
    vector<string> substrings;
    size_t pos = 0;

    string temp = removeSpaces(input);

    size_t tempLen = temp.length();

    if (tempLen < 3) {
        handleError(ErrCode::EMPTY_STR, ErrorContext());
        return { " " };
    }

    // Remove leading and trailing curly braces
    string trimmedStr = temp.substr(1, tempLen - 2);

    const string delimiter = "}},";

    size_t delimLen = delimiter.length();

    while ((pos = trimmedStr.find(delimiter, pos)) != string::npos) {
        substrings.emplace_back(trimmedStr.substr(0, pos + delimLen - 1));
        pos += delimLen;
        temp = trimmedStr.substr(pos);
        trimmedStr = temp;
        pos = 0;
    }

    // Add the last substring
    substrings.emplace_back(temp);

    return substrings;
}


void handleDiskSpace(double totalSize, const double maxSize) {
    double totalSizeKB = totalSize / 1024.0;
    double totalSizeMB = totalSizeKB / 1024.0;
    double totalSizeGB = totalSizeMB / 1024.0;

    // Disk space protection
    if (totalSizeMB > maxSize) {
        handleError(ErrCode::FILESIZE_LIMIT_REACHED, totalSizeMB, maxSize);
        exit(0);
    }

    formatTxt("Required space = ", LIGHT_YELLOW, "");
    const string space_required = (totalSizeGB >= 1) ? to_str_w_precision(totalSizeGB) + " GB" :
        (totalSizeMB >= 1) ? to_str_w_precision(totalSizeMB) + " MB" :
        (totalSizeKB >= 1) ? to_str_w_precision(totalSizeKB) + " KB" :
        to_string(static_cast<int>(totalSize)) + " Bytes";

    formatTxt(space_required, LIGHT_YELLOW);
}

bool skipAction(const string &desc) {
    ostringstream oss;
    oss << "Proceed with " << desc << " [Y / N] = ? ";
    formatTxt(oss, LIGHT_CYAN);
    char input;
    cin >> input;
    if (input == 'Y' || input == 'y') {
        formatTxt("OK!", LIGHT_GREEN);
        return false;
    }
    else {
        formatTxt("Exiting...", LIGHT_CYAN);
        return true;
    }
}

//adjust end value and num of elements of a non-evenly spaced range
pair<int, int> adjustedRange(int start, int stop, int step) {
    pair<int, int> result;
    result.first = start + ((stop - start) / step) * step; // adjusted end values 
    result.second = (result.first - start) / step + 1;  // num of elements 
    return result;
}