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

#include "cli_utils.hpp"
#include "max_color_uf.hpp"
#include "max_color_dfs.hpp"

void solveMccg(const string &algoChoice, DfsColorGrid &dfsCG, UnionFindColorGrid &ufCG,
               vector<vector<int>> &mat,
               bool &paint,
               bool &colors, bool crop, bool visualizer, const string &filepath = "");

void processData(const string &algoChoice,
                 DfsColorGrid &dfsCG, UnionFindColorGrid &ufCG,
                 MatFileHandler &mfh,
                 bool &paint, bool& colors, bool crop,
                 bool visualizer);