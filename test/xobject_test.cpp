//----------------------------------------------------------------------------------
// Name:        xobject_test.cpp
// Purpose:     Unit tests for xobject functionality
// Author:      Piotr Likus
// Created:     May 13, 2025
// License:     BSD
//----------------------------------------------------------------------------------

// Disable warnings from template instantiations in header files
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wnonnull"
#endif

#include "xnode.h"
#include "xobject.h"
#include <iostream>
#include <sstream>
#include <numeric>

#include "cunit.h"

// Restore warnings after includes
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

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

// TestTreeOfNodesWithNamedList moved to xnode_tree_test.cpp

int xobject_test() {
	TEST_PROLOG();
	TEST_FUNC(PropertyListPutGet);
	TEST_FUNC(PropertyListKeysInOrder);
	TEST_FUNC(PropertyListValuesInOrder);
	TEST_FUNC(PropertyListSum);
    TEST_FUNC(PropertyListKeysNoCopy);
    TEST_FUNC(OptionalNamedParams);
    TEST_FUNC(DefNamedParams);
    // TEST_FUNC(TreeOfNodesWithNamedList); // Moved to xnode_tree_test.cpp
	TEST_EPILOG();
}

int main()
{
	return xobject_test();
}
