//----------------------------------------------------------------------------------
// Name:        xarray_of_test.cpp
// Purpose:     Unit tests for xarray of() static initializer
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

void TestArrayOf() {
    // Test the static "of" initializer with different numbers of arguments
    
    // Empty array
    xarray empty = xarray::of_nodes();
    Assert(empty.empty(), "Empty array should be empty");
    Assert(empty.size() == 0, "Empty array size should be 0");
    
    // Single element (using appropriate version based on C++ standard)
#if __cplusplus >= 201703L
    // C++17: Use direct value
    xarray single = xarray::of(42);
#else
    // Pre-C++17: Use xnode objects
    xarray single = xarray::of(xnode::value_of(42));
#endif
    Assert(single.size() == 1, "Single element array size should be 1");
    Assert(single[0].get_as<int>() == 42, "Single element should be 42");
    
    // Multiple elements of the same type (using of_nodes which works on all standards)
    xarray multi = xarray::of_nodes(
        xnode::value_of(10),
        xnode::value_of(20),
        xnode::value_of(30)
    );
    Assert(multi.size() == 3, "Multi-element array size should be 3");
    Assert(multi[0].get_as<int>() == 10, "First element should be 10");
    Assert(multi[1].get_as<int>() == 20, "Second element should be 20");
    Assert(multi[2].get_as<int>() == 30, "Third element should be 30");
    
    // Different types
#if __cplusplus >= 201703L
    // C++17: Use direct values
    xarray mixed = xarray::of(10, "test", true, 3.14);
#else
    // Pre-C++17: Use xnode objects
    xarray mixed = xarray::of(
        xnode::value_of(10),
        xnode::value_of("test"),
        xnode::value_of(true),
        xnode::value_of(3.14)
    );
#endif
    Assert(mixed.size() == 4, "Mixed type array size should be 4");
    Assert(mixed[0].is<int>(), "First element should be int");
    Assert(mixed[1].is<std::string>(), "Second element should be string");
    Assert(mixed[2].is<bool>(), "Third element should be bool");
    Assert(mixed[3].is<double>(), "Fourth element should be double");
}

void TestArrayOfWithNestedStructures() {
    // Create an object
    xobject person;
    person.put("name", xnode::value_of("John Doe"));
    person.put("age", xnode::value_of(30));
    
    // Create a nested array
    xarray hobbies = xarray::of(
        xnode::value_of("reading"),
        xnode::value_of("coding"),
        xnode::value_of("hiking")
    );
    
    // Create main array with nested structures
    xarray data = xarray::of(
        xnode::value_of(person),
        xnode::value_of(hobbies),
        xnode::value_of(42)
    );
    
    // Verify the structure
    Assert(data.size() == 3, "Main array size should be 3");
    Assert(data[0].is<xobject>(), "First element should be xobject");
    Assert(data[1].is<xarray>(), "Second element should be xarray");
    Assert(data[2].is<int>(), "Third element should be int");
    
    // Verify nested object
    Assert(data[0].get_ptr<xobject>()->contains("name"), "Person object should contain name");
    Assert(data[0].get_ptr<xobject>()->get("name").get_as<std::string>() == "John Doe", 
           "Person name should be John Doe");
    
    // Verify nested array
    Assert(data[1].get_ptr<xarray>()->size() == 3, "Hobbies array should have 3 elements");
    Assert(data[1].get_ptr<xarray>()->at(0).get_as<std::string>() == "reading", 
           "First hobby should be reading");
}

void TestArrayOfIterations() {
    // Test iteration over array created with "of"
    xarray numbers = xarray::of(
        xnode::value_of(10),
        xnode::value_of(20),
        xnode::value_of(30),
        xnode::value_of(40)
    );
    
    // Range-based for loop
    int sum = 0;
    for (const auto& item : numbers) {
        sum += item.get_as<int>();
    }
    Assert(sum == 100, "Sum of elements should be 100");
    
    // STL algorithms with array created by "of"
    int product = 1;
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        product *= it->get_as<int>();
    }
    Assert(product == 240000, "Product of elements should be 240000");
}

int xarray_of_test() {
    TEST_PROLOG();
    TEST_FUNC(ArrayOf);
    TEST_FUNC(ArrayOfWithNestedStructures);
    TEST_FUNC(ArrayOfIterations);
    TEST_EPILOG();
}

int main() {
    return xarray_of_test();
}
