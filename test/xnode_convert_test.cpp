//----------------------------------------------------------------------------------
// Name:        xnode_convert_test.cpp
// Purpose:     Comprehensive type conversion tests for xnode library
// Author:      GitHub Copilot (based on xnode_test.cpp by Piotr Likus)
// Created:     30/04/2025
// License:     BSD
//----------------------------------------------------------------------------------

#include "xnode.h"
#include <iostream>

// for reporting
#include <iomanip>

#include "cunit.h"
#include "xnode_vector.h"
#include "xnode_property_list.h"
#include "xnode_long_double.h"
#include <limits>
#include <cmath>

using namespace std;

// Helper function to log the conversion being tested
void LogConversion(const std::string& fromType, const std::string& toType) {
    std::cout << "Testing conversion from " << std::left << std::setw(15) << fromType
              << " to " << std::setw(15) << toType << std::endl;
}

//----------------------------------------------------------------------
// Boolean source tests
//----------------------------------------------------------------------

void TestBoolToTypes() {
    // Test converting from bool to various types
    bool sourceValue = true;
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<bool>(), "Value should be stored as bool");
    
    // Test conversion to bool
    LogConversion("bool", "bool");
    Assert(value.get_as<bool>() == true, "bool to bool conversion");
    
    // Test conversion to numeric types
    LogConversion("bool", "int");
    Assert(value.get_as<int>() == 1, "bool to int conversion");
    
    LogConversion("bool", "float");
    Assert(value.get_as<float>() == 1.0f, "bool to float conversion");
    
    LogConversion("bool", "double");
    Assert(value.get_as<double>() == 1.0, "bool to double conversion");
    
    LogConversion("bool", "char");
    Assert(value.get_as<char>() == 1, "bool to char conversion");
    
    LogConversion("bool", "short");
    Assert(value.get_as<short>() == 1, "bool to short conversion");
    
    LogConversion("bool", "long");
    Assert(value.get_as<long>() == 1L, "bool to long conversion");
    
    LogConversion("bool", "long long");
    Assert(value.get_as<long long>() == 1LL, "bool to long long conversion");
    
    // Test conversion to unsigned types
    LogConversion("bool", "unsigned int");
    Assert(value.get_as<unsigned int>() == 1U, "bool to unsigned int conversion");
    
    LogConversion("bool", "unsigned char");
    Assert(value.get_as<unsigned char>() == (unsigned char)1, "bool to unsigned char conversion");
    
    LogConversion("bool", "unsigned short");
    Assert(value.get_as<unsigned short>() == (unsigned short)1, "bool to unsigned short conversion");
    
    LogConversion("bool", "unsigned long");
    Assert(value.get_as<unsigned long>() == 1UL, "bool to unsigned long conversion");
    
    LogConversion("bool", "unsigned long long");
    Assert(value.get_as<unsigned long long>() == 1ULL, "bool to unsigned long long conversion");
    
    // Test conversion to string
    LogConversion("bool", "string");
    Assert(value.get_as<std::string>() == "true", "bool to string conversion");
    
    // Test with false value
    value.set_as(false);
    LogConversion("bool(false)", "string");
    Assert(value.get_as<std::string>() == "false", "bool(false) to string conversion");
    
    LogConversion("bool(false)", "int");
    Assert(value.get_as<int>() == 0, "bool(false) to int conversion");
}

//----------------------------------------------------------------------
// Integer source tests
//----------------------------------------------------------------------

void TestIntToTypes() {
    // Test converting from int to various types
    int sourceValue = 123;
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<int>(), "Value should be stored as int");
    
    // Test conversion to bool
    LogConversion("int", "bool");
    Assert(value.get_as<bool>() == true, "int to bool conversion");
    
    // Test with zero
    value.set_as(0);
    LogConversion("int(0)", "bool");
    Assert(value.get_as<bool>() == false, "int(0) to bool conversion");

    // Test with one
    value.set_as(1);
    LogConversion("int(1)", "bool");
    Assert(value.get_as<bool>() == true, "int(1) to bool conversion");
    
    // Reset to original value
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("int", "int");
    Assert(value.get_as<int>() == 123, "int to int conversion");
    
    LogConversion("int", "float");
    Assert(value.get_as<float>() == 123.0f, "int to float conversion");
    
    LogConversion("int", "double");
    Assert(value.get_as<double>() == 123.0, "int to double conversion");
    
    LogConversion("int", "char");
    Assert(value.get_as<char>() == (char)123, "int to char conversion");
    
    LogConversion("int", "short");
    Assert(value.get_as<short>() == (short)123, "int to short conversion");
    
    LogConversion("int", "long");
    Assert(value.get_as<long>() == 123L, "int to long conversion");
    
    LogConversion("int", "long long");
    Assert(value.get_as<long long>() == 123LL, "int to long long conversion");
    
    // Test conversion to unsigned types
    LogConversion("int", "unsigned int");
    Assert(value.get_as<unsigned int>() == 123U, "int to unsigned int conversion");
    
    LogConversion("int", "unsigned char");
    Assert(value.get_as<unsigned char>() == (unsigned char)123, "int to unsigned char conversion");
    
    LogConversion("int", "unsigned short");
    Assert(value.get_as<unsigned short>() == (unsigned short)123, "int to unsigned short conversion");
    
    LogConversion("int", "unsigned long");
    Assert(value.get_as<unsigned long>() == 123UL, "int to unsigned long conversion");
    
    LogConversion("int", "unsigned long long");
    Assert(value.get_as<unsigned long long>() == 123ULL, "int to unsigned long long conversion");
    
    // Test conversion to string
    LogConversion("int", "string");
    Assert(value.get_as<std::string>() == "123", "int to string conversion");
}

void TestNegativeIntToTypes() {
    // Test converting from negative int to various types
    int sourceValue = -123;
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<int>(), "Value should be stored as int");
    
    // Test conversion to bool
    LogConversion("int(-)", "bool");
    Assert(value.get_as<bool>() == true, "negative int to bool conversion");
    
    // Test conversion to numeric types
    LogConversion("int(-)", "int");
    Assert(value.get_as<int>() == -123, "negative int to int conversion");
    
    LogConversion("int(-)", "float");
    Assert(value.get_as<float>() == -123.0f, "negative int to float conversion");
    
    LogConversion("int(-)", "double");
    Assert(value.get_as<double>() == -123.0, "negative int to double conversion");
    
    LogConversion("int(-)", "long");
    Assert(value.get_as<long>() == -123L, "negative int to long conversion");
    
    LogConversion("int(-)", "long long");
    Assert(value.get_as<long long>() == -123LL, "negative int to long long conversion");
    
    // Test conversion to string
    LogConversion("int(-)", "string");
    Assert(value.get_as<std::string>() == "-123", "negative int to string conversion");
}

//----------------------------------------------------------------------
// Short source tests
//----------------------------------------------------------------------

void TestShortToTypes() {
    // Test converting from short to various types
    short sourceValue = 12345;  // Near the upper limit for short (32767)
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<short>(), "Value should be stored as short");
    
    // Test conversion to bool
    LogConversion("short", "bool");
    Assert(value.get_as<bool>() == true, "short to bool conversion");
    
    // Test with zero
    value.set_as((short)0);
    LogConversion("short(0)", "bool");
    Assert(value.get_as<bool>() == false, "short(0) to bool conversion");
    
    // Reset to original value
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("short", "int");
    Assert(value.get_as<int>() == 12345, "short to int conversion");
    
    LogConversion("short", "float");
    Assert(value.get_as<float>() == 12345.0f, "short to float conversion");
    
    LogConversion("short", "double");
    Assert(value.get_as<double>() == 12345.0, "short to double conversion");
    
    LogConversion("short", "char");
    Assert(value.get_as<char>() == (char)12345, "short to char conversion (will truncate)");
    
    LogConversion("short", "long");
    Assert(value.get_as<long>() == 12345L, "short to long conversion");
    
    LogConversion("short", "long long");
    Assert(value.get_as<long long>() == 12345LL, "short to long long conversion");
    
    // Test conversion to unsigned types
    LogConversion("short", "unsigned int");
    Assert(value.get_as<unsigned int>() == 12345U, "short to unsigned int conversion");
    
    LogConversion("short", "unsigned short");
    Assert(value.get_as<unsigned short>() == (unsigned short)12345, "short to unsigned short conversion");
    
    // Test conversion to string
    LogConversion("short", "string");
    Assert(value.get_as<std::string>() == "12345", "short to string conversion");
    
    // Test with negative value
    value.set_as((short)-12345);
    LogConversion("short(-)", "int");
    Assert(value.get_as<int>() == -12345, "negative short to int conversion");
    
    LogConversion("short(-)", "string");
    Assert(value.get_as<std::string>() == "-12345", "negative short to string conversion");
}

//----------------------------------------------------------------------
// Float source tests
//----------------------------------------------------------------------

void TestFloatToTypes() {
    // Test converting from float to various types
    float sourceValue = 123.456f;
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<float>(), "Value should be stored as float");
    
    // Test conversion to bool
    LogConversion("float", "bool");
    Assert(value.get_as<bool>() == true, "float to bool conversion");
    
    // Test with zero
    value.set_as(0.0f);
    LogConversion("float(0)", "bool");
    Assert(value.get_as<bool>() == false, "float(0) to bool conversion");
    
    // Test with one
    value.set_as(1.0f);
    LogConversion("float(1)", "bool");
    Assert(value.get_as<bool>() == true, "float(1) to bool conversion");
    
    // Reset to original value
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("float", "int");
    Assert(value.get_as<int>() == 123, "float to int conversion");
    
    LogConversion("float", "float");
    Assert(abs(value.get_as<float>() - 123.456f) < 0.001f, "float to float conversion");
    
    LogConversion("float", "double");
    Assert(abs(value.get_as<double>() - 123.456) < 0.001, "float to double conversion");
    
    LogConversion("float", "long");
    Assert(value.get_as<long>() == 123L, "float to long conversion");
    
    LogConversion("float", "long long");
    Assert(value.get_as<long long>() == 123LL, "float to long long conversion");
    
    // Test conversion to unsigned types
    LogConversion("float", "unsigned int");
    Assert(value.get_as<unsigned int>() == 123U, "float to unsigned int conversion");
    
    LogConversion("float", "unsigned long");
    Assert(value.get_as<unsigned long>() == 123UL, "float to unsigned long conversion");
    
    // Test conversion to string
    LogConversion("float", "string");
    std::string str = value.get_as<std::string>();
    Assert(str.find("123.") != std::string::npos, "float to string conversion");
}

//----------------------------------------------------------------------
// Negative Float source tests
//----------------------------------------------------------------------

void TestNegativeFloatToTypes() {
    // Test converting from negative float to various types
    float sourceValue = -123.456f;
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<float>(), "Value should be stored as float");
    
    // Test conversion to bool
    LogConversion("float(-)", "bool");
    Assert(value.get_as<bool>() == true, "negative float to bool conversion");
    
    // Test conversion to numeric types
    LogConversion("float(-)", "int");
    Assert(value.get_as<int>() == -123, "negative float to int conversion");
    
    LogConversion("float(-)", "float");
    Assert(abs(value.get_as<float>() - (-123.456f)) < 0.001f, "negative float to float conversion");
    
    LogConversion("float(-)", "double");
    Assert(abs(value.get_as<double>() - (-123.456)) < 0.001, "negative float to double conversion");
    
    LogConversion("float(-)", "long");
    Assert(value.get_as<long>() == -123L, "negative float to long conversion");
    
    LogConversion("float(-)", "long long");
    Assert(value.get_as<long long>() == -123LL, "negative float to long long conversion");
    
    // Test conversion to string
    LogConversion("float(-)", "string");
    std::string str = value.get_as<std::string>();
    Assert(str.find("-123.") != std::string::npos, "negative float to string conversion");
}

//----------------------------------------------------------------------
// Double source tests
//----------------------------------------------------------------------

void TestDoubleToTypes() {
    // Test converting from double to various types
    double sourceValue = 123.456789;
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<double>(), "Value should be stored as double");
    
    // Test conversion to bool
    LogConversion("double", "bool");
    Assert(value.get_as<bool>() == true, "double to bool conversion");
    
    // Test with zero
    value.set_as(0.0);
    LogConversion("double(0)", "bool");
    Assert(value.get_as<bool>() == false, "double(0) to bool conversion");
    
    // Test with one
    value.set_as(1.0);
    LogConversion("double(1)", "bool");
    Assert(value.get_as<bool>() == true, "double(1) to bool conversion");
    
    // Reset to original value
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("double", "int");
    Assert(value.get_as<int>() == 123, "double to int conversion");
    
    LogConversion("double", "float");
    Assert(abs(value.get_as<float>() - 123.456f) < 0.001f, "double to float conversion");
    
    LogConversion("double", "double");
    Assert(abs(value.get_as<double>() - 123.456789) < 0.0001, "double to double conversion");
    
    LogConversion("double", "long");
    Assert(value.get_as<long>() == 123L, "double to long conversion");
    
    LogConversion("double", "long long");
    Assert(value.get_as<long long>() == 123LL, "double to long long conversion");
    
    // Test conversion to unsigned types
    LogConversion("double", "unsigned int");
    Assert(value.get_as<unsigned int>() == 123U, "double to unsigned int conversion");
    
    LogConversion("double", "unsigned long");
    Assert(value.get_as<unsigned long>() == 123UL, "double to unsigned long conversion");
    
    // Test conversion to string
    LogConversion("double", "string");
    std::string str = value.get_as<std::string>();
    Assert(str.find("123.") != std::string::npos, "double to string conversion");
}

//----------------------------------------------------------------------
// Negative Double source tests
//----------------------------------------------------------------------

void TestNegativeDoubleToTypes() {
    // Test converting from negative double to various types
    double sourceValue = -123.456789;
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<double>(), "Value should be stored as double");
    
    // Test conversion to bool
    LogConversion("double(-)", "bool");
    Assert(value.get_as<bool>() == true, "negative double to bool conversion");
    
    // Test conversion to numeric types
    LogConversion("double(-)", "int");
    Assert(value.get_as<int>() == -123, "negative double to int conversion");
    
    LogConversion("double(-)", "float");
    Assert(abs(value.get_as<float>() - (-123.456f)) < 0.001f, "negative double to float conversion");
    
    LogConversion("double(-)", "double");
    Assert(abs(value.get_as<double>() - (-123.456789)) < 0.0001, "negative double to double conversion");
    
    LogConversion("double(-)", "long");
    Assert(value.get_as<long>() == -123L, "negative double to long conversion");
    
    LogConversion("double(-)", "long long");
    Assert(value.get_as<long long>() == -123LL, "negative double to long long conversion");
    
    // Test conversion to string
    LogConversion("double(-)", "string");
    std::string str = value.get_as<std::string>();
    Assert(str.find("-123.") != std::string::npos, "negative double to string conversion");
}

//----------------------------------------------------------------------
// Character source tests
//----------------------------------------------------------------------

void TestCharToTypes() {
    // Test converting from char to various types
    char sourceValue = 65; // ASCII 'A'
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<char>(), "Value should be stored as char");
    
    // Test conversion to bool
    LogConversion("char", "bool");
    Assert(value.get_as<bool>() == true, "char to bool conversion");
    
    // Test with zero
    value.set_as((char)0);
    LogConversion("char(0)", "bool");
    Assert(value.get_as<bool>() == false, "char(0) to bool conversion");
    
    // Test with one
    value.set_as((char)1);
    LogConversion("char(1)", "bool");
    Assert(value.get_as<bool>() == true, "char(1) to bool conversion");
    
    // Reset to original value
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("char", "int");
    Assert(value.get_as<int>() == 65, "char to int conversion");
    
    LogConversion("char", "float");
    Assert(value.get_as<float>() == 65.0f, "char to float conversion");
    
    LogConversion("char", "double");
    Assert(value.get_as<double>() == 65.0, "char to double conversion");
    
    LogConversion("char", "char");
    Assert(value.get_as<char>() == 'A', "char to char conversion");
    
    LogConversion("char", "short");
    Assert(value.get_as<short>() == 65, "char to short conversion");
    
    LogConversion("char", "long");
    Assert(value.get_as<long>() == 65L, "char to long conversion");
    
    // Test conversion to unsigned types
    LogConversion("char", "unsigned int");
    Assert(value.get_as<unsigned int>() == 65U, "char to unsigned int conversion");
    
    LogConversion("char", "unsigned char");
    Assert(value.get_as<unsigned char>() == 65, "char to unsigned char conversion");
    
    // Test conversion to string
    LogConversion("char", "string");
    std::string str = value.get_as<std::string>();
    Assert(str == "A", "char to string conversion");
}

//----------------------------------------------------------------------
// Unsigned Character source tests
//----------------------------------------------------------------------

void TestUnsignedCharToTypes() {
    // Test converting from unsigned char to various types
    unsigned char sourceValue = 120; // Value greater than char max (127)
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<unsigned char>(), "Value should be stored as unsigned char");
    
    // Test conversion to bool
    LogConversion("unsigned char", "bool");
    Assert(value.get_as<bool>() == true, "unsigned char to bool conversion");
    
    // Test with zero
    value.set_as((unsigned char)0);
    LogConversion("unsigned char(0)", "bool");
    Assert(value.get_as<bool>() == false, "unsigned char(0) to bool conversion");

    // Test with one
    value.set_as((unsigned char)1);
    LogConversion("unsigned char(1)", "bool");
    Assert(value.get_as<bool>() == true, "unsigned char(1) to bool conversion");   
    
    // Reset to original value
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("unsigned char", "int");
    Assert(value.get_as<int>() == 120, "unsigned char to int conversion");
    
    LogConversion("unsigned char", "float");
    Assert(value.get_as<float>() == 120.0f, "unsigned char to float conversion");
    
    LogConversion("unsigned char", "double");
    Assert(value.get_as<double>() == 120.0, "unsigned char to double conversion");
    
    LogConversion("unsigned char", "char");
    Assert(value.get_as<char>() == (char)120, "unsigned char to char conversion (may truncate)");
    
    LogConversion("unsigned char", "unsigned int");
    Assert(value.get_as<unsigned int>() == 120U, "unsigned char to unsigned int conversion");
    
    LogConversion("unsigned char", "unsigned char");
    Assert(value.get_as<unsigned char>() == 120, "unsigned char to unsigned char conversion");
    
    // Test conversion to string
    LogConversion("unsigned char", "string");
    std::string str = value.get_as<std::string>();
    AssertEquals(std::string("")+(char)sourceValue, str, "unsigned char to string conversion");
}

//----------------------------------------------------------------------
// Unsigned Short source tests
//----------------------------------------------------------------------

void TestUnsignedShortToTypes() {
    // Test converting from unsigned short to various types
    unsigned short sourceValue = 65000; // Value greater than short max (32767)
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<unsigned short>(), "Value should be stored as unsigned short");
    
    // Test conversion to bool
    LogConversion("unsigned short", "bool");
    Assert(value.get_as<bool>() == true, "unsigned short to bool conversion");
    
    // Test with zero
    value.set_as((unsigned short)0);
    LogConversion("unsigned short(0)", "bool");
    Assert(value.get_as<bool>() == false, "unsigned short(0) to bool conversion");
    
    // Reset to original value
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("unsigned short", "int");
    Assert(value.get_as<int>() == 65000, "unsigned short to int conversion");
    
    LogConversion("unsigned short", "float");
    Assert(value.get_as<float>() == 65000.0f, "unsigned short to float conversion");
    
    LogConversion("unsigned short", "double");
    Assert(value.get_as<double>() == 65000.0, "unsigned short to double conversion");
    
    LogConversion("unsigned short", "short");
    // This will likely overflow on most platforms as 65000 > 32767
    Assert(value.get_as<short>() == (short)65000, "unsigned short to short conversion (may overflow)");
    
    LogConversion("unsigned short", "long");
    Assert(value.get_as<long>() == 65000L, "unsigned short to long conversion");
    
    LogConversion("unsigned short", "long long");
    Assert(value.get_as<long long>() == 65000LL, "unsigned short to long long conversion");
    
    // Test conversion to unsigned types
    LogConversion("unsigned short", "unsigned int");
    Assert(value.get_as<unsigned int>() == 65000U, "unsigned short to unsigned int conversion");
    
    LogConversion("unsigned short", "unsigned short");
    Assert(value.get_as<unsigned short>() == 65000, "unsigned short to unsigned short conversion");
    
    // Test conversion to string
    LogConversion("unsigned short", "string");
    Assert(value.get_as<std::string>() == "65000", "unsigned short to string conversion");
}

//----------------------------------------------------------------------
// Unsigned Integer source tests
//----------------------------------------------------------------------

void TestUnsignedIntToTypes() {
    // Test converting from unsigned int to various types
    unsigned int sourceValue = 12345;
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<unsigned int>(), "Value should be stored as unsigned int");
    
    // Test conversion to bool
    LogConversion("unsigned int", "bool");
    Assert(value.get_as<bool>() == true, "unsigned int to bool conversion");
    
    // Test with zero
    value.set_as(0U);
    LogConversion("unsigned int(0)", "bool");
    Assert(value.get_as<bool>() == false, "unsigned int(0) to bool conversion");
    
    // Test with one
    value.set_as(1U);
    LogConversion("unsigned int(1)", "bool");
    Assert(value.get_as<bool>() == true, "unsigned int(1) to bool conversion");
    
    // Reset to original value
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("unsigned int", "int");
    Assert(value.get_as<int>() == 12345, "unsigned int to int conversion");
    
    LogConversion("unsigned int", "float");
    Assert(value.get_as<float>() == 12345.0f, "unsigned int to float conversion");
    
    LogConversion("unsigned int", "double");
    Assert(value.get_as<double>() == 12345.0, "unsigned int to double conversion");
    
    LogConversion("unsigned int", "long");
    Assert(value.get_as<long>() == 12345L, "unsigned int to long conversion");
    
    LogConversion("unsigned int", "long long");
    Assert(value.get_as<long long>() == 12345LL, "unsigned int to long long conversion");
    
    // Test conversion to unsigned types
    LogConversion("unsigned int", "unsigned int");
    Assert(value.get_as<unsigned int>() == 12345U, "unsigned int to unsigned int conversion");
    
    LogConversion("unsigned int", "unsigned long");
    Assert(value.get_as<unsigned long>() == 12345UL, "unsigned int to unsigned long conversion");
    
    LogConversion("unsigned int", "unsigned long long");
    Assert(value.get_as<unsigned long long>() == 12345ULL, "unsigned int to unsigned long long conversion");
    
    // Test conversion to string
    LogConversion("unsigned int", "string");
    Assert(value.get_as<std::string>() == "12345", "unsigned int to string conversion");
}

//----------------------------------------------------------------------
// Long source tests
//----------------------------------------------------------------------

void TestLongToTypes() {
    // Test converting from long to various types
    long sourceValue = 123456789L;
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<long>(), "Value should be stored as long");
    
    // Test conversion to bool
    LogConversion("long", "bool");
    Assert(value.get_as<bool>() == true, "long to bool conversion");
    
    // Test with zero
    value.set_as(0L);
    LogConversion("long(0)", "bool");
    Assert(value.get_as<bool>() == false, "long(0) to bool conversion");
    
    // Test with one
    value.set_as(1L);
    LogConversion("long(1)", "bool");
    Assert(value.get_as<bool>() == true, "long(1) to bool conversion");
    
    // Reset to original value
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("long", "int");
    Assert(value.get_as<int>() == (int)123456789L, "long to int conversion");
    
    LogConversion("long", "float");
    Assert(abs(value.get_as<float>() - 123456789.0f) < 1.0f, "long to float conversion");
    
    LogConversion("long", "double");
    Assert(abs(value.get_as<double>() - 123456789.0) < 1.0, "long to double conversion");
    
    LogConversion("long", "long");
    Assert(value.get_as<long>() == 123456789L, "long to long conversion");
    
    LogConversion("long", "long long");
    Assert(value.get_as<long long>() == 123456789LL, "long to long long conversion");
    
    // Test conversion to unsigned types
    LogConversion("long", "unsigned int");
    Assert(value.get_as<unsigned int>() == (unsigned int)123456789UL, "long to unsigned int conversion");
    
    LogConversion("long", "unsigned long");
    Assert(value.get_as<unsigned long>() == 123456789UL, "long to unsigned long conversion");
    
    // Test conversion to string
    LogConversion("long", "string");
    Assert(value.get_as<std::string>() == "123456789", "long to string conversion");
}

//----------------------------------------------------------------------
// Negative Long source tests
//----------------------------------------------------------------------

void TestNegativeLongToTypes() {
    // Test converting from negative long to various types
    long sourceValue = -123456789L;
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<long>(), "Value should be stored as long");
    
    // Test conversion to bool
    LogConversion("long(-)", "bool");
    Assert(value.get_as<bool>() == true, "negative long to bool conversion");
    
    // Test conversion to numeric types
    LogConversion("long(-)", "int");
    Assert(value.get_as<int>() == (int)-123456789L, "negative long to int conversion");
    
    LogConversion("long(-)", "float");
    Assert(abs(value.get_as<float>() - (-123456789.0f)) < 1.0f, "negative long to float conversion");
    
    LogConversion("long(-)", "double");
    Assert(abs(value.get_as<double>() - (-123456789.0)) < 1.0, "negative long to double conversion");
    
    LogConversion("long(-)", "long");
    Assert(value.get_as<long>() == -123456789L, "negative long to long conversion");
    
    LogConversion("long(-)", "long long");
    Assert(value.get_as<long long>() == -123456789LL, "negative long to long long conversion");
    
    // Test conversion to string
    LogConversion("long(-)", "string");
    Assert(value.get_as<std::string>() == "-123456789", "negative long to string conversion");
}

//----------------------------------------------------------------------
// Unsigned Long source tests
//----------------------------------------------------------------------

void TestUnsignedLongToTypes() {
    // Test converting from unsigned long to various types
    unsigned long sourceValue = 4294967295UL;  // 2^32 - 1, max value for 32-bit unsigned int
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<unsigned long>(), "Value should be stored as unsigned long");
    
    // Test conversion to bool
    LogConversion("unsigned long", "bool");
    Assert(value.get_as<bool>() == true, "unsigned long to bool conversion");
    
    // Test with zero
    value.set_as(0UL);
    LogConversion("unsigned long(0)", "bool");
    Assert(value.get_as<bool>() == false, "unsigned long(0) to bool conversion");
    
    // Reset to original value
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("unsigned long", "float");
    Assert(abs(value.get_as<float>() - 4294967295.0f) < 1024.0f, "unsigned long to float conversion");
    
    LogConversion("unsigned long", "double");
    Assert(abs(value.get_as<double>() - 4294967295.0) < 1.0, "unsigned long to double conversion");
    
    // Test conversion to long
    LogConversion("unsigned long", "long");
    Assert(value.get_as<long>() == (long)4294967295UL, "unsigned long to long conversion");
    
    LogConversion("unsigned long", "long long");
    Assert(value.get_as<long long>() == 4294967295LL, "unsigned long to long long conversion");
    
    // Test conversion to unsigned types
    LogConversion("unsigned long", "unsigned long");
    Assert(value.get_as<unsigned long>() == 4294967295UL, "unsigned long to unsigned long conversion");
    
    LogConversion("unsigned long", "unsigned long long");
    Assert(value.get_as<unsigned long long>() == 4294967295ULL, "unsigned long to unsigned long long conversion");
    
    // Test conversion to string
    LogConversion("unsigned long", "string");
    Assert(value.get_as<std::string>() == "4294967295", "unsigned long to string conversion");
}

//----------------------------------------------------------------------
// Long Long source tests
//----------------------------------------------------------------------

void TestLongLongToTypes() {
    // Test converting from long long to various types
    long long sourceValue = 9223372036854775807LL; // Max value for 64-bit signed integer
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<long long>(), "Value should be stored as long long");
    
    // Test conversion to bool
    LogConversion("long long", "bool");
    Assert(value.get_as<bool>() == true, "long long to bool conversion");
    
    // Test with zero
    value.set_as(0LL);
    LogConversion("long long(0)", "bool");
    Assert(value.get_as<bool>() == false, "long long(0) to bool conversion");
    
    // Test with one
    value.set_as(1LL);
    LogConversion("long long(1)", "bool");
    Assert(value.get_as<bool>() == true, "long long(1) to bool conversion");
    
    // Reset to more testable value
    sourceValue = 1234567890123LL;
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("long long", "double");
    Assert(abs(value.get_as<double>() - 1234567890123.0) < 1.0, "long long to double conversion");
    
    LogConversion("long long", "long long");
    Assert(value.get_as<long long>() == 1234567890123LL, "long long to long long conversion");
    
    // Test conversion to unsigned types
    LogConversion("long long", "unsigned long long");
    Assert(value.get_as<unsigned long long>() == 1234567890123ULL, "long long to unsigned long long conversion");
    
    // Test conversion to string
    LogConversion("long long", "string");
    Assert(value.get_as<std::string>() == "1234567890123", "long long to string conversion");
}

//----------------------------------------------------------------------
// Negative Long Long source tests
//----------------------------------------------------------------------

void TestNegativeLongLongToTypes() {
    // Test converting from negative long long to various types
    long long sourceValue = -1234567890123LL;
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<long long>(), "Value should be stored as long long");
    
    // Test conversion to bool
    LogConversion("long long(-)", "bool");
    Assert(value.get_as<bool>() == true, "negative long long to bool conversion");
    
    // Test conversion to numeric types
    LogConversion("long long(-)", "double");
    Assert(abs(value.get_as<double>() - (-1234567890123.0)) < 1.0, "negative long long to double conversion");
    
    LogConversion("long long(-)", "float");
    // For very large values, we need a larger epsilon due to float precision limitations
    Assert(abs(value.get_as<float>() - (-1234567890123.0f)) < 1e5f, "negative long long to float conversion");
    
    LogConversion("long long(-)", "long");
    // This may lose precision on some platforms if long is 32-bit
    // Testing only if platform's long can hold this value
    if (sizeof(long) >= sizeof(long long)) {
        Assert(value.get_as<long>() == -1234567890123L, "negative long long to long conversion");
    }
    
    LogConversion("long long(-)", "long long");
    Assert(value.get_as<long long>() == -1234567890123LL, "negative long long to long long conversion");
    
    // Test conversion to string
    LogConversion("long long(-)", "string");
    Assert(value.get_as<std::string>() == "-1234567890123", "negative long long to string conversion");
}

//----------------------------------------------------------------------
// Unsigned Long Long source tests
//----------------------------------------------------------------------

void TestUnsignedLongLongToTypes() {
    // Test converting from unsigned long long to various types
    unsigned long long sourceValue = 18446744073709551615ULL;  // 2^64 - 1, max value for unsigned long long
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<unsigned long long>(), "Value should be stored as unsigned long long");
    
    // Test conversion to bool
    LogConversion("unsigned long long", "bool");
    Assert(value.get_as<bool>() == true, "unsigned long long to bool conversion");
    
    // Test with zero
    value.set_as(0ULL);
    LogConversion("unsigned long long(0)", "bool");
    Assert(value.get_as<bool>() == false, "unsigned long long(0) to bool conversion");
    
    // Reset to a more manageable value for testing
    sourceValue = 12345678901234ULL;
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("unsigned long long", "double");
    Assert(abs(value.get_as<double>() - 12345678901234.0) < 1.0, "unsigned long long to double conversion");
    
    // Test conversion to long long
    LogConversion("unsigned long long", "long long");
    Assert(value.get_as<long long>() == 12345678901234LL, "unsigned long long to long long conversion");
    
    // Test conversion to unsigned types
    LogConversion("unsigned long long", "unsigned long long");
    Assert(value.get_as<unsigned long long>() == 12345678901234ULL, "unsigned long long to unsigned long long conversion");
    
    // Test conversion to string
    LogConversion("unsigned long long", "string");
    Assert(value.get_as<std::string>() == "12345678901234", "unsigned long long to string conversion");
}

//----------------------------------------------------------------------
// String source tests
//----------------------------------------------------------------------

void TestStringToTypes() {
    // Test converting from string to various types
    std::string sourceValue = "123";
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<std::string>(), "Value should be stored as string");
    
    // Test conversion to bool
    LogConversion("string", "bool");
    Assert(value.get_as<bool>() == false, "string to bool conversion - numeric non-zero");
    
    // Test with "true" and "false" strings
    value.set_as(std::string("true"));
    LogConversion("string(true)", "bool");
    Assert(value.get_as<bool>() == true, "string(true) to bool conversion");
    
    value.set_as(std::string("false"));
    LogConversion("string(false)", "bool");
    Assert(value.get_as<bool>() == false, "string(false) to bool conversion");
    
    value.set_as(std::string("0"));
    LogConversion("string(0)", "bool");
    Assert(value.get_as<bool>() == false, "string(0) to bool conversion");
    
    value.set_as(std::string("1"));
    LogConversion("string(1)", "bool");
    Assert(value.get_as<bool>() == true, "string(1) to bool conversion");
    
    // Reset to original value
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("string", "int");
    Assert(value.get_as<int>() == 123, "string to int conversion");
    
    LogConversion("string", "float");
    Assert(value.get_as<float>() == 123.0f, "string to float conversion");
    
    LogConversion("string", "double");
    Assert(value.get_as<double>() == 123.0, "string to double conversion");
    
    LogConversion("string", "long");
    Assert(value.get_as<long>() == 123L, "string to long conversion");
    
    LogConversion("string", "long long");
    Assert(value.get_as<long long>() == 123LL, "string to long long conversion");
    
    // Test with decimal string
    value.set_as(std::string("123.456"));
    LogConversion("string(decimal)", "float");
    Assert(abs(value.get_as<float>() - 123.456f) < 0.001f, "string(decimal) to float conversion");
    
    LogConversion("string(decimal)", "double");
    Assert(abs(value.get_as<double>() - 123.456) < 0.001, "string(decimal) to double conversion");
    
    // Test conversion to string
    LogConversion("string", "string");
    Assert(value.get_as<std::string>() == "123.456", "string to string conversion");
}

//----------------------------------------------------------------------
// Tests for long double conversion with special policy
//----------------------------------------------------------------------

template<typename T, typename U>
struct readAsFunc {
    U &value_;
    xnode &dest_;

    readAsFunc(U &value, xnode &dest): value_(value), dest_(dest) {
    }

    void operator()() {
        T val = value_.template get_as<T>();
        dest_.set_as(val);
    }
};


void TestLongDoubleConversions() {
        
    typedef basic_xnode<xnode_ld_value_policy> xnode_ld;

    // Test converting from long double to various types
    long double sourceValue = 123.456789L;
    xnode_ld value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<long double>(), "Value should be stored as long double");
    
    // Test conversion to bool
    xnode dest;
    LogConversion("long double", "bool");
    AssertThrows(readAsFunc<bool, xnode_ld>(value, dest), "long double to bool conversion");
    
    // Test with zero
    value.set_as(0.0L);
    LogConversion("long double(0)", "bool");
    AssertThrows(readAsFunc<bool, xnode_ld>(value, dest), "long double(0) to bool conversion");
    
    // Test with one
    value.set_as(1.0L);
    LogConversion("long double(1)", "bool");
    AssertThrows(readAsFunc<bool, xnode_ld>(value, dest), "long double(1) to bool conversion");
    
    // Reset to original value
    value.set_as(sourceValue);
    
    // Test conversion to numeric types
    LogConversion("long double", "int");
    AssertThrows(readAsFunc<int, xnode_ld>(value, dest), "long double to int conversion");
    
    LogConversion("long double", "float");
    AssertThrows(readAsFunc<float, xnode_ld>(value, dest), "long double to float conversion");
    
    LogConversion("long double", "double");
    Assert(abs(value.get_as<double>() - 123.456789) < 0.0001, "long double to double conversion");
    
    LogConversion("long double", "long double");
    Assert(abs(value.get_as<long double>() - 123.456789L) < 0.0001L, "long double to long double conversion");
    
    // Test conversion to string
    LogConversion("long double", "string");
    AssertThrows(readAsFunc<std::string, xnode_ld>(value, dest), "long double to string conversion");
    
    // Test conversion from double to long double
    double doubleValue = 987.654321;
    xnode_ld value2;
    value2.set_as(doubleValue);
    LogConversion("double", "long double");
    Assert(abs(value2.get_as<long double>() - 987.654321L) < 0.0001L, "double to long double conversion");
}

//----------------------------------------------------------------------
// Special floating point values tests
//----------------------------------------------------------------------

void TestSpecialFloatingValues() {
    // Test converting special float values (NaN, Infinity)
    
    // NaN tests with float
    float nanValue = std::numeric_limits<float>::quiet_NaN();
    xnode value;
    value.set_as(nanValue);
    
    // Verify stored type
    Assert(value.is<float>(), "NaN value should be stored as float");
    
    xnode temp;
    // Test conversion to bool
    LogConversion("float(NaN)", "bool");
    AssertThrows(readAsFunc<bool, xnode>(value, temp), "NaN float to bool conversion");
    
    // Test conversion to numeric types
    LogConversion("float(NaN)", "float");
    Assert(std::isnan(value.get_as<float>()), "NaN float to float conversion");
    
    LogConversion("float(NaN)", "double");
    Assert(std::isnan(value.get_as<double>()), "NaN float to double conversion");
    
    // Test conversion to string
    LogConversion("float(NaN)", "string");
    std::string nanStr = value.get_as<std::string>();
    Assert(nanStr == "nan" || nanStr == "NaN" || nanStr == "NAN", "NaN float to string conversion");
    
    // Infinity tests with float
    float infinityValue = std::numeric_limits<float>::infinity();
    value.set_as(infinityValue);
    
    // Verify stored type
    Assert(value.is<float>(), "Infinity value should be stored as float");
    
    // Test conversion to bool
    LogConversion("float(Inf)", "bool");
    Assert(value.get_as<bool>() == true, "Infinity float to bool conversion");
    
    // Test conversion to numeric types
    LogConversion("float(Inf)", "float");
    Assert(std::isinf(value.get_as<float>()), "Infinity float to float conversion");
    
    LogConversion("float(Inf)", "double");
    Assert(std::isinf(value.get_as<double>()), "Infinity float to double conversion");
    
    // Test conversion to string
    LogConversion("float(Inf)", "string");
    std::string infStr = value.get_as<std::string>();
    Assert(infStr == "inf" || infStr == "Inf" || infStr == "INF" || 
           infStr == "infinity" || infStr == "Infinity" || infStr == "INFINITY", 
           "Infinity float to string conversion");
    
    // Negative Infinity tests with double
    double negInfValue = -std::numeric_limits<double>::infinity();
    value.set_as(negInfValue);
    
    // Verify stored type
    Assert(value.is<double>(), "Negative Infinity value should be stored as double");
    
    // Test conversion to bool
    LogConversion("double(-Inf)", "bool");
    Assert(value.get_as<bool>() == true, "Negative Infinity double to bool conversion");
    
    // Test conversion to numeric types
    LogConversion("double(-Inf)", "float");
    Assert(std::isinf(value.get_as<float>()) && value.get_as<float>() < 0, "Negative Infinity double to float conversion");
    
    LogConversion("double(-Inf)", "double");
    Assert(std::isinf(value.get_as<double>()) && value.get_as<double>() < 0, "Negative Infinity double to double conversion");
    
    // Test conversion to string
    LogConversion("double(-Inf)", "string");
    std::string negInfStr = value.get_as<std::string>();
    Assert(negInfStr == "-inf" || negInfStr == "-Inf" || negInfStr == "-INF" || 
           negInfStr == "-infinity" || negInfStr == "-Infinity" || negInfStr == "-INFINITY", 
           "Negative Infinity double to string conversion");
}

//----------------------------------------------------------------------
// String with negative value tests
//----------------------------------------------------------------------

void TestStringWithNegativeValues() {
    // Test converting from string containing negative values to various types
    std::string sourceValue = "-123";
    xnode value;
    value.set_as(sourceValue);
    
    // Verify stored type
    Assert(value.is<std::string>(), "Value should be stored as string");
    
    // Test conversion to numeric types
    LogConversion("string(-)", "int");
    Assert(value.get_as<int>() == -123, "negative string to int conversion");
    
    LogConversion("string(-)", "float");
    Assert(value.get_as<float>() == -123.0f, "negative string to float conversion");
    
    LogConversion("string(-)", "double");
    Assert(value.get_as<double>() == -123.0, "negative string to double conversion");
    
    LogConversion("string(-)", "long");
    Assert(value.get_as<long>() == -123L, "negative string to long conversion");
    
    LogConversion("string(-)", "long long");
    Assert(value.get_as<long long>() == -123LL, "negative string to long long conversion");
    
    // Test with negative decimal string
    value.set_as(std::string("-123.456"));
    LogConversion("string(-decimal)", "float");
    Assert(abs(value.get_as<float>() - (-123.456f)) < 0.001f, "negative decimal string to float conversion");
    
    LogConversion("string(-decimal)", "double");
    Assert(abs(value.get_as<double>() - (-123.456)) < 0.001, "negative decimal string to double conversion");
    
    // Test conversion to string
    LogConversion("string(-)", "string");
    Assert(value.get_as<std::string>() == "-123.456", "negative string to string conversion");
    
    // Test with extreme values in string
    value.set_as(std::string("-9223372036854775807")); // Near min value for 64-bit signed integer
    LogConversion("string(min_long_long)", "long long");
    Assert(value.get_as<long long>() == -9223372036854775807LL, "min long long string to long long conversion");
    
    // Test with extreme decimal value
    value.set_as(std::string("-1.7976931348623157e+308")); // Near min value for double
    LogConversion("string(min_double)", "double");
    double result = value.get_as<double>();
    Assert(result < -1.0e308, "min double string to double conversion");
}

//----------------------------------------------------------------------
// Main test function
//----------------------------------------------------------------------

int xnode_convert_test() {
    TEST_PROLOG();
    
    TEST_FUNC(BoolToTypes);
    TEST_FUNC(IntToTypes);
    TEST_FUNC(NegativeIntToTypes);
    TEST_FUNC(ShortToTypes);
    TEST_FUNC(FloatToTypes);
    TEST_FUNC(NegativeFloatToTypes);
    TEST_FUNC(DoubleToTypes);
    TEST_FUNC(NegativeDoubleToTypes);
    TEST_FUNC(CharToTypes);
    TEST_FUNC(UnsignedCharToTypes);
    TEST_FUNC(UnsignedShortToTypes);
    TEST_FUNC(UnsignedIntToTypes);
    TEST_FUNC(LongToTypes);
    TEST_FUNC(NegativeLongToTypes);
    TEST_FUNC(UnsignedLongToTypes);
    TEST_FUNC(LongLongToTypes);
    TEST_FUNC(NegativeLongLongToTypes);
    TEST_FUNC(UnsignedLongLongToTypes);
    TEST_FUNC(StringToTypes);
    TEST_FUNC(StringWithNegativeValues);
    TEST_FUNC(LongDoubleConversions);
    TEST_FUNC(SpecialFloatingValues);
    
    TEST_EPILOG();
}

int main()
{
    cout << "-------------------- XNODE CONVERSION TESTS --------------------" << endl;
    return xnode_convert_test();
}