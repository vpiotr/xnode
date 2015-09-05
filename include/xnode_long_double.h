//----------------------------------------------------------------------------------
// Name:        xnode_long_double.h
// Purpose:     Example how to define own casting basing on existing.
// Author:      Piotr Likus
// Created:     03/09/2015
// Last change: 
// License:     BSD
//----------------------------------------------------------------------------------

#ifndef __XN_LONG_DOUBLE_H__
#define __XN_LONG_DOUBLE_H__

#include <string>
#include "xnode_type_ext.h"
#include "details\xnode_builtins.h"

/// \file xnode_long_double.h
/// Defines conversion methods for "long double" type.
/// Can be used as a template for own conversion tables basing on existing ones.

struct xnode_ld_cast_policy {
    typedef xnode_def_cast_policy base_policy;
};

struct xnode_ld_value_policy {
	typedef xnode_ld_cast_policy cast_policy; // caster selection policy
    typedef xnode_def_value_policy base_policy;
};

template<>
struct xnode_type_code<long double> {
	enum { 
        value = xnode_cast_policy_type_codes<xnode_ld_value_policy::base_policy>::max_code + 1 
    };
};

template<>
struct xnode_cast_policy_type_codes<xnode_ld_value_policy> {
    enum { 
        null_code = xnode_cast_policy_silent_cast<false>::null_code,
        def_code = 0,
        max_code = xnode_type_code<long double>::value
    };
};


template<>
class xnode_caster<long double, xnode_ld_cast_policy> : xnode_caster_base {
public:
    typedef long double ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

        switch (srcTypeCode) {
        case xnode_type_code<bool>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<bool>(storage));
            break;
        }
        case xnode_type_code<float>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<float>(storage));
            break;
        }
        case xnode_type_code<double>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<double>(storage));
            break;
        }
        case xnode_type_code<long double>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<int>(storage));
            break;
        }
        case xnode_type_code<long>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<long>(storage));
            break;
        }
        case xnode_type_code<char>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<char>(storage));
            break;
        }
        case xnode_type_code<short>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<short>(storage));
            break;
        }
        case xnode_type_code<long long>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<long long>(storage));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned int>(storage));
            break;
        }
        case xnode_type_code<unsigned long>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned char>(storage));
            break;
        }
        case xnode_type_code<unsigned short>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned short>(storage));
            break;
        }
        case xnode_type_code<unsigned long long>::value: {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value: {
            output = from_string<ValueType>(*xnode_get_ptr<std::string>(storage));
            break;
        }
        case xnode_type_code<xnode_null_value>::value: {
            output = ValueType();
            break;
        }
        default: {
            result = false;
        }
        }
        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        bool result = true;

        switch (destTypeCode) {
        case xnode_type_code<bool>::value: {
            xnode_set_scalar<bool>(storage, (value != 0.0f));
            break;
        }
        case xnode_type_code<float>::value: {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value: {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
        case xnode_type_code<long double>::value: {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value: {
            xnode_set_scalar<int>(storage, static_cast<int>(value));
            break;
        }
        case xnode_type_code<long>::value: {
            xnode_set_scalar<long>(storage, static_cast<long>(value));
            break;
        }
        case xnode_type_code<char>::value: {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value: {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value: {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value: {
            xnode_set_scalar<unsigned int>(storage, static_cast<unsigned int>(value));
            break;
        }
        case xnode_type_code<unsigned long>::value: {
            xnode_set_scalar<unsigned long>(storage, static_cast<unsigned long>(value));
            break;
        }
        case xnode_type_code<unsigned char>::value: {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value: {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value: {
            xnode_set_scalar<unsigned long long>(storage, static_cast<unsigned long long>(value));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value: {
            *static_cast<std::string *>(*storage) = to_string(value);
            break;
        }
        case xnode_type_code<xnode_null_value>::value: {
            // empty - does nothing
            break;
        }
        default: {
            result = false;
        }
        }

        return result;
    }
};

template<>
class xnode_caster<std::string, xnode_ld_cast_policy> : xnode_caster_base {
public:
	typedef std::string ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

	static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
                output = to_string(
                    xnode_get_scalar<long double>(storage));
                return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
	}

	static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
                xnode_set_scalar<long double>(storage, from_string<long double>(value));
                return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
	}
};

// ----------------------------------------------------------------
// -- Floating point casters
// ----------------------------------------------------------------
template<>
class xnode_caster<float, xnode_ld_cast_policy> : xnode_caster_base {
public:
    typedef float ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
    }
};

template<>
class xnode_caster<double, xnode_ld_cast_policy> : xnode_caster_base {
public:
    typedef double ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
    }
};

// ----------------------------------------------------------------
// -- INTEGERS
// ----------------------------------------------------------------
template<>
class xnode_caster<char, xnode_ld_cast_policy> : xnode_caster_base {
public:
    typedef char ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            return true;
        }
        else {
           return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
    }

};

template<>
class xnode_caster<short, xnode_ld_cast_policy> : xnode_caster_base {
public:
    typedef short ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
    }
};

template<>
class xnode_caster<int, xnode_ld_cast_policy> : xnode_caster_base {
public:
	typedef int ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
    }
};

template<>
class xnode_caster<long, xnode_ld_cast_policy> : xnode_caster_base {
public:
	typedef long ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
    }
};

template<>
class xnode_caster<long long, xnode_ld_cast_policy> : xnode_caster_base {
public:
	typedef long long ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
    }
};

// ----------------------------------------------------------------
// -- UNSIGNED INTEGERS
// ----------------------------------------------------------------
template<>
class xnode_caster<unsigned char, xnode_ld_cast_policy> : xnode_caster_base {
public:
    typedef unsigned char ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
    }
};

template<>
class xnode_caster<unsigned short, xnode_ld_cast_policy> : xnode_caster_base {
public:
    typedef unsigned short ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
    }
};

template<>
class xnode_caster<unsigned int, xnode_ld_cast_policy> : xnode_caster_base {
public:
	typedef unsigned int ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
    }
};

template<>
class xnode_caster<unsigned long, xnode_ld_cast_policy> : xnode_caster_base {
public:
	typedef unsigned long ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
    }
};

template<>
class xnode_caster<unsigned long long, xnode_ld_cast_policy> : xnode_caster_base {
public:
	typedef unsigned long long ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
    }
};

// -----------------------------------------------
// --- END OF UNSIGNED 
// -----------------------------------------------
template<>
class xnode_caster<bool, xnode_ld_cast_policy> : xnode_caster_base {
public:
	typedef bool ValueType;
    typedef xnode_ld_cast_policy CastPolicy;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        if (srcTypeCode == xnode_type_code<long double>::value) {
            output = static_cast<ValueType>(xnode_get_scalar<long double>(storage) != 0.0);
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_to_value(output, storage, srcTypeCode);
        }
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        if (destTypeCode == xnode_type_code<long double>::value) {
            xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            return true;
        }
        else {
            return xnode_caster<ValueType, CastPolicy::base_policy>::cast_from_value(storage, destTypeCode, value);
        }
    }
};

#endif
