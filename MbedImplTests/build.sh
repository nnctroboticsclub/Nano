#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "Building MbedImpl Tests..."
echo "This requires ARM toolchain and StaticMbedOS package"
echo ""

# Create build directory
mkdir -p "${BUILD_DIR}"

# Configure
echo "Configuring..."
cmake -G Ninja \
  -S "${SCRIPT_DIR}" \
  -B "${BUILD_DIR}" \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo

# Build
echo "Building..."
cmake --build "${BUILD_DIR}" -j$(nproc)

echo ""
echo "Build complete!"
echo "Binaries are in: ${BUILD_DIR}/tests/"
echo ""
echo "To flash to NUCLEO_F446RE:"
echo "  1. Connect the board via USB"
echo "  2. Copy .bin file to the board's mass storage device"
echo "  3. Connect to serial port at 115200 baud"
echo ""
