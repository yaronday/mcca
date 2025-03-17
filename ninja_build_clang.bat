@echo off

REM Create build directory
if not exist build_clang_ninja mkdir build_clang_ninja
cd build_clang_ninja

REM Configure CMake with Clang and Ninja
cmake ..\mcca -G Ninja -DCMAKE_CXX_COMPILER=clang++.exe -DCMAKE_C_COMPILER=clang.exe -DCMAKE_BUILD_TYPE=Release

REM Build the project using Ninja
ninja

echo Done.
pause