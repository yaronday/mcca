#!/bin/bash

OS=$(uname -s)


if [[ "$OS" == MINGW64_NT-10.0* ]]; then 
  TARGET_DIR="$USERPROFILE/mcca_local/mcca_build"
else # macOS or Linux
  TARGET_DIR="$HOME/mcca_local/mcca_build"
fi

# Check if a target directory is provided as a cli param
if [ $# -gt 0 ]; then
  TARGET_DIR="$1"
fi

if [ ! -d "build_" ]; then
    mkdir build_
fi

cd build_

if [[ "$OS" == MINGW64_NT-10.0* ]]; then 
  cmake ../mcca
elif [[ "$OS" == "Darwin" ]]; then 
  cmake -DCMAKE_BUILD_TYPE=Release ../mcca
else
  echo "Unsupported operating system: $OS"
  exit 1
fi

cmake --build . --config Release

if [[ "$OS" == MINGW64_NT-10.0* ]]; then 
  if [ -f "Release/mcca.exe" ]; then
    mkdir -p "$(cygpath -u "$TARGET_DIR")"
    cp "Release/mcca.exe" "$(cygpath -u "$TARGET_DIR")/mcca.exe"
  else
    echo "Error: mcca.exe not found in Release directory."
  fi
elif [[ "$OS" == "Darwin" ]]; then
  if [ -f "mcca" ]; then
    mkdir -p "$TARGET_DIR"
    cp "mcca" "$TARGET_DIR/mcca"
  else
    echo "Error: mcca executable not found in current directory."
  fi
else
  echo "Unsupported operating system: $OS"
  exit 1
fi

echo "Done."

if [[ "$OS" == MINGW64_NT-10.0* ]]; then
  read -p "Press Enter to continue..."
fi