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

#include <random>

#include "common.hpp"

using namespace std;

string strToUpper(string);

string strToLower(string);

static bool hasSubstr(const string &, const string &);

bool isValidStr(const string &str);

bool isValidValue(const string &);

bool isNumeric(const string &);

int evalNum(size_t idx, vector<char> &buff);

bool isWithinRange(const int &, int, int);

string removeSpaces(const string &);

vector<string> strSplit(const string &);

vector<int> validateRow(const string &);

void handleDiskSpace(double totalSize, const double maxSize);

bool skipAction(const string &desc);

// Generates random char value in range
inline static char rndCharInRange(mt19937 &rng, char min_value, char max_value) {
    uniform_int_distribution<int> dist(min_value, max_value);
    return static_cast<char>('0' + dist(rng)); // Ensure ASCII representation for chars
}

// Generates random int value in range
inline static int rndIntInRange(mt19937 &rng, char min_value, char max_value) {
    uniform_int_distribution<int> dist(min_value, max_value);
    return dist(rng); 
}

inline char itoc(int num) {
    return static_cast<char>('0' + num);
}

pair<int, int> adjustedRange(int start, int stop, int step);

void delimFix(string &line,
              const string& delimiters = ";,",
              const char& replacer = ' ');

template <typename T>
inline string to_str_w_precision(const T val, const int prec = 2) {
    ostringstream out;
    out << fixed << setprecision(prec) << val;
    return std::move(out).str();
}

// generic for extracting a vector from an arbitrary container 
// Example: 
//      vector<tuple<int, double, string>> tuples = { {12, 2.234, "abra"}, {2, 3.5, "cadabra!"} };
//      auto lambFunc = [](const auto &t) { return get<2>(t); };
//      auto elements = conXtract(tuples, lambFunc);
template <typename Container, typename xtLambdaFunc>
auto conXtract(const Container &input, xtLambdaFunc extractor) {
    using ReturnType = decltype(extractor(*input.begin()));
    vector<ReturnType> output;
    output.reserve(input.size());
    transform(input.begin(), input.end(), back_inserter(output), extractor);
    return output;
}