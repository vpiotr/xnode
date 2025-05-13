//----------------------------------------------------------------------------------
// Name:        xnode_overflow_test.cpp
// Purpose:     Tests for numeric overflow/underflow in type conversion for xnode
// Author:      GitHub Copilot
// Created:     02/05/2025
// License:     BSD
//----------------------------------------------------------------------------------

#include "xnode.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <string>
#include <stdexcept>

#include "cunit.h"
#include "xarray.h"

using namespace std;

// Helper function to log the test being executed
void LogOverflowTest(const std::string& fromType, const std::string& toType, const std::string& testType) {
    std::cout << "Testing " << testType << " conversion from " << std::left << std::setw(15) << fromType
              << " to " << std::setw(15) << toType << std::endl;
}

// Helper struct for string conversion tests
template<typename T>
struct StringToTypeConverter {
    std::string value_;
    xnode &node_;

    StringToTypeConverter(const std::string &value, xnode &node): value_(value), node_(node) {
        node_.set_as(value_);
    }

    void operator()() {
        T result = node_.get_as<T>();
        std::cout << "Converted " << value_ << " to " << result << std::endl; // This shouldn't execute if overflow happens
    }
};

// Helper struct for numeric conversion tests
template<typename FromType, typename ToType>
struct NumericConverter {
    FromType value_;
    xnode &node_;

    NumericConverter(const FromType &value, xnode &node): value_(value), node_(node) {
        node_.set_as(value_);
    }

    void operator()() {
        ToType result = node_.get_as<ToType>();
        std::cout << "Converted " << value_ << " to " << result << std::endl; // This shouldn't execute if overflow happens
    }
};

//----------------------------------------------------------------------
// String to numeric overflow tests
//----------------------------------------------------------------------

void TestStringToNumericOverflow() {
    xnode node;

    // Test string to int overflow
    LogOverflowTest("string", "int", "overflow");
    std::string intMaxOverflow = "2147483648"; // INT_MAX + 1 (assuming 32-bit int)
    AssertThrows(StringToTypeConverter<int>(intMaxOverflow, node), "String to int overflow");

    // Test string to int underflow
    LogOverflowTest("string", "int", "underflow");
    std::string intMinUnderflow = "-2147483649"; // INT_MIN - 1 (assuming 32-bit int)
    AssertThrows(StringToTypeConverter<int>(intMinUnderflow, node), "String to int underflow");

    // Test string to short overflow
    LogOverflowTest("string", "short", "overflow");
    std::string shortMaxOverflow = "32768"; // SHRT_MAX + 1
    AssertThrows(StringToTypeConverter<short>(shortMaxOverflow, node), "String to short overflow");

    // Test string to short underflow
    LogOverflowTest("string", "short", "underflow");
    std::string shortMinUnderflow = "-32769"; // SHRT_MIN - 1
    AssertThrows(StringToTypeConverter<short>(shortMinUnderflow, node), "String to short underflow");

    // Test string to char overflow
    LogOverflowTest("string", "char", "overflow");
    std::string charMaxOverflow = "128"; // CHAR_MAX + 1 (assuming signed char)
    AssertThrows(StringToTypeConverter<char>(charMaxOverflow, node), "String to char overflow");

    // Test string to char underflow
    LogOverflowTest("string", "char", "underflow");
    std::string charMinUnderflow = "-129"; // CHAR_MIN - 1 (assuming signed char)
    AssertThrows(StringToTypeConverter<char>(charMinUnderflow, node), "String to char underflow");

    // Test string to unsigned conversions (underflow)
    LogOverflowTest("string", "unsigned int", "underflow");
    std::string uintUnderflow = "-1"; // Minimum unsigned value is 0
    AssertThrows(StringToTypeConverter<unsigned int>(uintUnderflow, node), "String to unsigned int underflow");

    LogOverflowTest("string", "unsigned short", "underflow");
    std::string ushortUnderflow = "-1"; // Minimum unsigned value is 0
    AssertThrows(StringToTypeConverter<unsigned short>(ushortUnderflow, node), "String to unsigned short underflow");

    LogOverflowTest("string", "unsigned char", "underflow");
    std::string ucharUnderflow = "-1"; // Minimum unsigned value is 0
    AssertThrows(StringToTypeConverter<unsigned char>(ucharUnderflow, node), "String to unsigned char underflow");

    // Test string to floating point special values
    LogOverflowTest("string", "float", "special");
    std::string floatOverflow = "1.0e+39"; // Beyond float range
    AssertThrows(StringToTypeConverter<float>(floatOverflow, node), "String to float overflow");
}

//----------------------------------------------------------------------
// Integer type overflow tests
//----------------------------------------------------------------------

void TestIntegerTypeOverflow() {
    xnode node;

    // Test int to short overflow
    LogOverflowTest("int", "short", "overflow");
    int intToShortOverflow = std::numeric_limits<short>::max() + 1;
    AssertThrows((NumericConverter<int, short>(intToShortOverflow, node)), "Int to short overflow");

    // Test int to short underflow
    LogOverflowTest("int", "short", "underflow");
    int intToShortUnderflow = std::numeric_limits<short>::min() - 1;
    AssertThrows((NumericConverter<int, short>(intToShortUnderflow, node)), "Int to short underflow");

    // Test int to char overflow
    LogOverflowTest("int", "char", "overflow");
    int intToCharOverflow = std::numeric_limits<char>::max() + 1;
    AssertThrows((NumericConverter<int, char>(intToCharOverflow, node)), "Int to char overflow");

    // Test int to char underflow
    LogOverflowTest("int", "char", "underflow");
    int intToCharUnderflow = std::numeric_limits<char>::min() - 1;
    AssertThrows((NumericConverter<int, char>(intToCharUnderflow, node)), "Int to char underflow");

    // Test int to unsigned int underflow
    LogOverflowTest("int", "unsigned int", "underflow");
    int intToUintUnderflow = -1;
    AssertThrows((NumericConverter<int, unsigned int>(intToUintUnderflow, node)), "Int to unsigned int underflow");

    // Test long long to int overflow
    LogOverflowTest("long long", "int", "overflow");
    long long llToIntOverflow = static_cast<long long>(std::numeric_limits<int>::max()) + 1LL;
    AssertThrows((NumericConverter<long long, int>(llToIntOverflow, node)), "Long long to int overflow");

    // Test long long to int underflow
    LogOverflowTest("long long", "int", "underflow");
    long long llToIntUnderflow = static_cast<long long>(std::numeric_limits<int>::min()) - 1LL;
    AssertThrows((NumericConverter<long long, int>(llToIntUnderflow, node)), "Long long to int underflow");
}

//----------------------------------------------------------------------
// Floating point overflow tests
//----------------------------------------------------------------------

void TestFloatingPointOverflow() {
    xnode node;

    // Test double to float overflow
    LogOverflowTest("double", "float", "overflow");
    double doubleToFloatOverflow = static_cast<double>(std::numeric_limits<float>::max()) * 2.0;
    AssertThrows((NumericConverter<double, float>(doubleToFloatOverflow, node)), "Double to float overflow");

    // Test double to float underflow (smallest negative number beyond float range)
    LogOverflowTest("double", "float", "underflow");
    double doubleToFloatUnderflow = -static_cast<double>(std::numeric_limits<float>::max()) * 2.0;
    AssertThrows((NumericConverter<double, float>(doubleToFloatUnderflow, node)), "Double to float underflow");

    // Test double to int overflow
    LogOverflowTest("double", "int", "overflow");
    double doubleToIntOverflow = static_cast<double>(std::numeric_limits<int>::max()) + 1000.0;
    AssertThrows((NumericConverter<double, int>(doubleToIntOverflow, node)), "Double to int overflow");

    // Test double to int underflow
    LogOverflowTest("double", "int", "underflow");
    double doubleToIntUnderflow = static_cast<double>(std::numeric_limits<int>::min()) - 1000.0;
    AssertThrows((NumericConverter<double, int>(doubleToIntUnderflow, node)), "Double to int underflow");

    // Test float to short overflow
    LogOverflowTest("float", "short", "overflow");
    float floatToShortOverflow = static_cast<float>(std::numeric_limits<short>::max()) + 1000.0f;
    AssertThrows((NumericConverter<float, short>(floatToShortOverflow, node)), "Float to short overflow");

    // Test float to short underflow
    LogOverflowTest("float", "short", "underflow");
    float floatToShortUnderflow = static_cast<float>(std::numeric_limits<short>::min()) - 1000.0f;
    AssertThrows((NumericConverter<float, short>(floatToShortUnderflow, node)), "Float to short underflow");
}

//----------------------------------------------------------------------
// Unsigned types overflow tests
//----------------------------------------------------------------------

void TestUnsignedTypesOverflow() {
    xnode node;

    // Test unsigned int to short overflow
    LogOverflowTest("unsigned int", "short", "overflow");
    unsigned int uintToShortOverflow = static_cast<unsigned int>(std::numeric_limits<short>::max()) + 1U;
    AssertThrows((NumericConverter<unsigned int, short>(uintToShortOverflow, node)), "Unsigned int to short overflow");

    // Test unsigned int to int overflow (when unsigned int max > int max)
    LogOverflowTest("unsigned int", "int", "overflow");
    unsigned int uintToIntOverflow = static_cast<unsigned int>(std::numeric_limits<int>::max()) + 1U;
    AssertThrows((NumericConverter<unsigned int, int>(uintToIntOverflow, node)), "Unsigned int to int overflow");

    // Test unsigned long long to unsigned int overflow
    LogOverflowTest("unsigned long long", "unsigned int", "overflow");
    unsigned long long ullToUintOverflow = static_cast<unsigned long long>(std::numeric_limits<unsigned int>::max()) + 1ULL;
    AssertThrows((NumericConverter<unsigned long long, unsigned int>(ullToUintOverflow, node)), "Unsigned long long to unsigned int overflow");

    // Test unsigned long to int overflow
    LogOverflowTest("unsigned long", "int", "overflow");
    unsigned long ulToIntOverflow = static_cast<unsigned long>(std::numeric_limits<int>::max()) + 1UL;
    AssertThrows((NumericConverter<unsigned long, int>(ulToIntOverflow, node)), "Unsigned long to int overflow");
}

//----------------------------------------------------------------------
// Special case tests for numeric extremes
//----------------------------------------------------------------------

void TestNumericExtremes() {
    xnode node;

    // Test max int to float (should be fine, but may lose precision)
    LogOverflowTest("int", "float", "max value");
    int maxInt = std::numeric_limits<int>::max();
    node.set_as(maxInt);
    float maxIntAsFloat = node.get_as<float>();
    float maxIntAsFloatExpected = static_cast<float>(maxInt);
    // This is not an overflow, but we should check precision
    cout << "Max int: " << maxInt << ", Max int as float: " << maxIntAsFloat << std::endl;
    Assert(abs(maxIntAsFloatExpected - (maxIntAsFloat)) < 1e5f, "Max int to float precision check");

    // Test min int to float (should be fine, but may lose precision)
    LogOverflowTest("int", "float", "min value");
    int minInt = std::numeric_limits<int>::min();
    node.set_as(minInt);
    float minIntAsFloat = node.get_as<float>();
    // This is not an overflow, but we should check precision
    AssertEquals(static_cast<int>(minIntAsFloat), minInt, "Min int to float precision check");

    // Test extreme unsigned values
    LogOverflowTest("unsigned int", "int", "max value");
    unsigned int maxUint = std::numeric_limits<unsigned int>::max();
    AssertThrows((NumericConverter<unsigned int, int>(maxUint, node)), "Max unsigned int to int overflow");

    // Test string representation of extreme values
    LogOverflowTest("string", "long", "max value");
    std::string maxLongStr = std::to_string(std::numeric_limits<long>::max());
    node.set_as(maxLongStr);
    AssertEquals(node.get_as<long>(), std::numeric_limits<long>::max(), "String max long to long");

    // Create a string just beyond max long
    std::string beyondMaxLongStr = maxLongStr + "0"; // Adding a digit makes it exceed max long
    AssertThrows(StringToTypeConverter<long>(beyondMaxLongStr, node), "String beyond max long to long");
}

//----------------------------------------------------------------------
// Main test function
//----------------------------------------------------------------------

int xnode_overflow_test() {
    TEST_PROLOG();
    
    TEST_FUNC(StringToNumericOverflow);
    TEST_FUNC(IntegerTypeOverflow);
    TEST_FUNC(FloatingPointOverflow);
    TEST_FUNC(UnsignedTypesOverflow);
    TEST_FUNC(NumericExtremes);
    
    TEST_EPILOG();
}

int main()
{
    cout << "-------------------- XNODE OVERFLOW TESTS --------------------" << endl;
    return xnode_overflow_test();
}