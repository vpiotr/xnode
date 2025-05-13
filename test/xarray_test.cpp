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

template<typename T>
xnode mysum(xnode x, xnode y) {
	xnode result;
	T value = x.get_as<T>() + y.get_as<T>();
	result.set_as(value);
	return result;
}

void TestArraySum() {
	xarray v;
	v.push_back(xnode::value_of(1));
	v.push_back(xnode::value_of(8));
	v.push_back(xnode::value_of(2));
	v.push_back(xnode::value_of<long>(3));
	v.push_back(xnode::value_of<long>(4));

	xnode sum = std::accumulate(v.begin(), v.end(), xnode::value_of(0), mysum<int>);
	int isum = sum.get_as<int>();
	Assert(isum == 18);
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
