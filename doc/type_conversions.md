# xnode Type Conversion Reference

## Overview

xnode provides automatic type conversion between various built-in types. This document outlines the supported conversions based on the implementation in `xnode_builtins.h` and the test cases in `xnode_convert_test.cpp`.

## Supported Types

The following types are supported for storage and conversion:

- **Booleans**: `bool`
- **Floating Point**: `float`, `double`
- **Signed Integers**: `char`, `short`, `int`, `long`, `long long`
- **Unsigned Integers**: `unsigned char`, `unsigned short`, `unsigned int`, `unsigned long`, `unsigned long long`
- **Strings**: `std::string`

## Conversion Matrix

The table below shows the supported conversions between types. Each cell indicates whether conversion from the source type (row) to the destination type (column) is supported.

| From ↓ To → | bool | float | double | char | short | int | long | long long | uchar | ushort | uint | ulong | ullong | string |
|-------------|------|-------|--------|------|-------|-----|------|-----------|-------|--------|------|-------|--------|--------|
| bool        | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |
| float       | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |
| double      | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |
| char        | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |
| short       | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |
| int         | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |
| long        | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |
| long long   | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |
| uchar       | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |
| ushort      | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |
| uint        | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |
| ulong       | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |
| ullong      | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |
| string      | ✓    | ✓     | ✓      | ✓    | ✓     | ✓   | ✓    | ✓         | ✓     | ✓      | ✓    | ✓     | ✓      | ✓      |

## Conversion Details

### Boolean Conversions

- **To Boolean**:
  - Numeric types: `0` converts to `false`, any other value converts to `true`
  - String: `"true"` or `"1"` converts to `true`, other values convert to `false`

### Numeric Conversions

- **Between numeric types**: 
  - Standard C++ casting rules apply with additional runtime safety checks
  - Conversions that would cause overflow or underflow are detected and throw exceptions:
    - Converting a larger integer type to a smaller type that cannot represent the value
    - Converting a floating-point value to an integer type when the value exceeds integer range
    - Converting a signed integer to an unsigned type when the value is negative
    - Converting between unsigned types when the source value exceeds the destination type's range
- **From string to numeric types**:
  - Comprehensive overflow and underflow detection with appropriate exceptions:
    - `std::overflow_error` if the string represents a value too large for the target type
    - `std::underflow_error` if trying to convert a negative string value to an unsigned type
    - `std::overflow_error` for extremely large floating-point strings (e.g., "1.0e+39" for float)

### String Conversions

- **To String**: All types can be converted to strings using string representations
- **From String**:
  - Numeric types: Parsed with overflow/underflow detection, will throw appropriate exceptions
  - Boolean: `"true"` or `"1"` converts to `true`, other values to `false`

### Special Floating Point Values

- **NaN (Not a Number)**:
  - Converts to string as `"nan"` or similar representation
  - Cannot be converted to boolean (will throw an exception)
  - Converting to integer types may have undefined behavior

- **Infinity**:
  - Converts to string as `"inf"` or similar representation
  - Converts to boolean as `true`
  - Converting to integer types may have undefined behavior

## Custom Type Conversions

The xnode library supports defining custom type conversions by creating specialized casters. See `xnode_long_double.h` for an example of extending the type conversion system to support `long double`.

## Usage Example

```cpp
xnode value;

// Store as int
value.set_as(42);

// Convert to other types
bool b = value.get_as<bool>();       // true
double d = value.get_as<double>();   // 42.0
std::string s = value.get_as<std::string>(); // "42"

// Store as string
value.set_as(std::string("123"));

// Convert string to numeric
int i = value.get_as<int>();         // 123
float f = value.get_as<float>();     // 123.0

// Check if conversion is possible
bool canConvert = value.is_convertable_to<int>();  // true

// Get with default value (no exceptions if conversion fails)
int safe = value.get_as_def<int>(0);  // 123 (or 0 if conversion fails)
```
