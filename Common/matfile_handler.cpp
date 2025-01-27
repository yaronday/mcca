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

#include "Headers/matfile_handler.hpp"

using namespace MyCommon;

MatFileHandler::MatFileHandler():
    min_rows(DEFAULT_MIN_R),
    max_rows(DEFAULT_MAX_R),
    row_inc(DEFAULT_INC),
    min_cols(DEFAULT_MIN_R),
    max_cols(DEFAULT_MAX_R),
    col_inc(DEFAULT_INC),
    min_v(DEFAULT_MIN_V),
    max_v(DEFAULT_MAX_V),
    sqmat(DEFAULT_SQMAT),
    destpath(DEFAULT_ROOTDIR),
    confirm(EN_CONFIRM),
    txt(EN_TXT),
    overwrite(EN_OVR),
    parsedMatIdx(0),
    matInfo({0, 0, 0, 0}),  // n rows, m cols, min_value, max_value
    filename("")
    {}

const string MatFileHandler::updatePath(int rows, int cols,
                                        int min_value, int max_value) const {

    const string ext = txt ? "txt" : "csv";

    ostringstream filepath;
    filepath << destpath << "/mat_"
        << rows << "_" << cols << "_"
        << min_value << "_" << max_value << "." << ext;
    return filepath.str();
}

void MatFileHandler::updateMatParams(int min_rows, int max_rows, int row_inc, int min_cols,
                                     int max_cols, int col_inc, int min_v, int max_v) {
    this->min_rows = min_rows;
    this->max_rows = max_rows;
    this->row_inc = row_inc;
    this->min_cols = min_cols;
    this->max_cols = max_cols;
    this->col_inc = col_inc;
    this->min_v = min_v;
    this->max_v = max_v;
}

void MatFileHandler::updateFileParams(string destpath, bool confirm, bool txt, bool overwrite) {
    this->destpath = destpath; 
    this->confirm = confirm;
    this->txt = txt;
    this->overwrite = overwrite;
}

const string MatFileHandler::updatePath(const string &filename) const {

    const string ext = txt ? "txt" : "csv";
    ostringstream filepath, default_fname;
    size_t matRowLen = currMat[0].size();
    default_fname << "/mat_" << currMat.size() << "_" << matRowLen;
    const string fname = (filename != "") ? filename : default_fname.str();
    filepath << destpath
        << "/mat_" << currMat.size() << "_" << matRowLen
        << "." << ext;
    return filepath.str();
}

// parsed matrix filename updater 
string MatFileHandler::updateStrByIntVect(vector<int> vect) const {
    const string ext = this->txt ? "txt" : "csv";
    ostringstream oss;
    oss << "mat" << this->parsedMatIdx << "_";
    for (auto it = vect.begin(); it != vect.end(); ++it) {
        oss << *it;
        if (std::next(it) != vect.end()) {
            oss << '_';
        }
    }
    oss << "." << ext;
    return oss.str();
}

void MatFileHandler::displayMat(const vector<vector<int>> &mat) {
    ostringstream oss;
    for (const auto &row : mat) {
        for (int value : row) {
            oss << value << ' ';
        }
        oss << '\n';
    }
    formatTxt(oss, LIGHT_CYAN);
}

void MatFileHandler::displayMatrices(const vector<vector<vector<int>>> &matVect) {
    for (const auto &mat : matVect) {
        displayMat(mat);
        string separator(2 * mat[0].size(), '-');
        formatTxt(separator, LIGHT_CYAN);
    }
}

bool MatFileHandler::isValidMatSpec(int rows, int cols) const {
    if (rows <= 0 || cols <= 0) {
        if (rows <= 0) {
            handleError(ErrCode::INVALID_RANGE, rows, "Rows", 1, ROWS_MAX);
        }
        if (cols <= 0) {
            handleError(ErrCode::INVALID_RANGE, cols, "Columns", 1, ROWS_MAX);
        }
        return false;
    }
    if (!isWithinRange(min_v, MIN_VAL, MAX_VAL) || !isWithinRange(max_v, MIN_VAL, MAX_VAL)) {
        return false;
    }  
    if (min_v > max_v) {
        handleError(ErrCode::INVALID_RANGE, min_v, "min_value", 1, max_v);
        return false;
    }
    return true;
}

vector<vector<int>> MatFileHandler::genRndMat(int rows, int cols, mt19937 gen) {
    if (!isValidMatSpec(rows, cols))
        return {};

    ostringstream info;
    info << "Generating a random " << rows << "x" << cols
        << " matrix with values " << withinRange(min_v, max_v) << "\n";
    formatTxt(info, LIGHT_MAGENTA);

    vector<vector<int>> mat(rows, vector<int>(cols));
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            mat[row][col] = rndIntInRange(gen, min_v, max_v);
        }
    }
    return mat;
}

vector<vector<int>> MatFileHandler::genRndMatWrapper(int rows, int cols, optional<unsigned int> seed) {
    mt19937 gen;
    if (seed) {
        gen.seed(*seed); // Use the provided seed for deterministic output
    }
    else {
        random_device rd;
        gen.seed(rd());
    }
    return genRndMat(rows, cols, gen);
}


vector<vector<int>> MatFileHandler::parseMat(const string &matrixStr) {
    vector<vector<int>> matrix;
    string rowToken;
    bool isFirstRow = true;
    size_t strLength = matrixStr.length();

    if (strLength < 3) {
        handleError(ErrCode::EMPTY_STR, ErrorContext());
        return { { 0 } };
    }

    string trimmedString = matrixStr.substr(1, strLength - 2);

    if (trimmedString.empty()) {
        handleError(ErrCode::EMPTY_STR, ErrorContext());
        return { { 0 } };
    }

    istringstream matrixStream(trimmedString);

    size_t row_len = 0;
    // Tokenize based on outer curly braces
    while (getline(matrixStream, rowToken, '}')) {
        if (!rowToken.empty()) {
            delimFix(rowToken, "{},");
            vector<int> row = validateRow(rowToken);
            if (row[0] != -1) {
                if (isFirstRow) {
                    matrix.emplace_back(move(row));
                    isFirstRow = false;
                    row_len = matrix[0].size();
                }
                else {
                    // Validate subsequent row length
                    if (row.size() != row_len) {
                        handleError(ErrCode::ROW_LEN_MISMATCH, ErrorContext());
                        return { { 0 } };
                    }
                    matrix.emplace_back(move(row));
                }
            }
            else {
                handleError(ErrCode::INVALID_MAT, ErrorContext());
                return { { 0 } };
            }
        }
    }
    return matrix;
}

vector<vector<vector<int>>> MatFileHandler::parseMatrices(const string &matricesStr) {
    vector<vector<vector<int>>> matrices;
    istringstream matricesStream(matricesStr);
    for (string matStr : strSplit(matricesStr)) {
        if (matStr != " ") {
            currMat = move(parseMat(matStr));
            if (!currMat.empty() && currMat[0][0]) {
                matrices.emplace_back(move(currMat));
            }
            else // fail right after the first invalid matrix / substring
                exit(1);
        }
    }
    return matrices;
}

/*calc diskspace required for N x M matrix files,
    where N = min_rows : row_inc : max_rows
          M = min_cols : col_inc : max_cols
          rowSize = M + M * delimiter + 2(CRLF)
 FileSize = N Rows * rowSize = N(2M+1)
 */
void MatFileHandler::calcSizeMatFiles() const {

    auto adj_rows = adjustedRange(min_rows, max_rows, row_inc);
    auto adj_cols = adjustedRange(min_cols, max_cols, col_inc);

    int r1 = min_rows;
    int rn = adj_rows.first;
    int n_rows = adj_rows.second;

    int c1 = min_cols;
    int cn = adj_cols.first;
    int n_cols = adj_cols.second;

    double sum_rows = (n_rows / 2.0) * (r1 + rn);
    double sum_cols = n_cols * (c1 + cn + 1);

    double totalSize = sum_rows * sum_cols;

    handleDiskSpace(totalSize, MAX_FILE_SIZE_MB);
}

//calc diskspace required for square matrix files NxN
void MatFileHandler::calcSizeSqMatFiles() const {
    auto adj_rows = adjustedRange(min_rows, max_rows, row_inc);
    int rn = adj_rows.first;
    int n = adj_rows.second;

    int r1 = min_rows;
    int d = row_inc;
    int r1_sq = r1 * r1;
    int d_sq = d * d;
    int k = n * (n - 1);

    double sum_rows = (n / 2.0) * (r1 + rn);
    double s1 = n * r1_sq;
    double s2 = r1 * d * k;
    double s3 = d_sq * k * (2 * n - 1) / 6.0;
    double sum_rows_sq = s1 + s2 + s3;

    double totalSize = 2 * sum_rows_sq + sum_rows;

    handleDiskSpace(totalSize, MAX_FILE_SIZE_MB);
}

// Load matrix from file
// Supporting csv/txt comma/tab/semicolon separated ints
bool MatFileHandler::matLoader(vector<vector<int>> &matrix, const string &filepath) {
    ifstream file(filepath, ios::binary);
    if (!file.is_open()) {
        handleError(ErrCode::FILE_OPEN_ERROR, ErrorContext());
        return false;
    }

    size_t file_size = fGetSize(filepath);

    // Read the entire file into a buffer
    vector<char> buffer(file_size);
    file.read(buffer.data(), file_size);
    file.close();

    size_t idx = 0;
    int row_cnt = 0;
    int col_cnt = 0;
    vector<int> row;
    bool in_number = false;
    bool isFirstRow = true;
    size_t row_len = 0;

    // Loop through the buffer and parse integers into rows
    while (idx < buffer.size()) {
        while (idx < buffer.size() && (buffer[idx] != '\n' && buffer[idx] != '\r')) {
            if (isdigit(buffer[idx])) {  // Only process digits (1-9)
                // detect a multi-digit scenario (out of range)
                if (in_number) {
                    handleError(ErrCode::MULTIDIGIT, row_cnt, col_cnt - 1, evalNum(idx - 1, buffer));
                    return false;
                }

                int value = buffer[idx] - '0';

                if (value == 0) {
                    handleError(ErrCode::INVALID_RANGE, value, "Matrix", MIN_VAL, MAX_VAL);
                    return false;
                }

                row.emplace_back(value);
                in_number = true;
                ++col_cnt;
            }
            // Valid delimiters
            else if (buffer[idx] == ' ' || buffer[idx] == ',' || buffer[idx] == ';' || buffer[idx] == '\t') {
                in_number = false;  // We're no longer in the middle of a number
                if (buffer[idx] == '\t')
                    col_cnt += TABWIDTH - (col_cnt % TABWIDTH);
                else
                    ++col_cnt;
            }
            else {
                handleError(ErrCode::INVALID_CHAR, buffer[idx], row_cnt, col_cnt);
                return false;
            }
            ++idx;
        }

        if (!row.empty()) {
            if (isFirstRow) {
                matrix.emplace_back(move(row));
                isFirstRow = false;
                row_len = matrix[0].size();
            }
            else {
                if (row.size() != row_len) {
                    handleError(ErrCode::ROW_LEN_MISMATCH, row_cnt);
                    return false;
                }
                matrix.emplace_back(move(row));
            }
        }

        // Skip CRLF chars at EOL
        while (idx < buffer.size() && (buffer[idx] == '\n' || buffer[idx] == '\r')) {
            in_number = false;
            if (buffer[idx] == '\n')
                ++row_cnt;
            ++idx;
        }
        // Get ready for next row 
        col_cnt = 0;
        row.clear();
    }
    return true;
}

// load matrices, return them & their location
vector<pair<string, vector<vector<int>>>> MatFileHandler::fLoadMatrices() {
    vector<pair<string, vector<vector<int>>>> files;
    bool success = false;

    formatTxt("Loading matrix files - please hang on a minute...\n", LIGHT_CYAN);
    bool directoryEmpty = true;
    int filecount = 0;
    for (const auto &entry : filesystem::directory_iterator(destpath)) {
        const auto &path = entry.path();
        const auto ext = path.extension();
        string filename = path.filename().string();
        size_t output_file = filename.find("out");
        if ((ext == ".txt" || ext == ".csv") && output_file == string::npos) {
            directoryEmpty = false;
            string filepath = path.string();
            ostringstream oss;
            oss << ++filecount << ")" << filepath;
            formatTxt(oss, LIGHT_CYAN);
            vector<vector<int>> mat;
            success = matLoader(mat, filepath);
            cout << endl;
            if (!mat.empty() && success) {
                files.emplace_back(filepath, move(mat));
            }
        }
    }
    if (directoryEmpty) {
        handleError(ErrCode::EMPTY_DIR, destpath);
        exit(1);
    }
    else {
        formatTxt("\nLoading - Complete!\n\n", LIGHT_CYAN);
    }
    return files;
}

//fill buffer with generated random matrix 
vector <char> MatFileHandler::fillBuffRndMat(int rows, int cols, mt19937 gen) {
    return fillBuffer([](int, int) { return true; }, // No condition, all cells are valid
                      [&](int, int) { return rndCharInRange(gen, min_v, max_v); },
                      0, rows - 1, 0, cols - 1
                     );
}

vector<char> MatFileHandler::fillBuffMat() {

    if (currMat.empty() || currMat[0].empty()) {
        handleError(ErrCode::MAT_EMPTY, ErrorContext());
        return {};
    }

    int rows = static_cast<int>(currMat.size());
    int cols = static_cast<int>(currMat[0].size());

    // No condition - all matrix cells are valid!
    auto cellCond = [](int, int) { return true; };
    auto valueCond = [&](int r, int c) { return itoc(currMat[r][c]); };

    return fillBuffer(cellCond, 
                      valueCond,
                      0, rows - 1, 0, cols - 1
                     );
}

vector<char> MatFileHandler::fillBuffRndMat(int rows, int cols) {
    random_device rd;
    mt19937 gen(rd());
    return fillBuffRndMat(rows, cols, gen);
}

//initialize a buffer
vector<char> MatFileHandler::bufferInit(int min_r, int max_r, int min_c, int max_c) {
    int numOfRows = max_r - min_r + 1;
    int numOfCols = max_c - min_c + 1;
    size_t buffSize = numOfRows * (2 * numOfCols + 1);
    vector <char> buffer(buffSize);
    return buffer;
}

//write random-matrix file 
void MatFileHandler::fWriteRndMat(int rows, int cols) {                         
    if (!isValidMatSpec(rows, cols))
        return;
    const string filepath = updatePath(rows, cols, min_v, max_v);
    ostringstream info;
    info << "Matrix File (" << rows << "x" << cols << ")";
    auto buffer = fillBuffRndMat(rows, cols);
    fWrite(buffer, filepath, info.str(), overwrite);
}

//write matrix -> buffer -> file
void MatFileHandler::fWriteMat(const string &filename, bool skipUpdate) {
    if (currMat.empty()) {
        handleError(ErrCode::MAT_EMPTY, ErrorContext());
        return; 
    }
    string filepath = filename;
    if (!skipUpdate)
        filepath = updatePath(filename);
    auto buffer = fillBuffMat();
    fWrite(buffer, filepath, DEFAULT_INFO, overwrite);
}

//Generate N x M matrix files
void MatFileHandler::genMatFiles() {
    calcSizeMatFiles();
    if (!confirm) {
        // files generation action 
        if (skipAction("files generation"))
            return;
    }
    for (int rows = min_rows; rows <= max_rows; rows += row_inc) {
        for (int cols = min_cols; cols <= max_cols; cols += col_inc) {
            fWriteRndMat(rows, cols);
        }
    }
}

//Generate N x N square matrix files 
void MatFileHandler::genSqMatFiles() {

    calcSizeSqMatFiles();
    if (!confirm) {
        // files generation action 
        if (skipAction("files generation"))
            return;
    }
    for (int rows = min_rows; rows <= max_rows; rows += row_inc)
        fWriteRndMat(rows, rows);
}

void MatFileHandler::invokeGenMatFiles() {
    if (sqmat) genSqMatFiles(); else genMatFiles();
}