# Configure
cmake -B build

# Build
cmake --build build

# Test with verbose output
cd build && ctest -V