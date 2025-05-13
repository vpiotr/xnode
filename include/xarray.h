//----------------------------------------------------------------------------------
// Name:        xarray.h
// Purpose:     Definition of vector of xnodes.
// Author:      Piotr Likus
// Created:     01/09/2015
// Last change: 
// License:     BSD
//----------------------------------------------------------------------------------

#ifndef __XNODE_ARRAY_H__
#define __XNODE_ARRAY_H__

#include <vector>
#include "xnode.h"

typedef std::vector<xnode> xarray;

template<>
struct xnode_type_code<xarray> {
    enum { value = 16 }; // Ensure this doesn't conflict with other type codes
};

#endif

