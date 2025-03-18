cmake_minimum_required(VERSION 3.29.5)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

project(mcca)

add_executable(mcca
        ../common/common.cpp
        ../common/file_utils.cpp
        ../common/helpers.cpp
        ../common/matfile_handler.cpp
    cli_utils.cpp
    color_grid.cpp
    main.cpp
    max_color_dfs.cpp
    max_color_uf.cpp
    union_find.cpp
    Resource.rc
)

target_include_directories(mcca PRIVATE common/headers mcca/headers)
