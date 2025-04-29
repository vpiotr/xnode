# Contributing to xnode

Thank you for your interest in contributing to xnode! This document provides guidelines and instructions for contributing to this project.

## Building the Project

xnode uses CMake as its build system. Follow these steps to build the project:

### Prerequisites

- CMake 3.14 or higher
- C++11 compatible compiler (GCC, Clang, MSVC)

### Build Steps

1. **Clone the repository**:
   ```bash
   git clone https://github.com/plikus/xnode.git
   cd xnode
   ```

2. **Create a build directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Configure the project**:
   ```bash
   cmake ..
   ```

4. **Build the project**:
   ```bash
   cmake --build .
   ```

5. **Run the tests**:
   ```bash
   ctest
   ```

### CMake Options

The following options can be used to customize the build:

- `XNODE_BUILD_TESTS` - Build tests (ON by default)

Example:
```bash
cmake -DXNODE_BUILD_TESTS=OFF ..
```

## Testing

The project includes a comprehensive test suite. To run the tests:

```bash
cd build
ctest
```

To run specific tests with verbose output:

```bash
ctest --verbose --tests-regex TestRegex
```

## Code Style

Please follow these style guidelines when contributing:

- Use consistent indentation (4 spaces)
- Follow the existing naming conventions
- Add comments for complex code sections
- Keep lines to a reasonable length (around 100 characters)

## Pull Request Process

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Run the tests to ensure they still pass
5. Commit your changes (`git commit -m 'Add some amazing feature'`)
6. Push to the branch (`git push origin feature/amazing-feature`)
7. Open a Pull Request

## Reporting Issues

When reporting issues, please include:

- A clear description of the problem
- Steps to reproduce the issue
- Expected vs. actual behavior
- System information (OS, compiler, CMake version)

## License

By contributing, you agree that your contributions will be licensed under the project's BSD license.