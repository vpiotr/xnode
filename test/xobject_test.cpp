//----------------------------------------------------------------------------------
// Name:        xobject_test.cpp
// Purpose:     Unit tests for xobject functionality
// Author:      Piotr Likus
// Created:     May 13, 2025
// License:     BSD
//----------------------------------------------------------------------------------

#include "xnode.h"
#include "xobject.h"
#include <iostream>
#include <sstream>
#include <numeric>
#include <algorithm>  // for std::find, std::sort

#include "cunit.h"

using namespace std;

void TestPropertyListPutGet() {
	xobject v;
	v.put("test1", xnode::value_of(12));
	v.put("test2", xnode::value_of(12L));
	v.put("test3", xnode::value_of(std::string("ala")));

	Assert(v.contains("test1"));
	Assert(v.get("test2").get_as<int>() == 12);
	Assert(v.get_ptr("test3") != nullptr);
	Assert(v.get_ptr("test3")->get_as<std::string>() == "ala");
}

void TestPropertyListKeysInOrder() {
	xobject v;
	v.put("z", xnode::value_of(12));
	v.put("a", xnode::value_of(1L));
	v.put("ba", xnode::value_of(std::string("ala")));
	v.put("d", xnode::value_of(3));

	std::vector<std::string> keys = v.get_keys();
	Assert(keys[0] == "z");
	Assert(keys[1] == "a");
	Assert(keys[2] == "ba");
	Assert(keys[3] == "d");

	v.remove("a");
	keys = v.get_keys();
	Assert(keys[2] == "d");
	Assert(v.get("d").is<int>());
	Assert(v.get("d").get_as<int>() == 3);
}

void TestPropertyListValuesInOrder() {
	xobject v;
	v.put("z", xnode::value_of(12));
	v.put("a", xnode::value_of(1L));
	v.put("ba", xnode::value_of(std::string("ala")));
	v.put("d", xnode::value_of(3));

	std::vector<xnode> values = v.get_values();
	Assert(values[0].get_as<int>() == 12);
	Assert(values[1].get_as<int>() == 1);
	Assert(values[2].get_as<std::string>() == "ala");
	Assert(values[3].get_as<int>() == 3);

	v.remove("a");
	values = v.get_values();
	Assert(values[2].get_as<int>() == 3);
}

void TestPropertyListSum() {
	xobject v;
	v.put("z", xnode::value_of(12));
	v.put("a", xnode::value_of(1L));
	v.put("ba", xnode::value_of(7));
	v.put("d", xnode::value_of(3));

	auto keys = v.get_keys();
    Assert(keys.size() == 4);

	int sum = 0;
	for (std::string key : keys)
		sum += v.get(key).get_as<int>();

	Assert(sum == 23);
}

void TestPropertyListKeysNoCopy() {
    // faster version of get_keys

	xobject v;
	v.put("z", xnode::value_of(12));
	v.put("a", xnode::value_of(1L));
	v.put("ba", xnode::value_of(7));
	v.put("bc", xnode::value_of(1));

    v.remove("a");

    xobject::key_list_type keys_helper;
	auto keys = v.get_keys(keys_helper);

    Assert(keys.size() == 3);
}

bool checkStructOk(const xnode &value) {
    // d,z are required int values
    bool result =
        value.is<xobject>() &&
        value.get_ptr<xobject>()->contains("z") &&
        value.get_ptr<xobject>()->contains("d") &&
        value.get_ptr<xobject>()->get("z").is<int>() &&
        value.get_ptr<xobject>()->get("d").is<int>();

    // a is optional long
    if (result)
        if (value.get_ptr<xobject>()->contains("a"))
            result = value.get_ptr<xobject>()->get("a").is<long>();

    return result;
}

void TestIterators() {
    // Create a test object with some key-value pairs
    xobject obj;
    obj.put("one", xnode::value_of(1));
    obj.put("two", xnode::value_of(2));
    obj.put("three", xnode::value_of(3));
    obj.put("four", xnode::value_of(4));
    
    // Test keys_begin and keys_end iterators
    {
        std::vector<std::string> keys;
        int keyCount = 0;
        
        for (auto it = obj.keys_begin(); it != obj.keys_end(); ++it) {
            keys.push_back(*it);
            keyCount++;
        }
        
        Assert(keyCount == 4, "Should iterate over 4 keys");
        Assert(keys[0] == "one", "First key should be 'one'");
        Assert(keys[1] == "two", "Second key should be 'two'");
        Assert(keys[2] == "three", "Third key should be 'three'");
        Assert(keys[3] == "four", "Fourth key should be 'four'");
    }
    
    // Test keys_cbegin and keys_cend const iterators
    {
        const xobject& const_obj = obj;
        int sum = 0;
        int keyCount = 0;
        
        for (auto it = const_obj.keys_cbegin(); it != const_obj.keys_cend(); ++it) {
            sum += const_obj.get(*it).get_as<int>();
            keyCount++;
        }
        
        Assert(keyCount == 4, "Should iterate over 4 keys with const iterators");
        Assert(sum == 10, "Sum of values should be 10");
    }
    
    // Test values_begin and values_end iterators
    {
        int sum = 0;
        int valueCount = 0;
        
        for (auto it = obj.values_begin(); it != obj.values_end(); ++it) {
            sum += it->second.get_as<int>();
            valueCount++;
        }
        
        Assert(valueCount == 4, "Should iterate over 4 values");
        Assert(sum == 10, "Sum of values should be 10");
    }
    
    // Test values_cbegin and values_cend const iterators
    {
        const xobject& const_obj = obj;
        int sum = 0;
        int valueCount = 0;
        
        for (auto it = const_obj.values_cbegin(); it != const_obj.values_cend(); ++it) {
            sum += it->second.get_as<int>();
            valueCount++;
        }
        
        Assert(valueCount == 4, "Should iterate over 4 values with const iterators");
        Assert(sum == 10, "Sum of values should be 10");
    }
    
    // Test iterator with reorganization
    {
        obj.remove("two");
        
        std::vector<std::string> keys;
        for (auto it = obj.keys_begin(); it != obj.keys_end(); ++it) {
            keys.push_back(*it);
        }
        
        Assert(keys.size() == 3, "Should have 3 keys after removal");
        Assert(std::find(keys.begin(), keys.end(), "two") == keys.end(), "Removed key should not be found");
        Assert(!obj.needs_reorg(), "Keys should be reorganized during iteration");
    }
    
    // Test with range-based for loop
    {
        int sum = 0;
        std::vector<std::string> keys = obj.get_keys();
        
        for (const auto& key : keys) {
            sum += obj.get(key).get_as<int>();
        }
        
        Assert(sum == 8, "Sum after removal should be 8");
    }
    
    // Test using iterators with std algorithms
    {
        std::vector<std::string> keys(obj.keys_begin(), obj.keys_end());
        Assert(keys.size() == 3, "Should copy 3 keys to vector");
        
        std::sort(keys.begin(), keys.end());
        Assert(keys[0] == "four", "First key after sorting should be 'four'");
        Assert(keys[1] == "one", "Second key after sorting should be 'one'");
        Assert(keys[2] == "three", "Third key after sorting should be 'three'");
    }
}

std::string printInFont(const xnode &font, const std::string &text) {
	std::ostringstream out;

    const xobject &list = font.get_ref<xobject>();

    out << "text in font [";
    out << "color:" << list.get_def("color", xnode::value_of(0x00ff00)).get_as<int>();
    out << ", font_name:" << list.get_def("font_name", xnode::value_of("courier")).get_as<std::string>();
    out << ", size:" << list.get_def("size", xnode::value_of(10)).get_as<int>();
    out << ", bold:" << list.get_def("bold", xnode::value_of(false)).get_as<bool>();
    out << "] = " << text;

	return(out.str());
}

void TestOptionalNamedParams() {
	xobject v;
	v.put("color", xnode::value_of(0xff0000));
	v.put("font_name", xnode::value_of(std::string("arial")));
	v.put("size", xnode::value_of(12));

    xnode value = std::move(xnode::value_of(v));

    std::string s = printInFont(value, "test");
    Assert(s.find("bold:") != std::string::npos);
}

void TestDefNamedParams() {
	xobject v;
	v.put("z", xnode::value_of(12));
	v.put("a", xnode::value_of(1L));
	v.put("d", xnode::value_of(7));

    Assert(v.get_def("c", xnode::value_of(-1)).get_as<int>() == -1);
}

void TestStaticOfMethod() {
    // Test with 3 key-value pairs of different types (int, string, float)
    auto obj = xobject::of(
        "intKey", xnode::value_of(42),
        "strKey", xnode::value_of(std::string("hello")),
        "floatKey", xnode::value_of(3.14f)
    );
    
    // Assert size is correct
    Assert(obj.size() == 3, "Object should contain 3 items");
    
    // Assert all keys exist
    Assert(obj.contains("intKey"), "Object should contain 'intKey'");
    Assert(obj.contains("strKey"), "Object should contain 'strKey'");
    Assert(obj.contains("floatKey"), "Object should contain 'floatKey'");
    
    // Assert values are of correct types
    Assert(obj.get("intKey").is<int>(), "intKey should be of type int");
    Assert(obj.get("strKey").is<std::string>(), "strKey should be of type string");
    Assert(obj.get("floatKey").is<float>(), "floatKey should be of type float");
    
    // Assert values are correct
    Assert(obj.get("intKey").get_as<int>() == 42, "intKey should have value 42");
    Assert(obj.get("strKey").get_as<std::string>() == "hello", "strKey should have value 'hello'");
    Assert(obj.get("floatKey").get_as<float>() == 3.14f, "floatKey should have value 3.14f");
    
    // Assert insertion order is preserved
    std::vector<std::string> keys = obj.get_keys();
    Assert(keys.size() == 3, "Keys vector should contain 3 items");
    Assert(keys[0] == "intKey", "First key should be 'intKey'");
    Assert(keys[1] == "strKey", "Second key should be 'strKey'");
    Assert(keys[2] == "floatKey", "Third key should be 'floatKey'");
    
    // Test with other value types
    auto obj2 = xobject::of(
        "boolKey", xnode::value_of(true),
        "doubleKey", xnode::value_of(2.71828),
        "longKey", xnode::value_of(9223372036854775807L) // max long
    );
    
    Assert(obj2.size() == 3, "Object should contain 3 items");
    Assert(obj2.get("boolKey").get_as<bool>() == true, "boolKey should have value true");
    Assert(obj2.get("doubleKey").get_as<double>() == 2.71828, "doubleKey should have value 2.71828");
    Assert(obj2.get("longKey").get_as<long>() == 9223372036854775807L, "longKey should have correct value");
    
    // Test with empty string and zero values
    auto obj3 = xobject::of(
        "emptyStr", xnode::value_of(std::string("")),
        "zero", xnode::value_of(0),
        "zeroF", xnode::value_of(0.0f)
    );
    
    Assert(obj3.get("emptyStr").get_as<std::string>() == "", "emptyStr should be empty");
    Assert(obj3.get("zero").get_as<int>() == 0, "zero should be 0");
    Assert(obj3.get("zeroF").get_as<float>() == 0.0f, "zeroF should be 0.0f");
}

int xobject_test() {
	TEST_PROLOG();
	TEST_FUNC(PropertyListPutGet);
	TEST_FUNC(PropertyListKeysInOrder);
	TEST_FUNC(PropertyListValuesInOrder);
	TEST_FUNC(PropertyListSum);
    TEST_FUNC(PropertyListKeysNoCopy);
    TEST_FUNC(OptionalNamedParams);
    TEST_FUNC(DefNamedParams);
    TEST_FUNC(StaticOfMethod);
    TEST_FUNC(Iterators);
	TEST_EPILOG();
}

int main()
{
	return xobject_test();
}
