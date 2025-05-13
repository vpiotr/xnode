//----------------------------------------------------------------------------------
// Name:        xnode_tree_test.cpp
// Purpose:     Unit tests for tree structure of xnode
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
#include "xarray.h"
#include "xobject.h"
#include <iostream>

// for std::accumulate
#include <numeric>

#include "cunit.h"

// Restore warnings after includes
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

using namespace std;

void TestTreeOfNodesWithArray() {
	xnode root;
	xarray arr1;
	xnode value;

	arr1.push_back(xnode::value_of(3));
	arr1.push_back(xnode::value_of(5));

	xobject obj;
	obj.put("z", xnode::value_of(12));
	obj.put("a", xnode::value_of(1L));

	xarray arr2;
	arr2.push_back(xnode::value_of(1));
	arr2.push_back(xnode::value_of(1L));
	arr2.push_back(xnode::value_of(false));

	arr1.push_back(xnode::value_of(obj));
	arr1.push_back(xnode::value_of(arr2));

	root.set_as<xarray>(arr1);

	Assert(root.is<xarray>());
	Assert(root.get_ptr<xarray>()->size() == 4);
	Assert(root.get_ptr<xarray>()->at(0).is<int>());
	Assert(root.get_ptr<xarray>()->at(1).is<int>());
	Assert(root.get_ptr<xarray>()->at(2).is<xobject>());
	Assert(root.get_ptr<xarray>()->at(3).is<xarray>());

	Assert(root.get_ptr<xarray>()->at(2).get_ptr<xobject>()->contains("z"));
}

void TestTreeOfNodesWithObject() {
	xnode root;
	xarray v;
	xnode value;

	v.push_back(xnode::value_of(3));
	v.push_back(xnode::value_of(5));

	xobject pl;
	pl.put("z", xnode::value_of(12));
	pl.put("a", xnode::value_of(1L));

	v.push_back(xnode::value_of(pl));

	root.set_as<xarray>(v);

	Assert(root.is<xarray>());
	Assert(root.get_ptr<xarray>()->size() == 3);
	Assert(root.get_ptr<xarray>()->at(0).is<int>());
	Assert(root.get_ptr<xarray>()->at(1).is<int>());
	Assert(root.get_ptr<xarray>()->at(2).is<xobject>());

	Assert(root.get_ptr<xarray>()->at(2).get_ptr<xobject>()->contains("z"));
}

void TestNestedTreeStructure() {
    // Create a complex nested structure with arrays and objects
    xnode root;
    xobject mainObj;
    
    // Add some primitive values
    mainObj.put("name", xnode::value_of(std::string("test-tree")));
    mainObj.put("version", xnode::value_of(2.5));
    
    // Add a nested object
    xobject metadata;
    metadata.put("author", xnode::value_of(std::string("Piotr Likus")));
    metadata.put("created", xnode::value_of(std::string("2025-05-13")));
    mainObj.put("metadata", xnode::value_of(metadata));
    
    // Add a nested array
    xarray tags;
    tags.push_back(xnode::value_of(std::string("test")));
    tags.push_back(xnode::value_of(std::string("tree")));
    tags.push_back(xnode::value_of(std::string("xnode")));
    mainObj.put("tags", xnode::value_of(tags));
    
    // Add object with nested array with objects
    xarray items;
    for (int i = 0; i < 3; i++) {
        xobject item;
        item.put("id", xnode::value_of(i));
        item.put("value", xnode::value_of(i * 10));
        items.push_back(xnode::value_of(item));
    }
    mainObj.put("items", xnode::value_of(items));
    
    root.set_as<xobject>(mainObj);
    
    // Validate the structure
    Assert(root.is<xobject>());
    Assert(root.get_ptr<xobject>()->contains("name"));
    Assert(root.get_ptr<xobject>()->get("name").is<std::string>());
    Assert(root.get_ptr<xobject>()->get("name").get_as<std::string>() == "test-tree");
    
    // Check metadata object
    Assert(root.get_ptr<xobject>()->contains("metadata"));
    Assert(root.get_ptr<xobject>()->get("metadata").is<xobject>());
    Assert(root.get_ptr<xobject>()->get("metadata").get_ptr<xobject>()->contains("author"));
    
    // Check tags array
    Assert(root.get_ptr<xobject>()->contains("tags"));
    Assert(root.get_ptr<xobject>()->get("tags").is<xarray>());
    Assert(root.get_ptr<xobject>()->get("tags").get_ptr<xarray>()->size() == 3);
    
    // Check items array with nested objects
    Assert(root.get_ptr<xobject>()->contains("items"));
    Assert(root.get_ptr<xobject>()->get("items").is<xarray>());
    Assert(root.get_ptr<xobject>()->get("items").get_ptr<xarray>()->size() == 3);
    Assert(root.get_ptr<xobject>()->get("items").get_ptr<xarray>()->at(1).is<xobject>());
    Assert(root.get_ptr<xobject>()->get("items").get_ptr<xarray>()->at(1).get_ptr<xobject>()->get("id").get_as<int>() == 1);
    Assert(root.get_ptr<xobject>()->get("items").get_ptr<xarray>()->at(1).get_ptr<xobject>()->get("value").get_as<int>() == 10);
}

void TestTreeOfNodes() {
	xnode root;
	xarray array1;
	xnode value;

	array1.push_back(xnode::value_of(3));
	array1.push_back(xnode::value_of(5));

	xobject obj;
	obj.put("z", xnode::value_of(12));
	obj.put("a", xnode::value_of(1L));

	xarray array2;
	array2.push_back(xnode::value_of(1));
	array2.push_back(xnode::value_of(1L));
	array2.push_back(xnode::value_of(false));

	array1.push_back(xnode::value_of(obj));
	array1.push_back(xnode::value_of(array2));

	root.set_as(array1);

	Assert(root.is<xarray>());
	Assert(root.get_ptr<xarray>()->size() == 4);
	Assert(root.get_ptr<xarray>()->at(0).is<int>());
	Assert(root.get_ptr<xarray>()->at(1).is<int>());
	Assert(root.get_ptr<xarray>()->at(2).is<xobject>());
	Assert(root.get_ptr<xarray>()->at(3).is<xarray>());

	Assert(root.get_ptr<xarray>()->at(2).get_ptr<xobject>()->contains("z"));
}

int xnode_tree_test() {
	TEST_PROLOG();
	TEST_FUNC(TreeOfNodesWithArray);
	TEST_FUNC(TreeOfNodesWithObject);
	TEST_FUNC(NestedTreeStructure);
    TEST_FUNC(TreeOfNodes);
	TEST_EPILOG();
}

int main()
{
	return xnode_tree_test();
}
