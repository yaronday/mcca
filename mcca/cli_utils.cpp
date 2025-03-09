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

#include "Headers/cli_utils.hpp"

void mainMenuDisplay() {
    title_and_ver();
    formatTxt("Main App:", LIGHT_CYAN);
    cout << "Usage: MCCA [--help][/?][--ver][--cond][--paint] [--no_color][--crop][--visualizer]\n"
        << "            [--root_dir <rootDir>][--algo <algo>][--matrix <matString>]\n"
        << "Options:\n"
        << "  --algo       : Select graph algorithm: DFS, UF or BOTH.\n"
        << "  --paint      : Paint max connected color regions.\n"
        << "  --no_color   : Paint black and white (using unique symbols).\n"
        << "                 If neiter paint nor no_color arg is used, then results will be saved under root_dir.\n"
        << "  --root_dir   : The root and destination directory for processing matrix files and storing results.\n"
        << "  --matrix     : Specify matrices (C/C++ format) as a string, e.g.\n"
        << "                 --matrix \"" << MATSTR << "\"\n"
        << "                 Values allowed: integers " << withinRange(MIN_VAL, MAX_VAL) << ".\n"
        << "  --visualizer : UnionFind Root visualizer. Limited by design choice to "
        << "small matrices with max(rows, cols) <= " << VIS_MAT_THR << ").\n"
        << "                 Graphviz must be installed and added to System Env. Path.\n"
        << "  --crop       : Save max regions (inc. their original coordinates) into files.\n";
    formatTxt("  --help or /? : Display this help menu.", LIGHT_MAGENTA);
    formatTxt("  --cond       : Display conditions.", LIGHT_MAGENTA);
    formatTxt("  --ver        : Display title and version.\n", LIGHT_MAGENTA);
    formatTxt("  Note: unlike Filegen, overwrite is permanently enabled for this operation.\n", LIGHT_YELLOW);
    formatTxt("Example: mcca --root_dir C:/MCCA/Data/matrix_shapes --paint --algo dfs\n", LIGHT_GREEN);
    formatTxt("Example: mcca --root_dir ../data/multicolor --visualizer --crop --algo both\n", LIGHT_GREEN);
    formatTxt("Matrix Files Generator:", LIGHT_CYAN);
    cout << "Usage: mcca filegen [--sqmat] [--confirm] [--ovr] [--root_dir <rootDir>] [--csv]\n"
        << "                    [--minrows <min_rows>] [--maxrows <max_rows>] [--row_inc <row_inc>]\n"
        << "                    [--mincols <min_cols>] [--maxcols <max_cols>] [--col_inc <col_inc>] \n"
        << "                    [--minv <min_v>][--maxv <max_v>]\n"
        << "  --root_dir          : Destination directory for storing the generated files.\n"
        << "  --sqmat             : Generate square matrix files, skipping column-related parameters.\n"
        << "  --confirm           : Automatically confirm saving files to disk.\n"
        << "  --ovr               : Enable overwrite.\n"
        << "  --csv               : Save csv files (otherwise: txt, space separated).\n"
        << "  --minrows <min_rows>: min_rows <= " << ROWS_MAX << ".\n"
        << "  --maxrows <max_rows>: max_rows <= " << ROWS_MAX << ".\n"
        << "  --row_inc <row_inc> : row_inc <= " << INC_MAX << ".\n"
        << "  --mincols <min_cols>: min_cols <= " << ROWS_MAX << ".\n"
        << "  --maxcols <max_cols>: max_cols <= " << ROWS_MAX << ".\n"
        << "  --col_inc <col_inc> : col_inc <= " << INC_MAX << ".\n"
        << "  --minv <min_v>      : min_v " << withinRange(MIN_VAL, MAX_VAL) << ".\n"
        << "  --maxv <max_v>      : max_v " << withinRange(MIN_VAL, MAX_VAL) << ".\n";
    formatTxt("  Note: --sqmat cannot be used with column parameters.\n", LIGHT_YELLOW);
    formatTxt("Examples: mcca filegen --maxrows 750 --maxcols 400 --inc_col 20\n", LIGHT_GREEN);
    formatTxt("          mcca filegen --sqmat --minrows 4000 --maxrows 5000 --row_inc 500 --csv \n", LIGHT_GREEN);
}

void handleArgs(int argc, char *argv[], 
                string &matStr, 
                MatFileHandler &mfh,
                string &algoChoice,  
                bool &paint, bool &colors,
                bool &crop, bool &visualizer) {

    int min_rows = DEFAULT_MIN_R;
    int max_rows = DEFAULT_MAX_R;
    int row_inc = DEFAULT_INC;
    int min_cols = DEFAULT_MIN_R;
    int max_cols = DEFAULT_MAX_R;
    int col_inc = DEFAULT_INC;
    int min_v = DEFAULT_MIN_V;
    int max_v = DEFAULT_MAX_V;

    bool filegen = DEFAULT_FILEGEN;
  
    bool algoSpecified = IS_ALGO_SPECIFIED; 
    bool skip_algo_handler = SKIP_ALGO_HANDLER; 

    // Map to store flags and their values
    map<string, string> args_map;

    unordered_set<string> fg_col_param_flags = { "--mincols", "--maxcols", "--col_inc" }; 

    unordered_set<string> param_flags = { "--algo", "--root_dir", "--matrix",
                                          "--minrows", "--maxrows", "--row_inc", 
                                          "--minv", "--maxv"};

    param_flags.insert(fg_col_param_flags.begin(), fg_col_param_flags.end());
                                
    unordered_set<string> standalone_flags = { "--help", "/?", "--paint", "--no_color", "--csv",
                                               "--crop", "--visualizer", "filegen", "--cond",
                                               "--sqmat", "--confirm", "--ovr", "--ver"};

    // Parse the arguments and map them
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        ranges::transform(arg, arg.begin(), ::tolower);

        if (standalone_flags.contains(arg)) {
            args_map[arg] = "";
        }
        else if (param_flags.contains(arg)) {
            if (i + 1 < argc && !isFlag(argv[i + 1])) {
                args_map[arg] = argv[++i]; 
            }
            else {
                handleError(ErrCode::MISSING_ARG_VALUE, arg);
                cliErrHandler();
            }
        }
        else {
            handleError(ErrCode::UNKNOWN_OPTION, arg);
            cliErrHandler();
        }
    }

    // Validate and process each argument
    for (const auto &[fst, snd] : args_map) {
        const string &arg = fst;
        const string &value = snd;

        if (arg == "--help" || arg == "/?") {
            mainMenuDisplay();
            exit(0);
        }

        if (arg == "--cond") {
            showConditions();
            exit(0);
        }

        if (arg == "--ver") {
            title_and_ver();
            exit(0);
        }

        if (arg == "--paint") {
            paint = true;
        }

        if (arg == "--confirm") {
            mfh.confirm = true; 
        }

        if (arg == "--no_color") {
            paint = true;
            colors = false;
        }

        if (arg == "--crop") {
            crop = true; 
        }

        if (arg == "--csv") { 
            mfh.txt = false;
        }

        if (arg == "--root_dir") {
            if (value.empty()) {
                handleError(ErrCode::MISSING_ARG_VALUE, arg);
                cliErrHandler();
            }
            mfh.destpath = value;
        }

        if (arg == "--algo") {
            if (value.empty()) {
                handleError(ErrCode::MISSING_ARG_VALUE, arg);
                cliErrHandler();
            }       
            handleAlgoSelection(algoChoice, value);
            algoSpecified = true;          
        }

        if (arg == "--matrix") {
            if (value.empty()) {
                handleError(ErrCode::MISSING_ARG_VALUE, arg);
                cliErrHandler();
            }
            matStr = value;
        }

        if (arg == "--visualizer") {
            if (!system("dot -V >nul 2>&1")) {
                visualizer = true;
            }
            else {
                handleError(ErrCode::GRAPHVIZ_NA, ErrorContext());
                cliErrHandler();
            }
        }

        if (arg == "filegen") {
            filegen = true;
            skip_algo_handler = true;
        }

        if (arg == "--sqmat") {
            mfh.sqmat = true;
        }

        if (arg == "--ovr") {
            mfh.overwrite = true;
        }

        if (arg == "--minrows") {
            auto it = args_map.find("--maxrows");
            int maxRowsVal = (it == args_map.end() || it->second.empty()) ? max_rows : stoi(it->second);
            if (maxRowsVal > ROWS_MAX) {
                handleError(ErrCode::INVALID_RANGE, maxRowsVal, arg, ROWS_MIN, ROWS_MAX);
                cliErrHandler();
            }
            validateAndAssign(min_rows, arg, ROWS_MIN, min(ROWS_MAX, maxRowsVal), args_map);
        }

        if (arg == "--maxrows") {
            auto it = args_map.find("--minrows");
            int minRowsVal = (it == args_map.end() || it->second.empty()) ? min_rows : stoi(it->second);
            if (minRowsVal > ROWS_MAX) {
                handleError(ErrCode::INVALID_RANGE, minRowsVal, arg, ROWS_MIN, ROWS_MAX);
                cliErrHandler();
            }
            validateAndAssign(max_rows, arg, max(minRowsVal, ROWS_MIN), ROWS_MAX, args_map);
        }

        if (arg == "--row_inc") {
            validateAndAssign(row_inc, arg, INC_MIN, INC_MAX, args_map);
        }

        if (arg == "--mincols") {
            auto it = args_map.find("--maxcols");
            int maxColsVal = (it == args_map.end() || it->second.empty()) ? max_cols : stoi(it->second);
            if (maxColsVal > ROWS_MAX) {
                handleError(ErrCode::INVALID_RANGE, maxColsVal, arg, ROWS_MIN, ROWS_MAX);
                cliErrHandler();
            }
            validateAndAssign(min_cols, arg, ROWS_MIN, min(ROWS_MAX, maxColsVal), args_map);
        }

        if (arg == "--maxcols") {
            auto it = args_map.find("--mincols");
            int minColsVal = (it == args_map.end() || it->second.empty()) ? min_cols : stoi(it->second);
            if (minColsVal > ROWS_MAX) {
                handleError(ErrCode::INVALID_RANGE, minColsVal, arg, ROWS_MIN, ROWS_MAX);
                cliErrHandler();
            }
            validateAndAssign(max_cols, arg, max(ROWS_MIN, minColsVal), ROWS_MAX, args_map);
        }

        if (arg == "--col_inc") {
            validateAndAssign(col_inc, arg, INC_MIN, INC_MAX, args_map);
        }

        if (arg == "--minv") {
            auto it = args_map.find("--maxv");
            int maxCellVal = (it == args_map.end() || it->second.empty()) ? max_v : stoi(it->second);
            validateAndAssign(min_v, arg, MIN_VAL, min(MAX_VAL, maxCellVal), args_map);
        }

        if (arg == "--maxv") {
            auto it = args_map.find("--minv");
            int minCellVal = (it == args_map.end() || it->second.empty()) ? min_v : stoi(it->second);
            validateAndAssign(max_v, arg, max(minCellVal, MIN_VAL), MAX_VAL, args_map);
        }

    }

    if (!algoSpecified && !skip_algo_handler)
        handleAlgoSelection(algoChoice);

    createDir(mfh.destpath);

    if (filegen) {

        mfh.updateMatParams(min_rows, max_rows, row_inc, min_cols, max_cols, col_inc, min_v, max_v);

        formatTxt("Matrix Files Generator activated!", LIGHT_GREEN);

        if (mfh.sqmat) {
            for (const auto &col_flag : fg_col_param_flags) {
                if (!args_map[col_flag].empty()) {
                    formatTxt("Error: column params cannot be used with --sqmat.", LIGHT_RED);
                    exit(1);
                }
            }
        }

        mfh.invokeGenMatFiles();
        exit(0);
    }
}

void validateAndAssign(int &variable, const string &arg, int min_value, 
                       int max_value, map<string, string> &args_map) {
    auto it = args_map.find(arg);
    if (it != args_map.end() && !it->second.empty()) {
        string st = it->second;
        if (isNumeric(st)) {
            int value = stoi(st);
            if (value < min_value || value > max_value) {
                handleError(ErrCode::INVALID_RANGE, value, 
                            arg, min_value, max_value);
                cliErrHandler();
            }
            variable = value;
        }
        else {
            handleError(ErrCode::INT_EXPECTED, arg);
            cliErrHandler();
        }
    }
    else {
        handleError(ErrCode::MISSING_ARG_VALUE, arg);
        cliErrHandler();
    }
}

void cliErrHandler(bool menu_disp) {
    if (menu_disp)
        mainMenuDisplay();
    exit(1);
}

bool isFlag(const string &str) {
    return str[0] == '-' && (str.length() > 1 && (str[1] == '-' || isalpha(str[1])));
}

bool isInvalidAlgoChoice(const string &algoChoice) {
    return (algoChoice != "DFS" && algoChoice != "UF" && algoChoice != "BOTH");
}

void handleAlgoSelection(string &algo, const string &value) {
    algo = strToUpper(value);
    if (isInvalidAlgoChoice(algo)) {
        handleError(ErrCode::INVALID_ALGO_SELECTION, ErrorContext());
        cliErrHandler();
    }
}

void algoNotifier(const string &algo) {
    string algorithmText = (algo == "BOTH") ? " algorithms were" : " algorithm was";
    ostringstream oss;
    oss << algo << algorithmText << " selected!\n";
    formatTxt(oss, LIGHT_GREEN);
}

void visualizerNotifier(const string &algo, bool &visualizerEnabled) {
    if (visualizerEnabled){
        if (algo != "UF" && algo != "BOTH") {
            // warn the user 
            formatTxt("Disabling Visualizer since UF was not included.\n", LIGHT_YELLOW);
            visualizerEnabled = false;
        }
    }
}

void title_and_ver() {
    ostringstream info;
    info << TITLE << VERSION << COPYRIGHT;
    formatTxt(info, LIGHT_CYAN);
} 