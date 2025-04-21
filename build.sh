#!/bin/bash
set -e  # Exit immediately if a command fails

BUILD_TYPE=${1:-Debug}  # Accepts "Debug" or "Release" as an argument, defaults to Debug

if [[ "$BUILD_TYPE" != "Debug" && "$BUILD_TYPE" != "Release" ]]; then
    echo "❌ Invalid build type: $BUILD_TYPE. Use 'Debug' or 'Release'."
    exit 1
fi

BUILD_DIR="build/$(echo $BUILD_TYPE | tr '[:upper:]' '[:lower:]')"  # Lowercase directory name

echo "🧹 Cleaning previous build..."
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR

# Install dependencies with Conan
echo "📦 Installing dependencies with Conan for $BUILD_TYPE build..."
conan install . --output-folder=$BUILD_DIR --build=missing -s build_type=$BUILD_TYPE

# Run CMake to configure the project
echo "⚙️ Running CMake for $BUILD_TYPE build..."
cmake -S . -B $BUILD_DIR -DCMAKE_TOOLCHAIN_FILE=$BUILD_DIR/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build the project and tests
echo "🔨 Building project and all test executables..."
cmake --build $BUILD_DIR

# Symlink compile_commands.json to the project root
echo "🔗 Creating symbolic link for compile_commands.json..."
ln -sf $BUILD_DIR/compile_commands.json ./compile_commands.json

echo "✅ Build completed successfully for $BUILD_TYPE build!"

echo "▶️ Running tests..."
cd $BUILD_DIR
ctest --output-on-failure --extra-verbose
