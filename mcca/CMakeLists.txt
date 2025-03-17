cmake_minimum_required(VERSION 3.29.5)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    #windows specific code
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    #Clang specific code
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

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
	Resource.rc
)

target_include_directories(mcca PRIVATE Common/Headers MaxConnectedColorsInGrid/Headers)