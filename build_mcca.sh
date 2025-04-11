#!/bin/bash

OS=$(uname -s)
WINSTR="MINGW64_NT-10.0*"
BUILD_DIR="build"
PROJ_NAME="mcca"
EXE_MAC="${PROJ_NAME}_mac"
TARGET_PATH="mcca_local/mcca_$BUILD_DIR"
STATIC_BINS="static_binaries"
BUILD_TYPE="Release"

WIN=$([[ "$OS" == $WINSTR ]] && echo "true" || echo "false")

if [[ "$WIN" == "true" ]]; then
  TARGET_DIR="$USERPROFILE/$TARGET_PATH"
else
  TARGET_DIR="$HOME/$TARGET_PATH"
fi

while [[ $# -gt 0 ]]; do
  case "$1" in
    --help|"/?")
      echo "Usage: $0 [--build <build_type>] [--clean] [target_dir]"
	  echo "  --clean      Removes the build directory and all generated files."
	  echo "  build_types: Release; Debug; RelWithDebInfo; MinSizeRel"
      exit 0
      ;;
    --clean)
      if [[ -d "$BUILD_DIR" ]]; then
        rm -rf "$BUILD_DIR"
      else
        echo "No build directory found, skipping clean."
      fi
      exit 0
      ;;
    --build)
      BUILD_TYPE="$2"
      shift 2
      ;;
    -*)
      echo "Unknown option: $1" >&2
      exit 1
      ;;
    *)
      TARGET_DIR="$1"
      shift
      ;;
  esac
done

if [[ ! -d "$BUILD_DIR" ]]; then
  mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR" || { echo "Failed to change directory to $BUILD_DIR" >&2; exit 1; }

if [[ "$WIN" == "true" ]]; then
  cmake "../$PROJ_NAME"
elif [[ "$OS" == "Darwin" ]]; then
  cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" "../$PROJ_NAME"
else
  echo "Unsupported operating system: $OS" >&2
  exit 1
fi

if [[ "$?" -ne 0 ]]; then
  echo "Error: CMake configuration failed." >&2
  exit 1
fi

cmake --build . --config "$BUILD_TYPE"

if [[ "$?" -ne 0 ]]; then
  echo "Error: CMake build failed." >&2
  exit 1
fi

if [[ "$WIN" == "true" ]]; then
  if [[ -f "$BUILD_TYPE/$PROJ_NAME.exe" ]]; then
    mkdir -p "$(cygpath -u "$TARGET_DIR")"
    cp "$BUILD_TYPE/$PROJ_NAME.exe" "$(cygpath -u "$TARGET_DIR")/$PROJ_NAME.exe"

    if [[ ! -d "../$STATIC_BINS" ]]; then
      mkdir "../$STATIC_BINS"
    fi
    cp "$BUILD_TYPE/$PROJ_NAME.exe" "../$STATIC_BINS/$PROJ_NAME.exe"
  else
    echo "Error: $PROJ_NAME.exe not found in $BUILD_TYPE directory." >&2
  fi
elif [[ "$OS" == "Darwin" ]]; then
  if [[ -f "$PROJ_NAME" ]]; then
    mkdir -p "$TARGET_DIR"
    cp "$PROJ_NAME" "$TARGET_DIR/$EXE_MAC"

    if [[ ! -d "../$STATIC_BINS" ]]; then
      mkdir "../$STATIC_BINS"
    fi
    cp "$PROJ_NAME" "../$STATIC_BINS/$EXE_MAC"
  else
    echo "Error: $PROJ_NAME executable not found in current directory." >&2
  fi
else
  echo "Unsupported operating system: $OS" >&2
  exit 1
fi

echo "Done."

if [[ "$WIN" == "true" ]]; then
  read -p "Press Enter to continue..."
fi