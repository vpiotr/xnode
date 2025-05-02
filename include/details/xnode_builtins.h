//----------------------------------------------------------------------------------
// Name:        xnode_builtins.h
// Purpose:     Conversion support for built-in types.
// Author:      Piotr Likus
// Created:     01/09/2015
// Last change: 
// License:     BSD
//----------------------------------------------------------------------------------

#ifndef __XNODE_BUILTINS_H__
#define __XNODE_BUILTINS_H__

#include <string>
#include <cmath>
#include "xnode_type_ext.h"

/// \file xnode_builtins.h
/// Conversion support for built-in types.
///
/// Supported types here:
/// - bool
/// - float, double
/// - char, short, int, long, long long
/// - unsigned: char, short, int, long, long long
/// - std::string

template<>
struct xnode_cast_policy_type_codes<xnode_def_value_policy> {
    enum { 
        null_code = xnode_cast_policy_silent_cast<false>::null_code,
        def_code = 0,
        max_code = 14
    };
};

template<>
struct xnode_type_code<xnode_null_value> {
	enum { value = xnode_cast_policy_type_codes<xnode_def_value_policy>::null_code };
};

template<>
struct xnode_type_code<bool> {
	enum { value = 1 };
};

template<>
struct xnode_type_code<float> {
	enum { value = 2 };
};

template<>
struct xnode_type_code<double> {
	enum { value = 3 };
};

template<>
struct xnode_type_code<std::string> {
	enum { value = 4 };
};

template<>
struct xnode_type_code<char> {
	enum { value = 5 };
};

template<>
struct xnode_type_code<short> {
	enum { value = 6 };
};

template<>
struct xnode_type_code<int> {
	enum { value = 7 };
};

template<>
struct xnode_type_code<long> {
	enum { value = 8 };
};

template<>
struct xnode_type_code<long long> {
	enum { value = 9 };
};

template<>
struct xnode_type_code<unsigned char> {
	enum { value = 10 };
};

template<>
struct xnode_type_code<unsigned short> {
	enum { value = 11 };
};

template<>
struct xnode_type_code<unsigned int> {
	enum { value = 12 };
};

template<>
struct xnode_type_code<unsigned long> {
	enum { value = 13 };
};

template<>
struct xnode_type_code<unsigned long long> {
	enum { value = 14 };
};

template<bool Parameter>
struct xnode_bool_to_str {
	static const char *get_value() {
		static const char value[] = "false";
		return value;
	}
};

template<>
struct xnode_bool_to_str<true> {
	static const char *get_value() {
		static const char value[] = "true";
		return value;
	}
};

template<typename T>
static bool extract_char_from_string(T &output, const std::string &str) {
    if (str.empty() || str.size() > 1) {
        return false;
    }
    output = static_cast<T>(str[0]);
    return true;
}
            

template<>
class xnode_caster<std::string, xnode_def_cast_policy> : xnode_caster_base {
public:
	typedef std::string ValueType;

	static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

		switch (srcTypeCode) {
			case xnode_type_code<bool>::value: {
				output = 
                    xnode_get_scalar<bool>(storage) ?
                      xnode_bool_to_str<true>::get_value() : 
                      xnode_bool_to_str<false>::get_value();
                break;
			}
            case xnode_type_code<float>::value: {
                output = to_string(
                    xnode_get_scalar<float>(storage));
                break;
            }
            case xnode_type_code<double>::value: {
                output = to_string(
                    xnode_get_scalar<double>(storage));
                break;
            }
            // -- signed integers --
            case xnode_type_code<int>::value: {
				output = to_string(
                    xnode_get_scalar<int>(storage));
                break;
			}
			case xnode_type_code<long>::value: {
                output = to_string(
                    xnode_get_scalar<long>(storage));
                break;
			}
            case xnode_type_code<char>::value: {
                output = xnode_get_scalar<char>(storage);
                break;
            }
            case xnode_type_code<short>::value: {
                output = to_string(
                    xnode_get_scalar<short>(storage));
                break;
            }
            case xnode_type_code<long long>::value: {
                output = to_string(
                    xnode_get_scalar<long long>(storage));
                break;
            }
            // -- unsigned integers --
            case xnode_type_code<unsigned int>::value: {
                output = to_string(
                    xnode_get_scalar<unsigned int>(storage));
                break;
            }
            case xnode_type_code<unsigned long>::value: {
                output = to_string(
                    xnode_get_scalar<unsigned long>(storage));
                break;
            }
            case xnode_type_code<unsigned char>::value: {
                output = xnode_get_scalar<unsigned char>(storage);
                break;
            }
            case xnode_type_code<unsigned short>::value: {
                output = to_string(
                    xnode_get_scalar<unsigned short>(storage));
                break;
            }
            case xnode_type_code<unsigned long long>::value: {
                output = to_string(
                    xnode_get_scalar<unsigned long long>(storage));
                break;
            }
            // -- rest --
            case xnode_type_code<std::string>::value: {
				output = *xnode_get_ptr<std::string>(storage);
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
                bool bvalue = (value == xnode_bool_to_str<true>::get_value());
                if (!bvalue)
                    bvalue = (value == "1");
                xnode_set_scalar<bool>(storage, bvalue);
                break;
            }
            case xnode_type_code<float>::value: {
                result = read_string<float>(storage, value);
                break;
            }
            case xnode_type_code<double>::value: {
                result = read_string<double>(storage, value);
                break;
            }
            // -- signed integers --
            case xnode_type_code<int>::value: {
                result = read_string<int>(storage, value);
				break;
			}
			case xnode_type_code<long>::value: {
                result = read_string<long>(storage, value);
				break;
			}
            case xnode_type_code<char>::value: {
                xnode_set_scalar<char>(storage, value.empty()?'\0':value.at(0));
                break;
            }
            case xnode_type_code<short>::value: {
                result = read_string<short>(storage, value);
                break;
            }
            case xnode_type_code<long long>::value: {
                result = read_string<long long>(storage, value);
                break;
            }
            // -- unsigned integers --
            case xnode_type_code<unsigned int>::value: {
                result = read_string<unsigned int>(storage, value);
                break;
            }
            case xnode_type_code<unsigned long>::value: {
                result = read_string<unsigned long>(storage, value);
                break;
            }
            case xnode_type_code<unsigned char>::value: {
                xnode_set_scalar<unsigned char>(storage, value.empty() ? '\0' : value.at(0));
                break;
            }
            case xnode_type_code<unsigned short>::value: {
                result = read_string<unsigned short>(storage, value);
                break;
            }
            case xnode_type_code<unsigned long long>::value: {
                result = read_string<unsigned long long>(storage, value);
                break;
            }
            // -- rest --
            case xnode_type_code<std::string>::value: {
                *xnode_get_ptr<std::string>(storage) = value;
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

protected:

    template<typename T>
    static bool read_string(void **storage, const ValueType &src) {
        bool result;

        T nativeValue;
        result = from_string<T>(nativeValue, src);

        if (result) {
            xnode_set_scalar<T>(storage, nativeValue);
        }

        return result;      
    }

};

// ----------------------------------------------------------------
// -- Floating point casters
// ----------------------------------------------------------------
template<>
class xnode_caster<float, xnode_def_cast_policy> : xnode_caster_base {
public:
    typedef float ValueType;

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
            result = from_string<ValueType>(output, *xnode_get_ptr<std::string>(storage));
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
class xnode_caster<double, xnode_def_cast_policy> : xnode_caster_base {
public:
    typedef double ValueType;

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
            result = from_string<ValueType>(output, *xnode_get_ptr<std::string>(storage));
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

// ----------------------------------------------------------------
// -- INTEGERS
// ----------------------------------------------------------------
template<>
class xnode_caster<char, xnode_def_cast_policy> : xnode_caster_base {
public:
    typedef char ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

        switch (srcTypeCode) {
        case xnode_type_code<bool>::value: {
            output = xnode_get_scalar<bool>(storage);
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
            auto str = *xnode_get_ptr<std::string>(storage);
            result = extract_char_from_string(output, str);
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
            xnode_set_scalar<bool>(storage, (value != 0));
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
            xnode_set_scalar<unsigned int>(storage, value);
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
class xnode_caster<short, xnode_def_cast_policy> : xnode_caster_base {
public:
    typedef short ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

        switch (srcTypeCode) {
        case xnode_type_code<bool>::value: {
            output = xnode_get_scalar<bool>(storage);
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
            result = from_string<ValueType>(output, *xnode_get_ptr<std::string>(storage));
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
            xnode_set_scalar<bool>(storage, (value != 0));
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
        // -- signed integers --
        case xnode_type_code<int>::value: {
            xnode_set_scalar<int>(storage, value);
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
            xnode_set_scalar<unsigned int>(storage, value);
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
class xnode_caster<int, xnode_def_cast_policy> : xnode_caster_base {
public:
	typedef int ValueType;

	static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

        switch (srcTypeCode) {
            case xnode_type_code<bool>::value: {
                output = xnode_get_scalar<bool>(storage);
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
            // -- signed integers --
            case xnode_type_code<int>::value: {
                output = xnode_get_scalar<int>(storage);
                break;
			}
			case xnode_type_code<long>::value: {
                output = xnode_get_scalar<long>(storage);
                break;
			}
            case xnode_type_code<char>::value: {
                output = xnode_get_scalar<char>(storage);
                break;
            }
            case xnode_type_code<short>::value: {
                output = xnode_get_scalar<short>(storage);
                break;
            }
            case xnode_type_code<long long>::value: {
                output = static_cast<ValueType>(xnode_get_scalar<long long>(storage));
                break;
            }
            // -- unsigned integers --
            case xnode_type_code<unsigned int>::value: {
                output = xnode_get_scalar<unsigned int>(storage);
                break;
            }
            case xnode_type_code<unsigned long>::value: {
                output = xnode_get_scalar<unsigned long>(storage);
                break;
            }
            case xnode_type_code<unsigned char>::value: {
                output = xnode_get_scalar<unsigned char>(storage);
                break;
            }
            case xnode_type_code<unsigned short>::value: {
                output = xnode_get_scalar<unsigned short>(storage);
                break;
            }
            case xnode_type_code<unsigned long long>::value: {
                output = static_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
                break;
            }
            // -- rest --
            case xnode_type_code<std::string>::value: {
                result = from_string<ValueType>(output, *xnode_get_ptr<std::string>(storage));
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
                xnode_set_scalar<bool>(storage, (value != 0));
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
            // -- signed integers --
            case xnode_type_code<int>::value: {
				xnode_set_scalar<int>(storage, value);
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
                xnode_set_scalar<unsigned int>(storage, value);
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
class xnode_caster<long, xnode_def_cast_policy> : xnode_caster_base {
public:
	typedef long ValueType;

	static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

        switch (srcTypeCode) {
            case xnode_type_code<bool>::value: {
                output = xnode_get_scalar<bool>(storage);
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
                output = static_cast<ValueType>(static_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage)));
                break;
            }
            case xnode_type_code<std::string>::value: {
                result = from_string<ValueType>(output, *xnode_get_ptr<std::string>(storage));
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
                xnode_set_scalar<bool>(storage, (value != 0));
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
            // -- signed integers --
            case xnode_type_code<int>::value: {
				xnode_set_scalar<int>(storage, value);
				break;
			}
			case xnode_type_code<long>::value: {
				xnode_set_scalar<long>(storage, value);
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
                xnode_set_scalar<unsigned int>(storage, value);
                break;
            }
            case xnode_type_code<unsigned long>::value: {
                xnode_set_scalar<unsigned long>(storage, value);
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
class xnode_caster<long long, xnode_def_cast_policy> : xnode_caster_base {
public:
	typedef long long ValueType;

	static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

        switch (srcTypeCode) {
            case xnode_type_code<bool>::value: {
                output = xnode_get_scalar<bool>(storage);
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
            // -- signed integers --
            case xnode_type_code<int>::value: {
				output = xnode_get_scalar<int>(storage);
				break;
			}
			case xnode_type_code<long>::value: {
				output = xnode_get_scalar<long>(storage);
				break;
			}
            case xnode_type_code<char>::value: {
                output = xnode_get_scalar<char>(storage);
                break;
            }
            case xnode_type_code<short>::value: {
                output = xnode_get_scalar<short>(storage);
                break;
            }
            case xnode_type_code<long long>::value: {
                output = static_cast<ValueType>(xnode_get_scalar<long long>(storage));
                break;
            }                                                    
            // -- unsigned integers --
            case xnode_type_code<unsigned int>::value: {
                output = xnode_get_scalar<unsigned int>(storage);
                break;
            }
            case xnode_type_code<unsigned long>::value: {
                output = static_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
                break;
            }
            case xnode_type_code<unsigned char>::value: {
                output = xnode_get_scalar<unsigned char>(storage);
                break;
            }
            case xnode_type_code<unsigned short>::value: {
                output = xnode_get_scalar<unsigned short>(storage);
                break;
            }
            case xnode_type_code<unsigned long long>::value: {
                output = static_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
                break;
            }
            case xnode_type_code<std::string>::value: {
                result = from_string<ValueType>(output, *xnode_get_ptr<std::string>(storage));
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
                xnode_set_scalar<bool>(storage, (value != 0));
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

// ----------------------------------------------------------------
// -- UNSIGNED INTEGERS
// ----------------------------------------------------------------
template<>
class xnode_caster<unsigned char, xnode_def_cast_policy> : xnode_caster_base {
public:
    typedef unsigned char ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

        switch (srcTypeCode) {
        case xnode_type_code<bool>::value: {
            output = xnode_get_scalar<bool>(storage);
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
            auto str = *xnode_get_ptr<std::string>(storage);
            result = extract_char_from_string(output, str);
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
            xnode_set_scalar<bool>(storage, (value != 0));
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
            xnode_set_scalar<unsigned int>(storage, value);
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
class xnode_caster<unsigned short, xnode_def_cast_policy> : xnode_caster_base {
public:
    typedef unsigned short ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

        switch (srcTypeCode) {
        case xnode_type_code<bool>::value: {
            output = xnode_get_scalar<bool>(storage);
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
            result = from_string<ValueType>(output, *xnode_get_ptr<std::string>(storage));
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
            xnode_set_scalar<bool>(storage, (value != 0));
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
        // -- signed integers --
        case xnode_type_code<int>::value: {
            xnode_set_scalar<int>(storage, value);
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
            xnode_set_scalar<unsigned int>(storage, value);
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
class xnode_caster<unsigned int, xnode_def_cast_policy> : xnode_caster_base {
public:
	typedef unsigned int ValueType;

	static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

        switch (srcTypeCode) {
            case xnode_type_code<bool>::value: {
                output = xnode_get_scalar<bool>(storage);
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
            // -- signed integers --
            case xnode_type_code<int>::value: {
                output = xnode_get_scalar<int>(storage);
                break;
			}
			case xnode_type_code<long>::value: {
                output = xnode_get_scalar<long>(storage);
                break;
			}
            case xnode_type_code<char>::value: {
                output = xnode_get_scalar<char>(storage);
                break;
            }
            case xnode_type_code<short>::value: {
                output = xnode_get_scalar<short>(storage);
                break;
            }
            case xnode_type_code<long long>::value: {
                output = static_cast<ValueType>(xnode_get_scalar<long long>(storage));
                break;
            }
            // -- unsigned integers --
            case xnode_type_code<unsigned int>::value: {
                output = xnode_get_scalar<unsigned int>(storage);
                break;
            }
            case xnode_type_code<unsigned long>::value: {
                output = xnode_get_scalar<unsigned long>(storage);
                break;
            }
            case xnode_type_code<unsigned char>::value: {
                output = xnode_get_scalar<unsigned char>(storage);
                break;
            }
            case xnode_type_code<unsigned short>::value: {
                output = xnode_get_scalar<unsigned short>(storage);
                break;
            }
            case xnode_type_code<unsigned long long>::value: {
                output = static_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
                break;
            }
            // -- rest --
            case xnode_type_code<std::string>::value: {
                result = from_string<ValueType>(output, *xnode_get_ptr<std::string>(storage));
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
                xnode_set_scalar<bool>(storage, (value != 0));
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
            // -- signed integers --
            case xnode_type_code<int>::value: {
				xnode_set_scalar<int>(storage, value);
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
                xnode_set_scalar<unsigned int>(storage, value);
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
class xnode_caster<unsigned long, xnode_def_cast_policy> : xnode_caster_base {
public:
	typedef unsigned long ValueType;

	static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

        switch (srcTypeCode) {
            case xnode_type_code<bool>::value: {
                output = xnode_get_scalar<bool>(storage);
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
                output = static_cast<ValueType>(static_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage)));
                break;
            }
            case xnode_type_code<std::string>::value: {
                result = from_string<ValueType>(output, *xnode_get_ptr<std::string>(storage));
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
                xnode_set_scalar<bool>(storage, (value != 0));
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
            // -- signed integers --
            case xnode_type_code<int>::value: {
				xnode_set_scalar<int>(storage, value);
				break;
			}
			case xnode_type_code<long>::value: {
				xnode_set_scalar<long>(storage, value);
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
                xnode_set_scalar<unsigned int>(storage, value);
                break;
            }
            case xnode_type_code<unsigned long>::value: {
                xnode_set_scalar<unsigned long>(storage, value);
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
class xnode_caster<unsigned long long, xnode_def_cast_policy> : xnode_caster_base {
public:
	typedef unsigned long long ValueType;

	static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

        switch (srcTypeCode) {
            case xnode_type_code<bool>::value: {
                output = xnode_get_scalar<bool>(storage);
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
            // -- signed integers --
            case xnode_type_code<int>::value: {
				output = xnode_get_scalar<int>(storage);
				break;
			}
			case xnode_type_code<long>::value: {
				output = xnode_get_scalar<long>(storage);
				break;
			}
            case xnode_type_code<char>::value: {
                output = xnode_get_scalar<char>(storage);
                break;
            }
            case xnode_type_code<short>::value: {
                output = xnode_get_scalar<short>(storage);
                break;
            }
            case xnode_type_code<long long>::value: {
                output = static_cast<ValueType>(xnode_get_scalar<long long>(storage));
                break;
            }                                                    
            // -- unsigned integers --
            case xnode_type_code<unsigned int>::value: {
                output = xnode_get_scalar<unsigned int>(storage);
                break;
            }
            case xnode_type_code<unsigned long>::value: {
                output = static_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
                break;
            }
            case xnode_type_code<unsigned char>::value: {
                output = xnode_get_scalar<unsigned char>(storage);
                break;
            }
            case xnode_type_code<unsigned short>::value: {
                output = xnode_get_scalar<unsigned short>(storage);
                break;
            }
            case xnode_type_code<unsigned long long>::value: {
                output = static_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
                break;
            }
            case xnode_type_code<std::string>::value: {
                result = from_string<ValueType>(output, *xnode_get_ptr<std::string>(storage));
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
                xnode_set_scalar<bool>(storage, (value != 0));
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

// -----------------------------------------------
// --- END OF UNSIGNED 
// -----------------------------------------------
template<>
class xnode_caster<bool, xnode_def_cast_policy> : xnode_caster_base {
public:
	typedef bool ValueType;

	static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode) {
        bool result = true;

        switch (srcTypeCode) {
            case xnode_type_code<bool>::value: {
                output = xnode_get_scalar<bool>(storage);
                break;
            }
            case xnode_type_code<float>::value: {
                float value = xnode_get_scalar<float>(storage);
                if (!std::isnan(value)) {
                    output = static_cast<ValueType>(value != 0.0f);
                } else {
                    result = false;
                }
                break;
            }
            case xnode_type_code<double>::value: {
                double value = xnode_get_scalar<double>(storage);
                if (!std::isnan(value)) {
                    output = static_cast<ValueType>(value != 0.0);
                } else {
                    result = false;
                }
                break;
            }
            // -- signed integers --
            case xnode_type_code<int>::value: {
				output = (xnode_get_scalar<int>(storage) != 0);
				break;
			}
			case xnode_type_code<long>::value: {
				output = (xnode_get_scalar<long>(storage) != 0);
				break;
			}
            case xnode_type_code<char>::value: {
                output = xnode_get_scalar<char>(storage) != 0;
                break;
            }
            case xnode_type_code<short>::value: {
                output = xnode_get_scalar<short>(storage) != 0;
                break;
            }
            case xnode_type_code<long long>::value: {
                output = xnode_get_scalar<long long>(storage) != 0;
                break;
            }
            // -- unsigned integers --
            case xnode_type_code<unsigned int>::value: {
                output = (xnode_get_scalar<unsigned int>(storage) != 0);
                break;
            }
            case xnode_type_code<unsigned long>::value: {
                output = (xnode_get_scalar<unsigned long>(storage) != 0);
                break;
            }
            case xnode_type_code<unsigned char>::value: {
                output = xnode_get_scalar<unsigned char>(storage) != 0;
                break;
            }
            case xnode_type_code<unsigned short>::value: {
                output = xnode_get_scalar<unsigned short>(storage) != 0;
                break;
            }
            case xnode_type_code<unsigned long long>::value: {
                output = xnode_get_scalar<unsigned long long>(storage) != 0;
                break;
            }
            // -- rest --
            case xnode_type_code<std::string>::value: {
				std::string *pstr = xnode_get_ptr<std::string>(storage);
				bool bvalue = (*pstr == xnode_bool_to_str<true>::get_value());
				if (!bvalue)
					bvalue = (*pstr == "1");
				output = bvalue;
				break;
			}
			case xnode_type_code<xnode_null_value>::value: {
				output = ValueType();
				break;
			}
			default: {
				result = false;
				break;
			}
		}

        return result;
	}

	static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value) {
        bool result = true;

        switch (destTypeCode) {
            case xnode_type_code<bool>::value: {
                xnode_set_scalar<bool>(storage, value);
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
            // -- signed integers --
            case xnode_type_code<int>::value: {
			    xnode_set_scalar<int>(storage, value);
			    break;
		    }
		    case xnode_type_code<long>::value: {
			    xnode_set_scalar<long>(storage, value);
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
                xnode_set_scalar<unsigned int>(storage, value);
                break;
            }
            case xnode_type_code<unsigned long>::value: {
                xnode_set_scalar<unsigned long>(storage, value);
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
			    *static_cast<std::string *>(*storage) = (value? xnode_bool_to_str<true>::get_value() : xnode_bool_to_str<false>::get_value());
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



#endif 