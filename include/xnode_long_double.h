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
#include <typeinfo>

// Define a specific type code for long double to distinguish it
template<>
struct xnode_type_code<long double> {
    enum { value = 17 }; // Ensure this doesn't conflict with other type codes
};

struct xnode_ld_cast_policy {

};

typedef xnode_def_cast_policy xnode_ld_value_policy_base;

struct xnode_ld_value_policy : xnode_ld_value_policy_base {
    typedef xnode_ld_cast_policy cast_policy;
};

// Specialized caster for double to handle long double conversion
template<>
class xnode_caster<double, xnode_ld_cast_policy> : xnode_caster_base {
public:
    typedef double ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        // Special handling for long double
        if (srcTypeCode == xnode_type_code<long double>::value) {
            long double value;

            if (xnode_storage_meta<long double>::storage_type == xstOwned) {
                value = *reinterpret_cast<long double*>(*storage);
            } else {
                value = xnode_get_scalar<long double>(storage);
            }

            output = static_cast<ValueType>(value);
            return true;
        }
        
        // For all other types, use default policy
        return xnode_caster<double, xnode_def_cast_policy>::cast_to_value(output, storage, srcTypeCode);
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        // Special handling for long double
        if (destTypeCode == xnode_type_code<long double>::value) {
            if (xnode_storage_meta<long double>::storage_type == xstOwned) {
              *reinterpret_cast<long double*>(*storage) = static_cast<long double>(value);
            } else {
              xnode_set_scalar<long double>(storage, static_cast<long double>(value));
            }
            return true;
        }

        // For all other types, use default policy
        return xnode_caster<double, xnode_def_cast_policy>::cast_from_value(storage, destTypeCode, value);
    }
};

// Specialized caster for long double 
template<>
class xnode_caster<long double, xnode_ld_cast_policy> : xnode_caster_base {
public:
    typedef long double ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

        switch (srcTypeCode) {
            case xnode_type_code<double>::value: {
                output = xnode_get_scalar<double>(storage);
                break;
            }
            case xnode_type_code<float>::value: {
                output = xnode_get_scalar<float>(storage);
                break;
            }
            case xnode_type_code<long double>::value: {
                output = xnode_get_scalar<long double>(storage);
                break;
            }
            case xnode_type_code<int>::value: {
                output = xnode_get_scalar<int>(storage);
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
            case xnode_type_code<double>::value: {
                *reinterpret_cast<double*>(*storage) = static_cast<double>(value);
                break;
            }
            case xnode_type_code<float>::value: {
                *reinterpret_cast<float*>(*storage) = static_cast<float>(value);
                break;
            }
            case xnode_type_code<long double>::value: {
                *reinterpret_cast<long double*>(*storage) = value;
                break;
            }
            default: {
                result = false;
            }
        }
        return result;
    }
};

#include "details/xnode_builtins.h"

#endif
