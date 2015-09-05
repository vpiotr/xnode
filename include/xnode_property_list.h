//----------------------------------------------------------------------------------
// Name:        xnode_property_list.h
// Purpose:     Definition of property list of xnodes.
// Author:      Piotr Likus
// Created:     01/09/2015
// Last change: 
// License:     BSD
//----------------------------------------------------------------------------------

#ifndef __XNODE_PROP_LIST_H__
#define __XNODE_PROP_LIST_H__

#include "property_list.h"
template<typename KeyType> using xnode_property_list = property_list<KeyType, xnode>;
typedef xnode_property_list<std::string> xnode_named_list;

#endif 