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

#include <iostream>
#include <chrono>
#include <string>

using namespace std;
using namespace chrono;

// Function to scale and format the time measurement
pair<double, string> time_scaler(double time_res) {
    string time_unit = " [s]";
    if (time_res < 0.5 && time_res > 1e-3) {
        time_res *= 1e3;
        time_unit = " [ms]";
    }
    else if (time_res < 1e-3 && time_res > 1e-6) {
        time_res *= 1e6;
        time_unit = " [us]";
    }
    return { time_res, time_unit };
}

 
/*Function to measure time of a code block using a lambda or any callable
*  Example:
    // Define the lambda to wrap the function call
    auto func = [&]() {
        write2fResultsArea(mat, uf);
    };

    // Time the function using timeIt
    timeIt("write2fResultsArea", func, 1);
*/
template <typename Func>
void timeIt(const string &funcName, Func &&func, int numCalls = 100) {
    
    auto start = steady_clock::now();
    for (int i = 0; i < numCalls; i++) {
        func();  // Execute the code block (lambda or callable) 
    }
    auto end = steady_clock::now();
    auto delta_time = duration_cast<duration<double>>(end - start);
    pair<double, string> avgDuration = time_scaler(delta_time.count() / numCalls);

    cout << "\n\nFunction or code section'" << funcName << "' took " << avgDuration.first << avgDuration.second << endl;
}
