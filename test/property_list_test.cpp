//----------------------------------------------------------------------------------
// Name:        property_list_test.cpp
// Purpose:     Unit tests for property_list class
// Author:      Piotr Likus
// Created:     15/05/2025
// License:     BSD
//----------------------------------------------------------------------------------

#include "../include/details/property_list.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "cunit.h"

using namespace std;

// Test keys iterators
void TestKeysIterators() {
    property_list<string, int> props;
    
    // Add some test data
    props.put("one", 1);
    props.put("two", 2);
    props.put("three", 3);
    
    // Test keys_begin() and keys_end()
    {
        std::vector<string> keys;
        for (auto it = props.keys_begin(); it != props.keys_end(); ++it) {
            keys.push_back(*it);
        }
        
        // Check iteration order matches insertion order
        Assert(keys.size() == 3, "Should iterate over 3 keys");
        Assert(keys[0] == "one", "First key should be 'one'");
        Assert(keys[1] == "two", "Second key should be 'two'");
        Assert(keys[2] == "three", "Third key should be 'three'");
    }
    
    // Test for-range loop with begin/end
    {
        std::vector<string> keys;
        for (const auto& key : props.get_keys()) {
            keys.push_back(key);
        }
        
        Assert(keys.size() == 3, "Range-based for should iterate over 3 keys");
        Assert(keys[0] == "one", "First key should be 'one'");
        Assert(keys[1] == "two", "Second key should be 'two'");
        Assert(keys[2] == "three", "Third key should be 'three'");
    }
}

// Test const keys iterators
void TestConstKeysIterators() {
    property_list<string, int> props;
    
    // Add some test data
    props.put("one", 1);
    props.put("two", 2);
    props.put("three", 3);
    
    // Test with const object
    const property_list<string, int>& const_props = props;
    
    // Test keys_cbegin() and keys_cend()
    {
        std::vector<string> keys;
        for (auto it = const_props.keys_cbegin(); it != const_props.keys_cend(); ++it) {
            keys.push_back(*it);
        }
        
        // Check iteration order matches insertion order
        Assert(keys.size() == 3, "Should iterate over 3 keys with const iterators");
        Assert(keys[0] == "one", "First key should be 'one'");
        Assert(keys[1] == "two", "Second key should be 'two'");
        Assert(keys[2] == "three", "Third key should be 'three'");
    }
}

// Test values iterators
void TestValuesIterators() {
    property_list<string, int> props;
    
    // Add some test data
    props.put("one", 1);
    props.put("two", 2);
    props.put("three", 3);
    
    // Test values_begin() and values_end()
    {
        int sum = 0;
        for (auto it = props.values_begin(); it != props.values_end(); ++it) {
            sum += it->second;
        }
        
        // Check sum of values
        Assert(sum == 6, "Sum of values should be 6");
    }
    
    // Test finding a specific value using values iterator
    {
        bool found_two = false;
        for (auto it = props.values_begin(); it != props.values_end(); ++it) {
            if (it->first == "two" && it->second == 2) {
                found_two = true;
                break;
            }
        }
        
        Assert(found_two, "Should find value 2 for key 'two'");
    }
}

// Test const values iterators
void TestConstValuesIterators() {
    property_list<string, int> props;
    
    // Add some test data
    props.put("one", 1);
    props.put("two", 2);
    props.put("three", 3);
    
    // Test with const object
    const property_list<string, int>& const_props = props;
    
    // Test values_cbegin() and values_cend()
    {
        int sum = 0;
        for (auto it = const_props.values_cbegin(); it != const_props.values_cend(); ++it) {
            sum += it->second;
        }
        
        // Check sum of values
        Assert(sum == 6, "Sum of values with const iterators should be 6");
    }
}

// Test key iterators with reorganization
void TestKeysIteratorsWithReorg() {
    property_list<string, int> props;
    
    // Add some test data
    props.put("one", 1);
    props.put("two", 2);
    props.put("three", 3);
    
    // Remove an item to mark keys as dirty
    props.remove("two");
    
    // Test keys_begin() and keys_end() after removal
    // This should trigger reorg
    {
        std::vector<string> keys;
        for (auto it = props.keys_begin(); it != props.keys_end(); ++it) {
            keys.push_back(*it);
        }
        
        // Check iteration order matches insertion order minus removed item
        Assert(keys.size() == 2, "Should iterate over 2 keys after removal");
        Assert(keys[0] == "one", "First key should be 'one'");
        Assert(keys[1] == "three", "Second key should be 'three'");
        
        // Verify dirty_keys_ was reset by checking needs_reorg()
        Assert(!props.needs_reorg(), "Should not need reorg after iterator use");
    }
}

// Test iterator modification
void TestKeyIteratorModification() {
    // This is just a compile-time test to ensure key iterators work with standard algorithms
    property_list<string, int> props;
    props.put("one", 1);
    props.put("two", 2);
    
    // Test with standard algorithm that requires modifiable iterators
    std::vector<string> keys(props.keys_begin(), props.keys_end());
    std::sort(keys.begin(), keys.end());
    
    Assert(keys.size() == 2, "Should copy 2 keys to vector");
}

// Main test runner
int main() {
    try {
        std::cout << "Running property_list iterator tests..." << std::endl;
        
        TestKeysIterators();
        TestConstKeysIterators();
        TestValuesIterators();
        TestConstValuesIterators();
        TestKeysIteratorsWithReorg();
        TestKeyIteratorModification();
        
        std::cout << "All tests passed!" << std::endl;
        return 0;
    }
    catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }
}
