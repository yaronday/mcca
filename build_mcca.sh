#!/bin/bash

OS=$(uname -s)
WINSTR="MINGW64_NT-10.0*"
PROJ_NAME="mcca"
TARGET_PATH="/mcca_local/mcca_build"
STATIC_BINS="static_binaries"
CONFIG_TYPE="Release"

WIN=$([[ "$OS" == $WINSTR ]] && echo "true" || echo "false")

if [[ "$WIN" == "true" ]]; then
  TARGET_DIR="$USERPROFILE/$TARGET_PATH"
else
  TARGET_DIR="$HOME/$TARGET_PATH"
fi

while [[ $# -gt 0 ]]; do
  case "$1" in
    --help)
      echo "Usage: $0 [--config <config_type>] [--clean] [target_dir]"
      echo "config_types: Release; Debug; RelWithDebInfo; MinSizeRel"
      exit 0
      ;;
    --clean)
      rm -rf build_
      exit 0
      ;;
    --config)
      CONFIG_TYPE="$2"
      shift
      shift
      ;;
    *)
      TARGET_DIR="$1"
      shift
      ;;
  esac
done

if [[ ! -d "build_" ]]; then
  mkdir build_
fi

cd build_

if [[ "$WIN" == "true" ]]; then
  cmake "../$PROJ_NAME"
elif [[ "$OS" == "Darwin" ]]; then
  cmake -DCMAKE_BUILD_TYPE="$CONFIG_TYPE" "../$PROJ_NAME"
else
  echo "Unsupported operating system: $OS"
  exit 1
fi

if [[ "$?" -ne 0 ]]; then
  echo "Error: CMake configuration failed."
  exit 1
fi

cmake --build . --config "$CONFIG_TYPE"

if [[ "$?" -ne 0 ]]; then
  echo "Error: CMake build failed."
  exit 1
fi

if [[ "$WIN" == "true" ]]; then
  if [[ -f "$CONFIG_TYPE/$PROJ_NAME.exe" ]]; then
    mkdir -p "$(cygpath -u "$TARGET_DIR")"
    cp "$CONFIG_TYPE/$PROJ_NAME.exe" "$(cygpath -u "$TARGET_DIR")/$PROJ_NAME.exe"

    if [[ ! -d "../$STATIC_BINS" ]]; then
      mkdir "../$STATIC_BINS"
    fi
    cp "$CONFIG_TYPE/$PROJ_NAME.exe" "../$STATIC_BINS/$PROJ_NAME.exe"
  else
    echo "Error: $PROJ_NAME.exe not found in $CONFIG_TYPE directory."
  fi
elif [[ "$OS" == "Darwin" ]]; then
  if [[ -f "$PROJ_NAME" ]]; then
    mkdir -p "$TARGET_DIR"
    cp "$PROJ_NAME" "$TARGET_DIR/$PROJ_NAME"

    if [[ ! -d "../$STATIC_BINS" ]]; then
      mkdir "../$STATIC_BINS"
    fi
    cp "$PROJ_NAME" "../$STATIC_BINS/$PROJ_NAME"

  else
    echo "Error: $PROJ_NAME executable not found in current directory."
  fi
else
  echo "Unsupported operating system: $OS"
  exit 1
fi

echo "Done."

if [[ "$WIN" == "true" ]]; then
  read -p "Press Enter to continue..."
fi