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

#include "Headers/file_utils.hpp"
#include "Headers/common.hpp"


// calc directory size except for file names specified by filenameFilter
size_t calcDirSize(const string &directoryPath, const string &fnameFilter) {
    size_t dirByteSize = 0;
    try {
        for (const auto &entry : filesystem::recursive_directory_iterator(directoryPath)) {
            const auto &path = entry.path();
            const auto ext = path.extension();
            string filename = path.filename().string();
            size_t output_file = filename.find(fnameFilter);
            if (filesystem::is_regular_file(entry) && output_file == string::npos) {
                dirByteSize += filesystem::file_size(entry);
            }
        }
    }
    catch (const exception &e) {
        handleError(ErrCode::GENERIC_EXCEPTION, e);
        exit(1);
    }
    return dirByteSize;
}

//create dir if necessary
void createDir(const string &root_dir) {
    try {
        if (!filesystem::exists(root_dir)) {
            if (!filesystem::create_directories(root_dir)) {
                handleError(ErrCode::CDIR_FAILURE, root_dir);
                exit(1);
            }
        }
        return;
    }
    catch (const exception &e) {
        handleError(ErrCode::GENERIC_EXCEPTION, e);
        exit(1);
    }
}

bool isValidDir(const string &dir) {
    if (!filesystem::is_directory(dir)) {
        handleError(ErrCode::INVALID_DIR, dir);
        return false;
    }
    return true;
}

size_t fGetSize(const string& filepath) {
    ifstream file(filepath, ios::binary | ios::ate);
    if (!file.is_open()) {
        handleError(ErrCode::FILE_OPEN_ERROR, ErrorContext());
        exit(1);
    }
    return static_cast<size_t>(file.tellg());
}

//Write to file
void fWrite(const string &contents,
            const string &filepath,
            const string &info, 
            bool overwrite) {
    if (contents == "")
        return;
    fHandleInfo(filepath, info, overwrite);
    ofstream file(filepath);
    if (!file.is_open()) {
        handleError(ErrCode::FILE_OPEN_ERROR, ErrorContext());
        return;
    }
    file << contents;
    file.close();
}

//Write to file directly from buffer 
void fWrite(vector <char> buffer,
            const string &filepath,
            const string &info, 
            bool overwrite) {
    if (buffer.empty())
        return;
    fHandleInfo(filepath, info, overwrite);     
    ofstream file(filepath, ios::binary);
    if (!file.is_open()) {
        handleError(ErrCode::FILE_OPEN_ERROR, ErrorContext());
        return;
    }
    file.write(buffer.data(), buffer.size());
    file.close();
}

void fHandleInfo(const string &filepath,
                 const string &info,
                 bool overwrite) {
    ifstream infile(filepath);
    bool fileExists = infile.good();
    infile.close();
    if ((!overwrite) && fileExists) {
        handleError(ErrCode::OVR_NOT_ENABLED, ErrorContext(filepath));
        return;
    }
    ostringstream oss;
    const string actionPrefix = fileExists ? "Overwriting" : "Writing";
    oss << actionPrefix << " " << info << ": " << filepath;
    formatTxt(oss, LIGHT_GREEN);
}

string fGenPath(const string &filename, 
                const string &suffix, 
                bool txt) {
    const string ext = txt ? "txt" : "csv";
    ostringstream full_filepath;
    full_filepath << fRemoveExt(filename) << suffix << "." << ext;
    return full_filepath.str();
}

string fRemoveExt(const string &filename) {
    return filename.substr(0, filename.length() - EXT_LEN - 1);
}