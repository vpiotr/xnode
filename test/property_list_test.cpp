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

// Test static of() methods
void TestStaticOfMethods() {
    // Test of() with 1 pair
    {
        auto props = property_list<string, int>::of("one", 1);
        Assert(props.size() == 1, "Should create list with 1 item");
        Assert(props.get("one") == 1, "Should have correct value for key 'one'");
    }
    
    // Test of() with 2 pairs
    {
        auto props = property_list<string, int>::of("one", 1, "two", 2);
        Assert(props.size() == 2, "Should create list with 2 items");
        Assert(props.get("one") == 1, "Should have correct value for key 'one'");
        Assert(props.get("two") == 2, "Should have correct value for key 'two'");
        
        // Check insertion order
        auto keys = props.get_keys();
        Assert(keys[0] == "one", "First key should be 'one'");
        Assert(keys[1] == "two", "Second key should be 'two'");
    }
    
    // Test of() with 3 pairs
    {
        auto props = property_list<string, int>::of("one", 1, "two", 2, "three", 3);
        Assert(props.size() == 3, "Should create list with 3 items");
        Assert(props.get("one") == 1, "Should have correct value for key 'one'");
        Assert(props.get("two") == 2, "Should have correct value for key 'two'");
        Assert(props.get("three") == 3, "Should have correct value for key 'three'");
        
        // Check insertion order
        auto keys = props.get_keys();
        Assert(keys[0] == "one", "First key should be 'one'");
        Assert(keys[1] == "two", "Second key should be 'two'");
        Assert(keys[2] == "three", "Third key should be 'three'");
    }
    
    // Test of() with 5 pairs
    {
        auto props = property_list<string, int>::of(
            "one", 1, "two", 2, "three", 3, "four", 4, "five", 5
        );
        Assert(props.size() == 5, "Should create list with 5 items");
        Assert(props.get("one") == 1, "Should have correct value for key 'one'");
        Assert(props.get("five") == 5, "Should have correct value for key 'five'");
        
        // Check insertion order preserved
        auto keys = props.get_keys();
        Assert(keys[0] == "one", "First key should be 'one'");
        Assert(keys[4] == "five", "Fifth key should be 'five'");
    }
    
    // Test of() with 10 pairs
    {
        auto props = property_list<string, int>::of(
            "one", 1, "two", 2, "three", 3, "four", 4, "five", 5,
            "six", 6, "seven", 7, "eight", 8, "nine", 9, "ten", 10
        );
        Assert(props.size() == 10, "Should create list with 10 items");
        Assert(props.get("one") == 1, "Should have correct value for key 'one'");
        Assert(props.get("ten") == 10, "Should have correct value for key 'ten'");
        
        // Check insertion order preserved
        auto keys = props.get_keys();
        Assert(keys[0] == "one", "First key should be 'one'");
        Assert(keys[9] == "ten", "Tenth key should be 'ten'");
    }
    
    // Test overwriting keys
    {
        auto props = property_list<string, int>::of(
            "one", 1, "two", 2, "one", 3
        );
        Assert(props.size() == 2, "Should have 2 items when key is repeated");
        Assert(props.get("one") == 3, "Should have last value for repeated key");
    }
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
        TestStaticOfMethods();
        
        std::cout << "All tests passed!" << std::endl;
        return 0;
    }
    catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }
}
