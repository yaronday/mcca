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

 // Define color constants for both platforms
#ifdef _WIN32
    // Windows color codes (based on standard 16-color palette)
const unsigned short BLACK = 0;
const unsigned short BLUE = 1;
const unsigned short GREEN = 2;
const unsigned short CYAN = 3;
const unsigned short RED = 4;
const unsigned short MAGENTA = 5;
const unsigned short YELLOW = 6;
const unsigned short WHITE = 7;
const unsigned short LIGHT_GREY = 8;
const unsigned short LIGHT_BLUE = 9;
const unsigned short LIGHT_GREEN = 10;
const unsigned short LIGHT_CYAN = 11;
const unsigned short LIGHT_RED = 12;
const unsigned short LIGHT_MAGENTA = 13;
const unsigned short LIGHT_YELLOW = 14;
const unsigned short BRIGHT_WHITE = 15;
const unsigned short DEFAULT_COLOR = WHITE;
#else
    // ANSI escape codes for macOS (or Unix-like systems)
const unsigned short BLACK = 30;
const unsigned short BLUE = 34;
const unsigned short GREEN = 32;
const unsigned short CYAN = 36;
const unsigned short RED = 31;
const unsigned short MAGENTA = 35;
const unsigned short YELLOW = 33;
const unsigned short WHITE = 37;
const unsigned short LIGHT_GREY = 90;
const unsigned short LIGHT_BLUE = 94;
const unsigned short LIGHT_GREEN = 92;
const unsigned short LIGHT_CYAN = 96;
const unsigned short LIGHT_RED = 91;
const unsigned short LIGHT_MAGENTA = 95;
const unsigned short LIGHT_YELLOW = 93;
const unsigned short BRIGHT_WHITE = 97;
const unsigned short DEFAULT_COLOR = WHITE;
#endif

// Utility function to convert a color key to a Windows or ANSI color code
inline unsigned short getColorFromKey(int key) {
    switch (key) {
    case 1: return LIGHT_BLUE;
    case 2: return LIGHT_GREEN;
    case 3: return LIGHT_YELLOW;
    case 4: return MAGENTA;
    case 5: return LIGHT_RED;
    case 6: return LIGHT_CYAN;
    case 7: return GREEN;
    case 8: return LIGHT_GREY;
    case 9: return LIGHT_RED;
    default: return WHITE;  // Default to WHITE if key is outside the range
    }
}

// Utility function to map a color key to a symbol
inline char getSymbolFromKey(int key) {
    switch (key) {
    case 1: return  '*';
    case 2: return  '@';
    case 3: return  '^';
    case 4: return  '>';
    case 5: return  '<';
    case 6: return  '$';
    case 7: return  '"';
    case 8: return  '&';
    case 9: return  '%';
    default: return '#';
    }
}

// Utility function to return the Graphviz color name from a key
inline const char* getGraphvizColorFromKey(int key) {
    switch (key) {
    case 1: return "blue";
    case 2: return "green";
    case 3: return "yellow";
    case 4: return "magenta";
    case 5: return "red";
    case 6: return "cyan";
    case 7: return "green";
    case 8: return "gray";
    case 9: return "pink";
    default: return "white";
    }
}
