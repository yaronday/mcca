@echo off

REM Create build directory
if not exist build_msvc mkdir build_msvc
cd build_msvc

REM Configure CMake with Clang (adjust path to mcca subdirectory)
cmake ..\mcca -DCMAKE_CXX_COMPILER=clang++.exe -DCMAKE_C_COMPILER=clang.exe

REM Build the project (Release configuration)
cmake --build . --config Release

REM Copy the executable to the desired location (adjust path to mcca subdirectory)
if exist Release\mcca.exe (
    if not exist C:\mcca_local\mcca_msvc mkdir C:\mcca_local\mcca_msvc
    copy Release\mcca.exe C:\mcca_local\mcca_msvc\mcca.exe
    echo Executable copied successfully to C:\mcca_local\mcca_msvc\mcca.exe
) else (
    echo Error: mcca.exe not found in Release directory.
)

echo Done.
pause