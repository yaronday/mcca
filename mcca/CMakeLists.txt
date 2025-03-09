cmake_minimum_required(VERSION 3.30.5)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mcca)

add_executable(mcca
    ../Common/common.cpp
    ../Common/file_utils.cpp
    ../Common/helpers.cpp
    ../Common/matfile_handler.cpp
    cli_utils.cpp
    color_grid.cpp
    main.cpp
    max_color_dfs.cpp
    max_color_uf.cpp
    union_find.cpp
)

target_include_directories(mcca PRIVATE Common/Headers MaxConnectedColorsInGrid/Headers)