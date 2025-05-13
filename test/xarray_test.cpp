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

// for std::sort, std::is_sorted
#include <algorithm>

#include "cunit.h"

using namespace std;

void TestArraySum() {
	// Reserve capacity upfront to avoid reallocations
	xarray v;
	v.reserve(5);
	
	// Add elements to the array (using initializer list would be nicer if supported)
	v.push_back(xnode::value_of(1));
	v.push_back(xnode::value_of(8));
	v.push_back(xnode::value_of(2));
	v.push_back(xnode::value_of<long>(3));
	v.push_back(xnode::value_of<long>(4));

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
	xarray v;
	v.push_back(xnode::value_of(1));
	v.push_back(xnode::value_of(18));
	v.push_back(xnode::value_of(128));
	v.push_back(xnode::value_of(3));
	v.push_back(xnode::value_of(23));

	std::sort(v.begin(), v.end());
	Assert(std::is_sorted(v.begin(), v.end()));
}

int xarray_test() {
	TEST_PROLOG();
	TEST_FUNC(ArraySum);
	TEST_FUNC(ArraySort);
	TEST_EPILOG();
}

int main()
{
	return xarray_test();
}
