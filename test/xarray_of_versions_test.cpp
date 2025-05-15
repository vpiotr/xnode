//----------------------------------------------------------------------------------
// Name:        xarray_of_versions_test.cpp
// Purpose:     Unit tests for xarray of() and of_nodes() static initializers
// Author:      Piotr Likus
// Created:     May 15, 2025
// License:     BSD
//----------------------------------------------------------------------------------

#include "xnode.h"
#include "xarray.h"
#include <iostream>
#include <string>
#include "cunit.h"
#include "xobject.h"

using namespace std;

#if __cplusplus >= 201703L
void TestArrayOfWithAutomaticConversion() {
    // Test the C++17 "of" initializer with automatic value conversion
    
    // Basic types
    xarray numbers = xarray::of(10, 20, 30);
    Assert(numbers.size() == 3, "Array size should be 3");
    Assert(numbers[0].get_as<int>() == 10, "First element should be 10");
    Assert(numbers[1].get_as<int>() == 20, "Second element should be 20");
    Assert(numbers[2].get_as<int>() == 30, "Third element should be 30");
    
    // Mixed types
    xarray mixed = xarray::of(42, "hello", 3.14, true);
    Assert(mixed.size() == 4, "Mixed array size should be 4");
    Assert(mixed[0].get_as<int>() == 42, "First element should be 42");
    Assert(mixed[1].get_as<std::string>() == "hello", "Second element should be 'hello'");
    Assert(abs(mixed[2].get_as<double>() - 3.14) < 0.0001, "Third element should be 3.14");
    Assert(mixed[3].get_as<bool>() == true, "Fourth element should be true");
}
#endif

void TestArrayOfNodesVersions() {
    // Test the "of_nodes" initializer (compatible with all C++ standards)
    
    // Empty array
    xarray empty = xarray::of_nodes();
    Assert(empty.empty(), "Empty array should be empty");
    
    // Single element
    xarray single = xarray::of_nodes(xnode::value_of(100));
    Assert(single.size() == 1, "Single element array size should be 1");
    Assert(single[0].get_as<int>() == 100, "Element should be 100");
    
    // Multiple elements
    xarray multi = xarray::of_nodes(
        xnode::value_of(1),
        xnode::value_of(2),
        xnode::value_of(3)
    );
    Assert(multi.size() == 3, "Multi-element array size should be 3");
    Assert(multi[0].get_as<int>() == 1, "First element should be 1");
    Assert(multi[1].get_as<int>() == 2, "Second element should be 2");
    Assert(multi[2].get_as<int>() == 3, "Third element should be 3");
    
    // Mixed types
    xarray mixed = xarray::of_nodes(
        xnode::value_of(123),
        xnode::value_of("string value"),
        xnode::value_of(4.56),
        xnode::value_of(false)
    );
    Assert(mixed.size() == 4, "Mixed type array size should be 4");
    Assert(mixed[0].is<int>(), "First element should be int");
    Assert(mixed[1].is<std::string>(), "Second element should be string");
    Assert(mixed[2].is<double>(), "Third element should be double");
    Assert(mixed[3].is<bool>(), "Fourth element should be bool");
}

void TestArrayOfCompatibility() {
    // Test the compatibility between different of() implementations
    
    // Create arrays using both methods
#if __cplusplus >= 201703L
    xarray direct = xarray::of(10, 20, 30);
#else
    xarray direct = xarray::of(
        xnode::value_of(10),
        xnode::value_of(20),
        xnode::value_of(30)
    );
#endif

    xarray nodes = xarray::of_nodes(
        xnode::value_of(10),
        xnode::value_of(20),
        xnode::value_of(30)
    );
    
    // Verify they're the same
    Assert(direct.size() == nodes.size(), "Both arrays should have the same size");
    
    for (size_t i = 0; i < direct.size(); ++i) {
        Assert(direct[i].get_as<int>() == nodes[i].get_as<int>(), 
               "Values should match between arrays");
    }
    
    // Test equality operators
    Assert(direct == nodes, "Arrays should be equal");
}

int xarray_of_versions_test() {
    TEST_PROLOG();
#if __cplusplus >= 201703L
    TEST_FUNC(ArrayOfWithAutomaticConversion);
#endif
    TEST_FUNC(ArrayOfNodesVersions);
    TEST_FUNC(ArrayOfCompatibility);
    TEST_EPILOG();
}

int main() {
    return xarray_of_versions_test();
}
