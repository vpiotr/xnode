//----------------------------------------------------------------------------------
// Name:        xnode_type_ext.h
// Purpose:     utility base classes & definitions required for type extensions
// Author:      Piotr Likus
// Created:     01/09/2015
// Last change: 12/09/2015
// License:     BSD
//----------------------------------------------------------------------------------

#ifndef __XNODE_TYPEE_EXT_H__
#define __XNODE_TYPEE_EXT_H__
#include <string>
#include "xnode_utils.h"

/// \file xnode_type_ext.h
/// utility base classes & definitions required for type extensions
struct xnode_def_cast_policy {
};

struct xnode_def_value_policy {
	typedef xnode_def_cast_policy cast_policy; // caster selection policy
};

template<bool selector>
struct xnode_cast_policy_silent_cast {
    enum {
        null_code = 0
    };
};

template<>
struct xnode_cast_policy_silent_cast<false> {
    enum {
        null_code = -1
    };
};

template<typename T>
struct xnode_cast_policy_type_codes {
    enum {
        null_code = xnode_cast_policy_silent_cast<false>::null_code,
        def_code = 0,
        max_code = 0
    };
};

template<typename T>
struct xnode_type_code {
	enum { value = 0 };
};

class xnode_caster_base {
protected:
	template<typename DestType>
	static void throwWrongCast(int typeCode) {
		throw std::runtime_error(std::string("Conversion failed, source type: ") + to_string(typeCode) + ", target: " + typeid(DestType).name());
	}
};

template<typename ValueType, typename CastPolicy>
class xnode_caster {
public:

	static bool cast_to_value(ValueType &output, void *storage, int srcTypeCode) {
		return false;
	}

	static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
		return false;
	}
};


#endif
