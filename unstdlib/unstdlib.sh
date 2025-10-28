#!/bin/bash

# ------------------ [dependency check] ------------------
# Function to check if gcc supports -std=c11
check_std_c11_support() {
  if gcc -v --help 2>/dev/null | grep -iv deprecated | grep "C " | sed -n '/^ *-std=\([^<][^ ]\+\).*/ {s//\1/p}' | grep -q "c11"; then
    return 0 # True
  else
    return 1 # False
  fi
}

# Check if gcc version 11 is installed
if gcc --version >/dev/null 2>&1; then
  gcc_version=$(gcc --version | head -n1 | awk '{print $3}')
  if [[ $gcc_version =~ ^11\..* ]]; then
    echo "GCC version 11 is installed: gcc $gcc_version"
  else
    # Check if gcc supports -std=c11
    if check_std_c11_support; then
      echo "GCC supports -std=c11."
    else
      echo "GCC does not support -std=c11."
      exit 1
    fi

    #    echo 'int main() { return 0; }' >test.c
    #    if gcc -std=c11 -o test test.c >/dev/null 2>&1; then
    #      echo "GCC supports -std=c11."
    #      rm -f test test.c
    #    else
    #      echo "GCC does not support -std=c11."
    #      echo "GCC is installed, but the version is not 11.x: gcc $gcc_version"
    #      rm -f test.c
    #      exit 1
    #    fi
  fi
else
  echo "GCC is not installed."
  exit 1
fi

# Check if cmake is installed
if cmake --version >/dev/null 2>&1; then
  cmake_version=$(cmake --version | head -n1 | awk '{print $3}')
  echo "CMake is installed: cmake $cmake_version"
else
  echo "CMake is not installed."
  exit 1
fi

# ------------------ [installation check] ------------------
# Default paths
LIB_FILES_DESTINATION_PATH="/usr/local/lib"
HEADER_FILES_SOURCE_PATH="./src"
HEADER_FILES_DESTINATION_PATH="/usr/local/include/unstdlib"
BUILD_PATH="./build"
TEST_MODE=false
INSTALL_MODE=false
BUILD_MODE=false

# Help menu function
print_help() {
  echo "Usage: $0 [OPTIONS] [LIB_FILES_DESTINATION_PATH] [HEADER_FILES_DESTINATION_PATH]"
  echo "Options:"
  echo "  -install   <dst_lib_folder> <dst_header_folder> : Build the project as Release and"
  echo "                                                    move libraries and headers to"
  echo "                                                   [LIB_FILES_DESTINATION_PATH] [HEADER_FILES_DESTINATION_PATH]"

  echo "  -uninstall <dst_lib_folder> <dst_header_folder> : Removes the project by searching in"
  echo "                                                    default paths [LIB_FILES_DESTINATION_PATH] [HEADER_FILES_DESTINATION_PATH]"

  echo "  -build     <dst_build_folder>                   : Only build the project as Release and"
  echo "                                                    move libraries and headers to [BUILD_PATH]"

  echo "  -cleanup   : Removes the built project folders."
  echo "  -test      : Build the project as Debug and run executables in ./cmake-build-debug"
  echo "  -h         : Show this help message"
  echo "Arguments:"
  echo "  LIB_FILES_DESTINATION_PATH    : Path to where the libraries should be installed. default=\"$LIB_FILES_DESTINATION_PATH\""
  echo "  HEADER_FILES_DESTINATION_PATH : Path to where the headers should be installed.   default=\"$HEADER_FILES_DESTINATION_PATH\""
  echo "  BUILD_PATH                    : Path to where the libraries and headers should be installed upon -build. default=\"$BUILD_PATH\""
}

# Function to prompt the user to confirm paths
confirm_paths() {
  #  read -p "Install libraries to '$LIB_FILES_DESTINATION_PATH' and headers to '$HEADER_FILES_DESTINATION_PATH'. Continue the operation? [y/N]: " choice
  read -p "$1. Continue the operation? [y/N]: " choice
  choice=${choice:-N}
  case "$choice" in
  y | Y) ;;
  *)
    echo "Installation aborted. Use (-h, --help) for more information on how to specify paths."
    exit 0
    ;;
  esac
}

# Function to check if directory is empty
is_dir_empty() {
  if [ "$(ls -A "$1" 2>/dev/null)" ]; then
    return 1
  else
    return 0
  fi
}

invalid_path() {
  if [[ "$1" == */ ]]; then
    return 0
  else
    return 1
  fi
}

case $1 in
-test)
  TEST_MODE=true
  shift
  ;;
-h | --help)
  print_help
  exit 0
  ;;
-install)
  INSTALL_MODE=true
  if [[ ! "$LIB_PATH_SPECIFIED" && "$2" ]]; then
    LIB_FILES_DESTINATION_PATH="$2"
    LIB_PATH_SPECIFIED=true
  fi
  if [[ ! "$HEADER_FILES_DESTINATION_PATH_SPECIFIED" && "$3" ]]; then
    if [[ "$HEADER_FILES_DESTINATION_PATH" == */ || "$3" == */ ]]; then
      HEADER_FILES_DESTINATION_PATH="$3unstdlib"
    else
      HEADER_FILES_DESTINATION_PATH="$3/unstdlib"
    fi
    HEADER_FILES_DESTINATION_PATH_SPECIFIED=true
  fi
  shift
  ;;
-build)
  BUILD_MODE=true
  if [[ ! "$BUILD_PATH_SPECIFIED" && "$2" ]]; then
    BUILD_PATH="$2"
    BUILD_PATH_SPECIFIED=true
  fi

  if invalid_path "$BUILD_PATH"; then
    last_copy=$BUILD_PATH
    echo "Bad path detected, fixing the path..."
    while invalid_path "$BUILD_PATH"; do
      BUILD_PATH=${BUILD_PATH::-1}
    done
    echo "Path fixed."
    echo "$last_copy -changed-to-> $BUILD_PATH"
  fi

  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G "Unix Makefiles" -S ./ -B "$BUILD_PATH"

  echo "Building the project..."
  cmake --build "$BUILD_PATH"

  # Find and delete all files that do not have .so, .so.*, or .a extensions
  find "$BUILD_PATH" -maxdepth 1 -mindepth 1 -type f ! \( -name "*.so" -o -name "*.so.*" -o -name "*.a" \) -exec rm -rf '{}' +
  find "$BUILD_PATH" -maxdepth 1 -mindepth 1 -type d -exec rm -rf '{}' \;

  mkdir -p "$BUILD_PATH"/include/unstdlib
  cp -v "$HEADER_FILES_SOURCE_PATH"/*unstd*.h "$BUILD_PATH"/include/unstdlib

  echo "Build finished."

  exit 0
  ;;
-cleanup)
  rm -rf ./cmake-build-release
  rm -rf ./cmake-build-debug
  rm -rf ./build

  echo "Cleanup finished."
  exit 0
  ;;
-uninstall)
  if [[ ! "$LIB_PATH_SPECIFIED" && "$2" ]]; then
    LIB_FILES_DESTINATION_PATH="$2"
    LIB_PATH_SPECIFIED=true
  fi
  if [[ ! "$HEADER_FILES_DESTINATION_PATH_SPECIFIED" && "$3" ]]; then
    if [[ "$HEADER_FILES_DESTINATION_PATH" == */ || "$3" == */ ]]; then
      HEADER_FILES_DESTINATION_PATH="$3unstdlib"
    else
      HEADER_FILES_DESTINATION_PATH="$3/unstdlib"
    fi
    HEADER_FILES_DESTINATION_PATH_SPECIFIED=true
  fi

  confirm_paths "Remove libraries from '$LIB_FILES_DESTINATION_PATH' and headers from '$HEADER_FILES_DESTINATION_PATH'"

  echo "Using library (.a|.so) destination path: $LIB_FILES_DESTINATION_PATH"
  echo "Using include (.h)     destination path: $HEADER_FILES_DESTINATION_PATH"

  echo "Removing existing libraries in $LIB_FILES_DESTINATION_PATH..."
  if ! is_dir_empty "$LIB_FILES_DESTINATION_PATH"/*unstd*.{a,so*}; then
    sudo rm -rf "$LIB_FILES_DESTINATION_PATH"/*unstd*.{a,so*}
    echo "Headers removed successfully."
  else
    echo "No libraries found within $LIB_FILES_DESTINATION_PATH to remove."
  fi

  echo "Removing existing headers in $HEADER_FILES_DESTINATION_PATH..."
  if ! is_dir_empty "$HEADER_FILES_DESTINATION_PATH"/*unstd*.h; then
    sudo rm -rf "$HEADER_FILES_DESTINATION_PATH"/*unstd*.h
    echo "Headers removed successfully."
  else
    echo "No headers found within $HEADER_FILES_DESTINATION_PATH to remove."
  fi

  echo "Uninstall finished."

  exit 0
  ;;
*)
  print_help
  exit 0
  ;;
esac

if invalid_path "$HEADER_FILES_DESTINATION_PATH"; then
  last_copy=$HEADER_FILES_DESTINATION_PATH
  echo "Bad path detected, fixing the path..."
  while invalid_path "$HEADER_FILES_DESTINATION_PATH"; do
    HEADER_FILES_DESTINATION_PATH=${HEADER_FILES_DESTINATION_PATH::-1}
  done
  echo "Path fixed."
  echo "$last_copy -changed-to-> $HEADER_FILES_DESTINATION_PATH"
fi

if invalid_path "$LIB_FILES_DESTINATION_PATH"; then
  last_copy=$LIB_FILES_DESTINATION_PATH
  echo "Bad path detected, fixing the path..."
  while invalid_path "$LIB_FILES_DESTINATION_PATH"; do
    LIB_FILES_DESTINATION_PATH=${LIB_FILES_DESTINATION_PATH::-1}
  done
  echo "Path fixed."
  echo "$last_copy -changed-to-> $LIB_FILES_DESTINATION_PATH"
fi

# Create build directory if it doesn't exist
if [ "$TEST_MODE" == "true" ]; then
  BUILD_DIR="./cmake-build-debug"
elif [ "$INSTALL_MODE" == "true" ]; then
  confirm_paths "Install libraries to '$LIB_FILES_DESTINATION_PATH' and headers to '$HEADER_FILES_DESTINATION_PATH'"
  echo "Using library (.a|.so) destination path: $LIB_FILES_DESTINATION_PATH"
  echo "Using include (.h)     destination path: $HEADER_FILES_DESTINATION_PATH"
  BUILD_DIR="./cmake-build-release"
else
  echo "Something went wrong."
  exit 1
fi

if [ ! -d "$BUILD_DIR" ]; then
  mkdir -p "$BUILD_DIR"
  echo "Created build directory: $BUILD_DIR"
fi

# Check if a build has been done before
if [ ! -f "$BUILD_DIR/Makefile" ]; then
  echo "Configuring the build with CMake..."
  if [ "$TEST_MODE" == "true" ]; then
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G "Unix Makefiles" -S ./ -B "$BUILD_DIR"
  else
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G "Unix Makefiles" -S ./ -B "$BUILD_DIR"
  fi
else
  echo "Build configuration already exists."
fi

echo "Building the project..."
cmake --build "$BUILD_DIR"

# If test mode is enabled, run the executables and exit
if [ "$TEST_MODE" == "true" ]; then
  #  for file in "$BUILD_DIR"/test*unstd*; do
  #    if [ -x "$file" ] && [[ "$file" == *"unstd"* ]]; then
  #      echo "Running $file..."
  #      "$file"
  #    fi
  #  done
  "$BUILD_DIR"/test_all
  exit 0
fi

# Ensure the target directories exist
if [ ! -d "$LIB_FILES_DESTINATION_PATH" ]; then
  echo "Creating library path: $LIB_FILES_DESTINATION_PATH"
  sudo mkdir -p "$LIB_FILES_DESTINATION_PATH"
fi

if [ ! -d "$HEADER_FILES_DESTINATION_PATH" ]; then
  echo "Creating include path: $HEADER_FILES_DESTINATION_PATH"
  sudo mkdir -p "$HEADER_FILES_DESTINATION_PATH"
fi

# Function to prompt the user to replace existing files
prompt_replace() {
  read -p "Seems like unstdlib $2 were already built in $1. Do you wish to replace it with the current build? [y/N]: " choice
  choice=${choice:-N}
  case "$choice" in
  y | Y) return 0 ;;
  n | N) return 1 ;;
  *)
    echo "Invalid input. Please enter 'y' or 'n'."
    prompt_replace "$1"
    ;;
  esac
}

INSTALL_LIBS_FLAG=true
INSTALL_HEADERS_FLAG=true

# Check if libraries directory is empty
if ! is_dir_empty "$LIB_FILES_DESTINATION_PATH"/*unstd*.{a,so*}; then
  prompt_replace "$LIB_FILES_DESTINATION_PATH" "libraries"
  replace=$?
  if [ $replace -eq 0 ]; then
    echo "Removing existing libraries in $LIB_FILES_DESTINATION_PATH..."
    sudo rm -rf "$LIB_FILES_DESTINATION_PATH"/*unstd*.{a,so*}
    echo "Libraries removed successfully."
  else
    echo "Skipping the installation of libraries."
    INSTALL_LIBS_FLAG=false
  fi
fi

if [ "$INSTALL_LIBS_FLAG" == "true" ]; then
  # Move libraries to the specified path
  echo "Moving libraries to $LIB_FILES_DESTINATION_PATH..."
  if sudo cp -v "$BUILD_DIR"/*unstd*.{a,so*} "$LIB_FILES_DESTINATION_PATH"; then
    echo "Libraries moved successfully."
  else
    echo "Failed to move libraries. Please check permissions or run the script with sudo."
    exit 1
  fi
fi

# Check if headers directory is empty
if ! is_dir_empty "$HEADER_FILES_DESTINATION_PATH"/*unstd*.h; then
  prompt_replace "$HEADER_FILES_DESTINATION_PATH" "headers"
  replace=$?
  if [ $replace -eq 0 ]; then
    echo "Removing existing headers in $HEADER_FILES_DESTINATION_PATH..."
    sudo rm -rf "$HEADER_FILES_DESTINATION_PATH"/*unstd*.h
    echo "Headers removed successfully."
  else
    echo "Skipping the installation of headers."
    INSTALL_HEADERS_FLAG=false
  fi
fi

if [ "$INSTALL_HEADERS_FLAG" == "true" ]; then
  # Move headers to the specified path
  echo "Moving headers to $HEADER_FILES_DESTINATION_PATH..."
  if sudo cp -v "$HEADER_FILES_SOURCE_PATH"/*unstd*.h "$HEADER_FILES_DESTINATION_PATH"; then
    echo "Headers moved successfully."
  else
    echo "Failed to move headers. Please check permissions or run the script with sudo."
    exit 1
  fi
fi

echo "Build and installation complete."
