//----------------------------------------------------------------------------------
// Name:        xnode_long_double.h
// Purpose:     Example how to define own casting basing on existing.
// Author:      Piotr Likus
// Created:     03/09/2015
// Last change: 
// License:     BSD
//----------------------------------------------------------------------------------

#ifndef __XNODE_LONG_DOUBLE_H__
#define __XNODE_LONG_DOUBLE_H__

#include "xnode.h"

struct xnode_ld_cast_policy {

};

typedef xnode_def_cast_policy xnode_ld_value_policy_base;

struct xnode_ld_value_policy : xnode_ld_value_policy_base {
	typedef xnode_ld_cast_policy cast_policy;
};


#include "details/xnode_builtins.h"

#endif
