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

#include "Headers/union_find.hpp"

int UnionFind::find(int x) const {
    // path compression 
    return (parent[x] != x) ? find(parent[x]) : parent[x];
}

UnionFind::UnionFind(int n) {
    parent.resize(n);
    size.resize(n);
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
        size[i] = 1;
    }
}

void UnionFind::unite(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    if (rootX != rootY) {
        if (size[rootX] < size[rootY]) {
            swap(rootX, rootY);
        }
        parent[rootY] = rootX;
        size[rootX] += size[rootY];
    }
}

int UnionFind::getSize(int x) {
    return size[find(x)];
}
