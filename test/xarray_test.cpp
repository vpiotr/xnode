//----------------------------------------------------------------------------------
// Name:        xarray_test.cpp
// Purpose:     Unit tests for xarray functionality
// Author:      Piotr Likus
// Created:     May 13, 2025
// License:     BSD
//----------------------------------------------------------------------------------

#include "xnode.h"
#include "xarray.h"
#include <iostream>

// for std::accumulate
#include <numeric>

// for std::sort, std::is_sorted, std::find_if, std::transform, std::copy_if
#include <algorithm>

// for std::back_inserter
#include <iterator>

// for std::to_string
#include <string>

#include "cunit.h"
#include "xobject.h"

using namespace std;

void TestArraySum() {
	// Create array using the new of() static initializer
	xarray v = xarray::of(
		xnode::value_of(1),
		xnode::value_of(8),
		xnode::value_of(2),
		xnode::value_of<long>(3),
		xnode::value_of<long>(4)
	);
	
	// Sum as integers using std::accumulate with a lambda that extracts int values
	int intSum = std::accumulate(v.begin(), v.end(), 0, 
		[](int acc, const xnode& x) { return acc + x.get_as<int>(); });
	Assert(intSum == 18);
	
	// Sum as longs using std::accumulate with a lambda that extracts long values
	long longSum = std::accumulate(v.begin(), v.end(), 0L,
		[](long acc, const xnode& x) { return acc + x.get_as<long>(); });
	Assert(longSum == 18L);
}

void TestArraySort() {
	xarray v = xarray::of(
		xnode::value_of(1),
		xnode::value_of(18),
		xnode::value_of(128),
		xnode::value_of(3),
		xnode::value_of(23)
	);

	std::sort(v.begin(), v.end());
	Assert(std::is_sorted(v.begin(), v.end()));
}

void TestArrayFind() {
	// Demonstrate finding elements in an xarray
	xarray v;
	v.push_back(xnode::value_of(10));
	v.push_back(xnode::value_of(20));
	v.push_back(xnode::value_of(30));
	v.push_back(xnode::value_of(40));
	v.push_back(xnode::value_of("test string"));
	v.push_back(xnode::value_of(true));
	
	// Find using find_if with a lambda - finding a specific int value
	auto it = std::find_if(v.begin(), v.end(), [](const xnode& n) { 
		return n.is<int>() && n.get_as<int>() == 30; 
	});
	Assert(it != v.end(), "Failed to find element with value 30");
	Assert(it->get_as<int>() == 30, "Found element has incorrect value");
	
	// Find using find_if with a lambda - finding a string
	auto strIt = std::find_if(v.begin(), v.end(), [](const xnode& n) { 
		return n.is<std::string>() && n.get_as<std::string>() == "test string"; 
	});
	Assert(strIt != v.end(), "Failed to find element with string value");
	Assert(strIt->get_as<std::string>() == "test string", "Found string element has incorrect value");
	
	// Count elements of a specific type
	int intCount = std::count_if(v.begin(), v.end(), [](const xnode& n) { return n.is<int>(); });
	Assert(intCount == 4, "Incorrect count of int elements");
}

void TestArrayTransform() {
	// Demonstrate transforming an xarray
	xarray source;
	source.push_back(xnode::value_of(1));
	source.push_back(xnode::value_of(2));
	source.push_back(xnode::value_of(3));
	source.push_back(xnode::value_of(4));
	source.push_back(xnode::value_of(5));
	
	// Transform to a new array with doubled values
	xarray result;
	result.reserve(source.size());
	
	std::transform(source.begin(), source.end(), std::back_inserter(result), 
		[](const xnode& n) { return xnode::value_of(n.get_as<int>() * 2); });
	
	// Verify transformation
	Assert(result.size() == source.size(), "Result size should match source size");
	for (size_t i = 0; i < source.size(); ++i) {
		Assert(result[i].get_as<int>() == source[i].get_as<int>() * 2, 
			"Transformed value should be double the original");
	}
}

void TestArrayMixedTypes() {
	// Demonstrate storing and processing mixed data types in an xarray
	xarray v;
	
	// Add different types
	v.push_back(xnode::value_of(42));              // int
	v.push_back(xnode::value_of(3.14159));         // double
	v.push_back(xnode::value_of("Hello, world!")); // string
	v.push_back(xnode::value_of(true));            // bool
	
	// Create a long value explicitly to avoid ambiguity
	xnode longNode;
	long longValue = 1000000L;
	longNode.set_as(longValue);
	v.push_back(longNode);  // long
	
	// Check types and values
	Assert(v[0].is<int>(), "Element 0 should be an int");
	Assert(v[1].is<double>(), "Element 1 should be a double");
	Assert(v[2].is<std::string>(), "Element 2 should be a string");
	Assert(v[3].is<bool>(), "Element 3 should be a bool");
	Assert(v[4].is<long>(), "Element 4 should be a long");
	
	// Create a string representation of each element
	std::vector<std::string> stringReps;
	for (const auto& node : v) {
		if (node.is<std::string>()) {
			stringReps.push_back(node.get_as<std::string>());
		} else if (node.is<int>()) {
			stringReps.push_back("Int: " + std::to_string(node.get_as<int>()));
		} else if (node.is<double>()) {
			stringReps.push_back("Double: " + std::to_string(node.get_as<double>()));
		} else if (node.is<bool>()) {
			stringReps.push_back(node.get_as<bool>() ? "Bool: true" : "Bool: false");
		} else if (node.is<long>()) {
			stringReps.push_back("Long: " + std::to_string(node.get_as<long>()));
		}
	}
	
	// Verify string representations
	Assert(stringReps.size() == 5, "Should have 5 string representations");
	Assert(stringReps[0] == "Int: 42", "First string rep should be 'Int: 42'");
	Assert(stringReps[3] == "Bool: true", "Fourth string rep should be 'Bool: true'");
}

void TestArrayFilter() {
	// Demonstrate filtering an array
	xarray original;
	for (int i = 1; i <= 10; ++i) {
		original.push_back(xnode::value_of(i));
	}
	
	// Create a filtered copy with only even numbers
	xarray evens;
	std::copy_if(original.begin(), original.end(), std::back_inserter(evens),
		[](const xnode& n) { return n.get_as<int>() % 2 == 0; });
	
	// Verify filter results
	Assert(evens.size() == 5, "Filtered array should have 5 elements");
	for (const auto& n : evens) {
		Assert(n.get_as<int>() % 2 == 0, "All elements should be even");
	}
}

void TestArrayNestedStructures() {
	// Demonstrate an xarray containing other containers (objects and arrays)
	xarray root;
	
	// Add primitive types
	root.push_back(xnode::value_of(1));
	root.push_back(xnode::value_of("root level string"));
	
	// Create and add an object
	xobject obj;
	obj.put("name", xnode::value_of(std::string("test object")));
	obj.put("value", xnode::value_of(42));
	root.push_back(xnode::value_of(obj));
	
	// Create and add a nested array
	xarray nested;
	nested.push_back(xnode::value_of(10));
	nested.push_back(xnode::value_of(20));
	nested.push_back(xnode::value_of("nested string"));
	root.push_back(xnode::value_of(nested));
	
	// Verify root structure
	Assert(root.size() == 4, "Root array should have 4 elements");
	Assert(root[0].is<int>(), "First element should be int");
	Assert(root[1].is<std::string>(), "Second element should be string");
	Assert(root[2].is<xobject>(), "Third element should be xobject");
	Assert(root[3].is<xarray>(), "Fourth element should be xarray");
	
	// Verify object content
	Assert(root[2].get_ptr<xobject>()->contains("name"), "Object should contain 'name' property");
	Assert(root[2].get_ptr<xobject>()->get("value").get_as<int>() == 42, "Object 'value' should be 42");
	
	// Verify nested array content
	Assert(root[3].get_ptr<xarray>()->size() == 3, "Nested array should have 3 elements");
	Assert(root[3].get_ptr<xarray>()->at(2).get_as<std::string>() == "nested string", 
		"Nested array string element has wrong value");
}

void TestArrayCopy() {
	// Demonstrate copying and comparing arrays
	xarray original;
	original.reserve(3);
	original.push_back(xnode::value_of(1));
	original.push_back(xnode::value_of(2));
	original.push_back(xnode::value_of(3));
	
	// Test copying using vector's copy constructor
	xarray copy(original);
	
	// Verify the copy
	Assert(copy.size() == original.size(), "Copy size should match original");
	for (size_t i = 0; i < original.size(); ++i) {
		Assert(copy[i].get_as<int>() == original[i].get_as<int>(), 
			"Copy values should match original");
	}
	
	// Modify the copy and ensure original is unchanged
	copy[0].set_as(100);
	Assert(original[0].get_as<int>() == 1, "Original should be unchanged");
	Assert(copy[0].get_as<int>() == 100, "Copy should be modified");
}

void TestArrayIteration() {
	// Demonstrate various ways to iterate through an xarray
	xarray v = xarray::of(
		xnode::value_of(10),
		xnode::value_of(20),
		xnode::value_of(30)
	);
	
	// Method 1: Index-based for loop
	int sum1 = 0;
	for (size_t i = 0; i < v.size(); ++i) {
		sum1 += v[i].get_as<int>();
	}
	Assert(sum1 == 60, "Index-based sum should be 60");
	
	// Method 2: Iterator-based for loop
	int sum2 = 0;
	for (xarray::iterator it = v.begin(); it != v.end(); ++it) {
		sum2 += it->get_as<int>();
	}
	Assert(sum2 == 60, "Iterator-based sum should be 60");
	
	// Method 3: Range-based for loop (C++11)
	int sum3 = 0;
	for (const auto& node : v) {
		sum3 += node.get_as<int>();
	}
	Assert(sum3 == 60, "Range-based sum should be 60");
	
	// Method 4: Use STL algorithm
	int sum4 = 0;
	std::for_each(v.begin(), v.end(), [&sum4](const xnode& node) {
		sum4 += node.get_as<int>();
	});
	Assert(sum4 == 60, "STL algorithm sum should be 60");
}

void TestArrayManipulation() {
	// Demonstrate various vector manipulation operations on xarray
	xarray v;
	
	// Test push_back
	v.push_back(xnode::value_of(1));
	v.push_back(xnode::value_of(2));
	Assert(v.size() == 2, "Size should be 2 after push_back");
	
	// Test insert
	v.insert(v.begin() + 1, xnode::value_of(3));
	Assert(v.size() == 3, "Size should be 3 after insert");
	Assert(v[1].get_as<int>() == 3, "Inserted value should be 3");
	
	// Test erase
	v.erase(v.begin());
	Assert(v.size() == 2, "Size should be 2 after erase");
	Assert(v[0].get_as<int>() == 3, "First value should be 3 after erase");
	
	// Test clear
	v.clear();
	Assert(v.empty(), "Array should be empty after clear");
	
	// Test resize
	v.resize(3, xnode::value_of(5));
	Assert(v.size() == 3, "Size should be 3 after resize");
	Assert(v[0].get_as<int>() == 5, "Resized element should have default value");
}

int xarray_test() {
	TEST_PROLOG();
	TEST_FUNC(ArraySum);
	TEST_FUNC(ArraySort);
	TEST_FUNC(ArrayFind);
	TEST_FUNC(ArrayTransform);
	TEST_FUNC(ArrayMixedTypes);
	TEST_FUNC(ArrayFilter);
	TEST_FUNC(ArrayNestedStructures);
	TEST_FUNC(ArrayCopy);
	TEST_FUNC(ArrayIteration);
	TEST_FUNC(ArrayManipulation);
	TEST_EPILOG();
}

int main()
{
	return xarray_test();
}
