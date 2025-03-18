#!/bin/bash


OS=$(uname -s)

if [ ! -d "build_" ]; then
  mkdir build_
fi

cd build_

# let CMake detect the compiler
if [[ "$OS" == MINGW64_NT-10.0* ]]; then
  cmake ../mcca
elif [[ "$OS" == "Darwin" ]]; then
  cmake ../mcca
else
  echo "Unsupported operating system: $OS"
  exit 1
fi

cmake --build . --config Release

# Copy executable file to local target if exists
if [[ "$OS" == MINGW64_NT-10.0* ]]; then 
  if [ -f "Release/mcca.exe" ]; then
    if [ ! -d "C:/mcca_local/mcca_build" ]; then
      mkdir -p "C:/mcca_local/mcca_build"
    fi
    cp "Release/mcca.exe" "C:/mcca_local/mcca_build/mcca.exe"
  else
    echo "Error: mcca.exe not found in Release directory."
  fi
elif [[ "$OS" == "Darwin" ]]; then
  if [ -f "mcca" ]; then 
    if [ ! -d "~/mcca_local/mcca_build" ]; then
        mkdir -p "~/mcca_local/mcca_build"
    fi
    cp "mcca" "~/mcca_local/mcca_build/mcca"
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