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
#include <limits>

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

template <>
struct xnode_cast_policy_type_codes<xnode_def_value_policy>
{
    enum
    {
        null_code = xnode_cast_policy_silent_cast<false>::null_code,
        def_code = 0,
        max_code = 14
    };
};

template <>
struct xnode_type_code<xnode_null_value>
{
    enum
    {
        value = xnode_cast_policy_type_codes<xnode_def_value_policy>::null_code
    };
};

template <>
struct xnode_type_code<bool>
{
    enum
    {
        value = 1
    };
};

template <>
struct xnode_type_code<float>
{
    enum
    {
        value = 2
    };
};

template <>
struct xnode_type_code<double>
{
    enum
    {
        value = 3
    };
};

template <>
struct xnode_type_code<std::string>
{
    enum
    {
        value = 4
    };
};

template <>
struct xnode_type_code<char>
{
    enum
    {
        value = 5
    };
};

template <>
struct xnode_type_code<short>
{
    enum
    {
        value = 6
    };
};

template <>
struct xnode_type_code<int>
{
    enum
    {
        value = 7
    };
};

template <>
struct xnode_type_code<long>
{
    enum
    {
        value = 8
    };
};

template <>
struct xnode_type_code<long long>
{
    enum
    {
        value = 9
    };
};

template <>
struct xnode_type_code<unsigned char>
{
    enum
    {
        value = 10
    };
};

template <>
struct xnode_type_code<unsigned short>
{
    enum
    {
        value = 11
    };
};

template <>
struct xnode_type_code<unsigned int>
{
    enum
    {
        value = 12
    };
};

template <>
struct xnode_type_code<unsigned long>
{
    enum
    {
        value = 13
    };
};

template <>
struct xnode_type_code<unsigned long long>
{
    enum
    {
        value = 14
    };
};

template <bool Parameter>
struct xnode_bool_to_str
{
    static const char *get_value()
    {
        static const char value[] = "false";
        return value;
    }
};

template <>
struct xnode_bool_to_str<true>
{
    static const char *get_value()
    {
        static const char value[] = "true";
        return value;
    }
};

template <typename T>
static bool extract_char_from_string(T &output, const std::string &str)
{
    if (str.empty() || str.size() > 1)
    {
        return false;
    }
    output = static_cast<T>(str[0]);
    return true;
}

// Helper function to check if a value is positive, throws runtime_error if not
template <typename T>
T assert_is_positive(T value)
{
    if (value < 0)
    {
        throw std::runtime_error("Cannot convert negative value to unsigned type");
    }
    return value;
}

template <typename TargetType, typename SourceType>
TargetType ranged_cast(SourceType value)
{
    bool in_range = true;
    if (std::numeric_limits<TargetType>::is_signed != std::numeric_limits<SourceType>::is_signed)
    {
        if (std::numeric_limits<TargetType>::is_signed)
        {
            in_range = value <= std::numeric_limits<TargetType>::max();
        }
        else
        {
            in_range = value <= std::numeric_limits<TargetType>::max() && value >= 0;
        }
    }
    else if (value < std::numeric_limits<TargetType>::lowest() || value > std::numeric_limits<TargetType>::max())
    {
        in_range = false;
    }

    if (!in_range)
    {
        throw std::runtime_error("Value out of range for target type");
    }

    return static_cast<TargetType>(value);
}

template <typename T>
bool cast_string_to_number(T &output, const std::string &input)
{
    // Default implementation for most numeric types
    return from_string<T>(output, input);
}

// Specialization for float with overflow checking
template <>
bool cast_string_to_number<float>(float &output, const std::string &input)
{
    if (input.find_first_not_of("-0123456789.eE") != std::string::npos)
    {
        return from_string<float>(output, input);
    }

    try
    {
        size_t pos = 0;
        double intermediate = std::stod(input, &pos);
        if (std::abs(intermediate) > std::numeric_limits<float>::max())
        {
            throw std::overflow_error("Numeric overflow when converting string to float");
        }
        if (pos != input.length())
        {
            throw std::runtime_error("Invalid format for float conversion");
        }
    }
    catch (const std::out_of_range &)
    {
        throw std::overflow_error("Numeric overflow when converting string to float");
    }
    catch (const std::invalid_argument &)
    {
        // Let the from_string function handle invalid format
    }

    return from_string<float>(output, input);
}

// Specialization for double with overflow checking
template <>
bool cast_string_to_number<double>(double &output, const std::string &input)
{
    if (input.find_first_not_of("-0123456789.eE") != std::string::npos)
    {
        return from_string<double>(output, input);
    }

    try
    {
        size_t pos = 0;
        std::stod(input, &pos);
        if (pos != input.length())
        {
            throw std::runtime_error("Invalid format for double conversion");
        }
    }
    catch (const std::out_of_range &)
    {
        throw std::overflow_error("Numeric overflow when converting string to double");
    }
    catch (const std::invalid_argument &)
    {
        // Let the from_string function handle invalid format
    }

    return from_string<double>(output, input);
}

// Specialization for int with range checking
template <>
bool cast_string_to_number<int>(int &output, const std::string &input)
{
    if (input.find_first_not_of("-0123456789") != std::string::npos)
    {
        return from_string<int>(output, input);
    }

    try
    {
        size_t pos = 0;
        long long intermediate = std::stoll(input, &pos);
        if (intermediate > std::numeric_limits<int>::max() ||
            intermediate < std::numeric_limits<int>::min())
        {
            throw std::overflow_error("Numeric overflow when converting string to int");
        }
    }
    catch (const std::out_of_range &)
    {
        throw std::overflow_error("Numeric overflow when converting string to int");
    }
    catch (const std::invalid_argument &)
    {
        // Let the from_string function handle invalid format
    }

    return from_string<int>(output, input);
}

// Specialization for long with range checking
template <>
bool cast_string_to_number<long>(long &output, const std::string &input)
{
    if (input.find_first_not_of("-0123456789") != std::string::npos)
    {
        return from_string<long>(output, input);
    }

    try
    {
        size_t pos = 0;
        long long intermediate = std::stoll(input, &pos);
        if (sizeof(long) < sizeof(long long))
        {
            if (intermediate > static_cast<long long>(std::numeric_limits<long>::max()) ||
                intermediate < static_cast<long long>(std::numeric_limits<long>::min()))
            {
                throw std::overflow_error("Numeric overflow when converting string to long");
            }
        }
    }
    catch (const std::out_of_range &)
    {
        throw std::overflow_error("Numeric overflow when converting string to long");
    }
    catch (const std::invalid_argument &)
    {
        // Let the from_string function handle invalid format
    }

    return from_string<long>(output, input);
}

// Specialization for long long with range validation
template <>
bool cast_string_to_number<long long>(long long &output, const std::string &input)
{
    return from_string<long long>(output, input);
}

// Specialization for short with range checking
template <>
bool cast_string_to_number<short>(short &output, const std::string &input)
{
    if (input.find_first_not_of("-0123456789") != std::string::npos)
    {
        return from_string<short>(output, input);
    }

    try
    {
        size_t pos = 0;
        long long intermediate = std::stoll(input, &pos);
        if (intermediate > std::numeric_limits<short>::max() ||
            intermediate < std::numeric_limits<short>::min())
        {
            throw std::overflow_error("Numeric overflow when converting string to short");
        }
    }
    catch (const std::out_of_range &)
    {
        throw std::overflow_error("Numeric overflow when converting string to short");
    }
    catch (const std::invalid_argument &)
    {
        // Let the from_string function handle invalid format
    }

    return from_string<short>(output, input);
}

// Specialization for unsigned int with range checking and negative value detection
template <>
bool cast_string_to_number<unsigned int>(unsigned int &output, const std::string &input)
{
    if (input.find('-') != std::string::npos)
    {
        throw std::underflow_error("Cannot convert negative value to unsigned int");
    }

    if (input.find_first_not_of("0123456789") != std::string::npos)
    {
        return from_string<unsigned int>(output, input);
    }

    try
    {
        size_t pos = 0;
        unsigned long long intermediate = std::stoull(input, &pos);
        if (intermediate > std::numeric_limits<unsigned int>::max())
        {
            throw std::overflow_error("Numeric overflow when converting string to unsigned int");
        }
    }
    catch (const std::out_of_range &)
    {
        throw std::overflow_error("Numeric overflow when converting string to unsigned int");
    }
    catch (const std::invalid_argument &)
    {
        // Let the from_string function handle invalid format
    }

    return from_string<unsigned int>(output, input);
}

// Specialization for unsigned long with range checking and negative value detection
template <>
bool cast_string_to_number<unsigned long>(unsigned long &output, const std::string &input)
{
    if (input.find('-') != std::string::npos)
    {
        throw std::underflow_error("Cannot convert negative value to unsigned long");
    }

    if (input.find_first_not_of("0123456789") != std::string::npos)
    {
        return from_string<unsigned long>(output, input);
    }

    try
    {
        size_t pos = 0;
        unsigned long long intermediate = std::stoull(input, &pos);
        if (sizeof(unsigned long) < sizeof(unsigned long long) &&
            intermediate > std::numeric_limits<unsigned long>::max())
        {
            throw std::overflow_error("Numeric overflow when converting string to unsigned long");
        }
    }
    catch (const std::out_of_range &)
    {
        throw std::overflow_error("Numeric overflow when converting string to unsigned long");
    }
    catch (const std::invalid_argument &)
    {
        // Let the from_string function handle invalid format
    }

    return from_string<unsigned long>(output, input);
}

// Specialization for unsigned short with range checking and negative value detection
template <>
bool cast_string_to_number<unsigned short>(unsigned short &output, const std::string &input)
{
    if (input.find('-') != std::string::npos)
    {
        throw std::underflow_error("Cannot convert negative value to unsigned short");
    }

    if (input.find_first_not_of("0123456789") != std::string::npos)
    {
        return from_string<unsigned short>(output, input);
    }

    try
    {
        size_t pos = 0;
        unsigned long long intermediate = std::stoull(input, &pos);
        if (intermediate > std::numeric_limits<unsigned short>::max())
        {
            throw std::overflow_error("Numeric overflow when converting string to unsigned short");
        }
    }
    catch (const std::out_of_range &)
    {
        throw std::overflow_error("Numeric overflow when converting string to unsigned short");
    }
    catch (const std::invalid_argument &)
    {
        // Let the from_string function handle invalid format
    }

    return from_string<unsigned short>(output, input);
}

// Specialization for unsigned long long with range checking and negative value detection
template <>
bool cast_string_to_number<unsigned long long>(unsigned long long &output, const std::string &input)
{
    if (input.find('-') != std::string::npos)
    {
        throw std::underflow_error("Cannot convert negative value to unsigned long long");
    }

    if (input.find_first_not_of("0123456789") != std::string::npos)
    {
        return from_string<unsigned long long>(output, input);
    }

    try
    {
        size_t pos = 0;
        std::stoull(input, &pos);
        if (pos != input.length())
        {
            throw std::runtime_error("Invalid format for unsigned long long conversion");
        }
    }
    catch (const std::out_of_range &)
    {
        throw std::overflow_error("Numeric overflow when converting string to unsigned long long");
    }
    catch (const std::invalid_argument &)
    {
        // Let the from_string function handle invalid format
    }

    return from_string<unsigned long long>(output, input);
}

// Specialization for char that extracts a single character from string
template <>
bool cast_string_to_number<char>(char &output, const std::string &input)
{
    return extract_char_from_string(output, input);
}

// Specialization for unsigned char that extracts a single character from string
template <>
bool cast_string_to_number<unsigned char>(unsigned char &output, const std::string &input)
{
    return extract_char_from_string(output, input);
}

template <>
class xnode_caster<std::string, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef std::string ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output =
                xnode_get_scalar<bool>(storage) ? xnode_bool_to_str<true>::get_value() : xnode_bool_to_str<false>::get_value();
            break;
        }
        case xnode_type_code<float>::value:
        {
            output = to_string(
                xnode_get_scalar<float>(storage));
            break;
        }
        case xnode_type_code<double>::value:
        {
            output = to_string(
                xnode_get_scalar<double>(storage));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = to_string(
                xnode_get_scalar<int>(storage));
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = to_string(
                xnode_get_scalar<long>(storage));
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = xnode_get_scalar<char>(storage);
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = to_string(
                xnode_get_scalar<short>(storage));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = to_string(
                xnode_get_scalar<long long>(storage));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = to_string(
                xnode_get_scalar<unsigned int>(storage));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = to_string(
                xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = xnode_get_scalar<unsigned char>(storage);
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = to_string(
                xnode_get_scalar<unsigned short>(storage));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = to_string(
                xnode_get_scalar<unsigned long long>(storage));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            output = *xnode_get_ptr<std::string>(storage);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
        }
        }
        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            bool bvalue = (value == xnode_bool_to_str<true>::get_value());
            if (!bvalue)
                bvalue = (value == "1");
            xnode_set_scalar<bool>(storage, bvalue);
            break;
        }
        case xnode_type_code<float>::value:
        {
            result = read_string<float>(storage, value);
            break;
        }
        case xnode_type_code<double>::value:
        {
            result = read_string<double>(storage, value);
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            result = read_string<int>(storage, value);
            break;
        }
        case xnode_type_code<long>::value:
        {
            result = read_string<long>(storage, value);
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, value.empty() ? '\0' : value.at(0));
            break;
        }
        case xnode_type_code<short>::value:
        {
            result = read_string<short>(storage, value);
            break;
        }
        case xnode_type_code<long long>::value:
        {
            result = read_string<long long>(storage, value);
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            result = read_string<unsigned int>(storage, value);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            result = read_string<unsigned long>(storage, value);
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, value.empty() ? '\0' : value.at(0));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            result = read_string<unsigned short>(storage, value);
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            result = read_string<unsigned long long>(storage, value);
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *xnode_get_ptr<std::string>(storage) = value;
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }

protected:
    template <typename T>
    static bool read_string(void **storage, const ValueType &src)
    {
        bool result;

        T nativeValue;
        result = from_string<T>(nativeValue, src);

        if (result)
        {
            xnode_set_scalar<T>(storage, nativeValue);
        }

        return result;
    }
};

// ----------------------------------------------------------------
// -- Floating point casters
// ----------------------------------------------------------------
template <>
class xnode_caster<float, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef float ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<bool>(storage));
            break;
        }
        case xnode_type_code<float>::value:
        {
            output = xnode_get_scalar<float>(storage);
            break;
        }
        case xnode_type_code<double>::value:
        {
            double temp = xnode_get_scalar<double>(storage);
            if (std::isinf(temp) || std::isnan(temp))
            {
                output = static_cast<ValueType>(temp);
            }
            else
            {
                output = ranged_cast<ValueType>(temp);
            }
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<int>(storage));
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<long>(storage));
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<char>(storage));
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<short>(storage));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<long long>(storage));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned int>(storage));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned char>(storage));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned short>(storage));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            std::string inputStr = *xnode_get_ptr<std::string>(storage);
            result = cast_string_to_number<ValueType>(output, inputStr);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            xnode_set_scalar<bool>(storage, (value != 0.0f));
            break;
        }
        case xnode_type_code<float>::value:
        {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value:
        {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            xnode_set_scalar<int>(storage, static_cast<int>(value));
            break;
        }
        case xnode_type_code<long>::value:
        {
            xnode_set_scalar<long>(storage, static_cast<long>(value));
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value:
        {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            xnode_set_scalar<unsigned int>(storage, static_cast<unsigned int>(value));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            xnode_set_scalar<unsigned long>(storage, static_cast<unsigned long>(value));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            xnode_set_scalar<unsigned long long>(storage, static_cast<unsigned long long>(value));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *static_cast<std::string *>(*storage) = to_string(value);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }
};

template <>
class xnode_caster<double, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef double ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<bool>(storage));
            break;
        }
        case xnode_type_code<float>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<float>(storage));
            break;
        }
        case xnode_type_code<double>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<double>(storage));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<int>(storage));
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<long>(storage));
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<char>(storage));
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<short>(storage));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<long long>(storage));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned int>(storage));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned char>(storage));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned short>(storage));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            std::string inputStr = *xnode_get_ptr<std::string>(storage);
            result = cast_string_to_number<ValueType>(output, inputStr);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
        }
        }
        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            xnode_set_scalar<bool>(storage, (value != 0.0f));
            break;
        }
        case xnode_type_code<float>::value:
        {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value:
        {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            xnode_set_scalar<int>(storage, static_cast<int>(value));
            break;
        }
        case xnode_type_code<long>::value:
        {
            xnode_set_scalar<long>(storage, static_cast<long>(value));
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value:
        {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            xnode_set_scalar<unsigned int>(storage, static_cast<unsigned int>(value));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            xnode_set_scalar<unsigned long>(storage, static_cast<unsigned long>(value));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            xnode_set_scalar<unsigned long long>(storage, static_cast<unsigned long long>(value));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *static_cast<std::string *>(*storage) = to_string(value);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }
        return result;
    }
};

// ----------------------------------------------------------------
// -- INTEGERS
// ----------------------------------------------------------------
template <>
class xnode_caster<char, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef char ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output = xnode_get_scalar<bool>(storage);
            break;
        }
        case xnode_type_code<float>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<float>(storage));
            break;
        }
        case xnode_type_code<double>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<double>(storage));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<int>(storage));
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<long>(storage));
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = xnode_get_scalar<char>(storage);
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<short>(storage));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<long long>(storage));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned int>(storage));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned char>(storage));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned short>(storage));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            auto str = *xnode_get_ptr<std::string>(storage);
            result = extract_char_from_string(output, str);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            xnode_set_scalar<bool>(storage, (value != 0));
            break;
        }
        case xnode_type_code<float>::value:
        {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value:
        {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            xnode_set_scalar<int>(storage, static_cast<int>(value));
            break;
        }
        case xnode_type_code<long>::value:
        {
            xnode_set_scalar<long>(storage, static_cast<long>(value));
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value:
        {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            xnode_set_scalar<unsigned int>(storage, value);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            xnode_set_scalar<unsigned long>(storage, static_cast<unsigned long>(value));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            xnode_set_scalar<unsigned long long>(storage, static_cast<unsigned long long>(value));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *static_cast<std::string *>(*storage) = to_string(value);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }
};

template <>
class xnode_caster<short, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef short ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output = xnode_get_scalar<bool>(storage);
            break;
        }
        case xnode_type_code<float>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<float>(storage));
            break;
        }
        case xnode_type_code<double>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<double>(storage));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<int>(storage));
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<long>(storage));
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<char>(storage));
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = xnode_get_scalar<short>(storage);
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<long long>(storage));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned int>(storage));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned char>(storage));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned short>(storage));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            auto inputStr = *xnode_get_ptr<std::string>(storage);
            result = cast_string_to_number<ValueType>(output, inputStr);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            xnode_set_scalar<bool>(storage, (value != 0));
            break;
        }
        case xnode_type_code<float>::value:
        {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value:
        {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            xnode_set_scalar<int>(storage, value);
            break;
        }
        case xnode_type_code<long>::value:
        {
            xnode_set_scalar<long>(storage, static_cast<long>(value));
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value:
        {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            xnode_set_scalar<unsigned int>(storage, value);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            xnode_set_scalar<unsigned long>(storage, static_cast<unsigned long>(value));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            xnode_set_scalar<unsigned long long>(storage, static_cast<unsigned long long>(value));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *static_cast<std::string *>(*storage) = to_string(value);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }
};

template <>
class xnode_caster<int, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef int ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output = xnode_get_scalar<bool>(storage);
            break;
        }
        case xnode_type_code<float>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<float>(storage));
            break;
        }
        case xnode_type_code<double>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<double>(storage));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = xnode_get_scalar<int>(storage);
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<long>(storage));
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = xnode_get_scalar<char>(storage);
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = xnode_get_scalar<short>(storage);
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<long long>(storage));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned int>(storage));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = xnode_get_scalar<unsigned char>(storage);
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = xnode_get_scalar<unsigned short>(storage);
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            std::string inputStr = *xnode_get_ptr<std::string>(storage);
            result = cast_string_to_number<ValueType>(output, inputStr);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            xnode_set_scalar<bool>(storage, (value != 0));
            break;
        }
        case xnode_type_code<float>::value:
        {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value:
        {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            xnode_set_scalar<int>(storage, value);
            break;
        }
        case xnode_type_code<long>::value:
        {
            xnode_set_scalar<long>(storage, static_cast<long>(value));
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value:
        {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            xnode_set_scalar<unsigned int>(storage, value);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            xnode_set_scalar<unsigned long>(storage, static_cast<unsigned long>(value));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            xnode_set_scalar<unsigned long long>(storage, static_cast<unsigned long long>(value));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *static_cast<std::string *>(*storage) = to_string(value);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }
};

template <>
class xnode_caster<long, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef long ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output = xnode_get_scalar<bool>(storage);
            break;
        }
        case xnode_type_code<float>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<float>(storage));
            break;
        }
        case xnode_type_code<double>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<double>(storage));
            break;
        }
            // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<int>(storage));
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<long>(storage));
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<char>(storage));
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<short>(storage));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<long long>(storage));
            break;
        }
            // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned int>(storage));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned char>(storage));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned short>(storage));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
            break;
        }
        case xnode_type_code<std::string>::value:
        {
            std::string inputStr = *xnode_get_ptr<std::string>(storage);
            result = cast_string_to_number<ValueType>(output, inputStr);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            xnode_set_scalar<bool>(storage, (value != 0));
            break;
        }
        case xnode_type_code<float>::value:
        {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value:
        {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
            // -- signed integers --
        case xnode_type_code<int>::value:
        {
            xnode_set_scalar<int>(storage, value);
            break;
        }
        case xnode_type_code<long>::value:
        {
            xnode_set_scalar<long>(storage, value);
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value:
        {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
            // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            xnode_set_scalar<unsigned int>(storage, value);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            xnode_set_scalar<unsigned long>(storage, value);
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            xnode_set_scalar<unsigned long long>(storage, static_cast<unsigned long long>(value));
            break;
        }
            // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *static_cast<std::string *>(*storage) = to_string(value);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }
};

template <>
class xnode_caster<long long, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef long long ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output = xnode_get_scalar<bool>(storage);
            break;
        }
        case xnode_type_code<float>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<float>(storage));
            break;
        }
        case xnode_type_code<double>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<double>(storage));
            break;
        }
            // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = xnode_get_scalar<int>(storage);
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = xnode_get_scalar<long>(storage);
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = xnode_get_scalar<char>(storage);
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = xnode_get_scalar<short>(storage);
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<long long>(storage));
            break;
        }
            // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = xnode_get_scalar<unsigned int>(storage);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = xnode_get_scalar<unsigned char>(storage);
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = xnode_get_scalar<unsigned short>(storage);
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
            break;
        }
        case xnode_type_code<std::string>::value:
        {
            std::string inputStr = *xnode_get_ptr<std::string>(storage);
            result = cast_string_to_number<ValueType>(output, inputStr);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            xnode_set_scalar<bool>(storage, (value != 0));
            break;
        }
        case xnode_type_code<float>::value:
        {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value:
        {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
            // -- signed integers --
        case xnode_type_code<int>::value:
        {
            xnode_set_scalar<int>(storage, static_cast<int>(value));
            break;
        }
        case xnode_type_code<long>::value:
        {
            xnode_set_scalar<long>(storage, static_cast<long>(value));
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value:
        {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
            // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            xnode_set_scalar<unsigned int>(storage, static_cast<unsigned int>(value));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            xnode_set_scalar<unsigned long>(storage, static_cast<unsigned long>(value));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            xnode_set_scalar<unsigned long long>(storage, static_cast<unsigned long long>(value));
            break;
        }
            // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *static_cast<std::string *>(*storage) = to_string(value);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }
};

// ----------------------------------------------------------------
// -- UNSIGNED INTEGERS
// ----------------------------------------------------------------
template <>
class xnode_caster<unsigned char, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef unsigned char ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output = xnode_get_scalar<bool>(storage);
            break;
        }
        case xnode_type_code<float>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<float>(storage));
            break;
        }
        case xnode_type_code<double>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<double>(storage));
            break;
        }
            // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<int>(storage));
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<long>(storage));
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<char>(storage));
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<short>(storage));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<long long>(storage));
            break;
        }
            // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned int>(storage));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned char>(storage));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned short>(storage));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            auto str = *xnode_get_ptr<std::string>(storage);
            result = extract_char_from_string(output, str);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            xnode_set_scalar<bool>(storage, (value != 0));
            break;
        }
        case xnode_type_code<float>::value:
        {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value:
        {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
            // -- signed integers --
        case xnode_type_code<int>::value:
        {
            xnode_set_scalar<int>(storage, static_cast<int>(value));
            break;
        }
        case xnode_type_code<long>::value:
        {
            xnode_set_scalar<long>(storage, static_cast<long>(value));
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value:
        {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            xnode_set_scalar<unsigned int>(storage, value);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            xnode_set_scalar<unsigned long>(storage, static_cast<unsigned long>(value));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            xnode_set_scalar<unsigned long long>(storage, static_cast<unsigned long long>(value));
            break;
        }
            // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *static_cast<std::string *>(*storage) = to_string(value);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }
};

template <>
class xnode_caster<unsigned short, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef unsigned short ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output = xnode_get_scalar<bool>(storage);
            break;
        }
        case xnode_type_code<float>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<float>(storage));
            break;
        }
        case xnode_type_code<double>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<double>(storage));
            break;
        }
            // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<int>(storage));
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<long>(storage));
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<char>(storage));
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<short>(storage));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<long long>(storage));
            break;
        }
            // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned int>(storage));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned char>(storage));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned short>(storage));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
            break;
        }
            // -- rest --
        case xnode_type_code<std::string>::value:
        {
            std::string inputStr = *xnode_get_ptr<std::string>(storage);
            result = cast_string_to_number<ValueType>(output, inputStr);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            xnode_set_scalar<bool>(storage, (value != 0));
            break;
        }
        case xnode_type_code<float>::value:
        {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value:
        {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
            // -- signed integers --
        case xnode_type_code<int>::value:
        {
            xnode_set_scalar<int>(storage, value);
            break;
        }
        case xnode_type_code<long>::value:
        {
            xnode_set_scalar<long>(storage, static_cast<long>(value));
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value:
        {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
            // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            xnode_set_scalar<unsigned int>(storage, value);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            xnode_set_scalar<unsigned long>(storage, static_cast<unsigned long>(value));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            xnode_set_scalar<unsigned long long>(storage, static_cast<unsigned long long>(value));
            break;
        }
            // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *static_cast<std::string *>(*storage) = to_string(value);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }
};

template <>
class xnode_caster<unsigned int, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef unsigned int ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output = xnode_get_scalar<bool>(storage);
            break;
        }
        case xnode_type_code<float>::value:
        {
            output = ranged_cast<ValueType>(assert_is_positive(xnode_get_scalar<float>(storage)));
            break;
        }
        case xnode_type_code<double>::value:
        {
            output = ranged_cast<ValueType>(assert_is_positive(xnode_get_scalar<double>(storage)));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = assert_is_positive(xnode_get_scalar<int>(storage));
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = ranged_cast<ValueType>(assert_is_positive(xnode_get_scalar<long>(storage)));
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = static_cast<unsigned char>(xnode_get_scalar<char>(storage));
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = assert_is_positive(xnode_get_scalar<short>(storage));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = ranged_cast<ValueType>(assert_is_positive(xnode_get_scalar<long long>(storage)));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = xnode_get_scalar<unsigned int>(storage);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = xnode_get_scalar<unsigned char>(storage);
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = xnode_get_scalar<unsigned short>(storage);
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = ranged_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            std::string inputStr = *xnode_get_ptr<std::string>(storage);
            result = cast_string_to_number<ValueType>(output, inputStr);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            xnode_set_scalar<bool>(storage, (value != 0));
            break;
        }
        case xnode_type_code<float>::value:
        {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value:
        {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            xnode_set_scalar<int>(storage, value);
            break;
        }
        case xnode_type_code<long>::value:
        {
            xnode_set_scalar<long>(storage, static_cast<long>(value));
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value:
        {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            xnode_set_scalar<unsigned int>(storage, value);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            xnode_set_scalar<unsigned long>(storage, static_cast<unsigned long>(value));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            xnode_set_scalar<unsigned long long>(storage, static_cast<unsigned long long>(value));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *static_cast<std::string *>(*storage) = to_string(value);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }
};

template <>
class xnode_caster<unsigned long, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef unsigned long ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output = xnode_get_scalar<bool>(storage);
            break;
        }
        case xnode_type_code<float>::value:
        {
            output = static_cast<ValueType>(assert_is_positive(xnode_get_scalar<float>(storage)));
            break;
        }
        case xnode_type_code<double>::value:
        {
            output = static_cast<ValueType>(assert_is_positive(xnode_get_scalar<double>(storage)));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = static_cast<ValueType>(assert_is_positive(xnode_get_scalar<int>(storage)));
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = static_cast<ValueType>(assert_is_positive(xnode_get_scalar<long>(storage)));
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = static_cast<ValueType>(static_cast<unsigned char>(xnode_get_scalar<char>(storage)));
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = static_cast<ValueType>(assert_is_positive(xnode_get_scalar<short>(storage)));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = static_cast<ValueType>(assert_is_positive(xnode_get_scalar<long long>(storage)));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned int>(storage));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned char>(storage));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned short>(storage));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = static_cast<ValueType>(static_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage)));
            break;
        }
        case xnode_type_code<std::string>::value:
        {
            std::string inputStr = *xnode_get_ptr<std::string>(storage);
            result = cast_string_to_number<ValueType>(output, inputStr);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            xnode_set_scalar<bool>(storage, (value != 0));
            break;
        }
        case xnode_type_code<float>::value:
        {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value:
        {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            xnode_set_scalar<int>(storage, value);
            break;
        }
        case xnode_type_code<long>::value:
        {
            xnode_set_scalar<long>(storage, value);
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value:
        {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            xnode_set_scalar<unsigned int>(storage, value);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            xnode_set_scalar<unsigned long>(storage, value);
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            xnode_set_scalar<unsigned long long>(storage, static_cast<unsigned long long>(value));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *static_cast<std::string *>(*storage) = to_string(value);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }
};

template <>
class xnode_caster<unsigned long long, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef unsigned long long ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output = xnode_get_scalar<bool>(storage);
            break;
        }
        case xnode_type_code<float>::value:
        {
            output = static_cast<ValueType>(assert_is_positive(xnode_get_scalar<float>(storage)));
            break;
        }
        case xnode_type_code<double>::value:
        {
            output = static_cast<ValueType>(assert_is_positive(xnode_get_scalar<double>(storage)));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = assert_is_positive(xnode_get_scalar<int>(storage));
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = assert_is_positive(xnode_get_scalar<long>(storage));
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = static_cast<unsigned char>(xnode_get_scalar<char>(storage));
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = assert_is_positive(xnode_get_scalar<short>(storage));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = static_cast<ValueType>(assert_is_positive(xnode_get_scalar<long long>(storage)));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = xnode_get_scalar<unsigned int>(storage);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long>(storage));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = xnode_get_scalar<unsigned char>(storage);
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = xnode_get_scalar<unsigned short>(storage);
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = static_cast<ValueType>(xnode_get_scalar<unsigned long long>(storage));
            break;
        }
        case xnode_type_code<std::string>::value:
        {
            std::string inputStr = *xnode_get_ptr<std::string>(storage);
            result = cast_string_to_number<ValueType>(output, inputStr);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            xnode_set_scalar<bool>(storage, (value != 0));
            break;
        }
        case xnode_type_code<float>::value:
        {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value:
        {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            xnode_set_scalar<int>(storage, static_cast<int>(value));
            break;
        }
        case xnode_type_code<long>::value:
        {
            xnode_set_scalar<long>(storage, static_cast<long>(value));
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value:
        {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            xnode_set_scalar<unsigned int>(storage, static_cast<unsigned int>(value));
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            xnode_set_scalar<unsigned long>(storage, static_cast<unsigned long>(value));
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            xnode_set_scalar<unsigned long long>(storage, value);
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *static_cast<std::string *>(*storage) = to_string(value);
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }
};

// -----------------------------------------------
// --- END OF UNSIGNED
// -----------------------------------------------
template <>
class xnode_caster<bool, xnode_def_cast_policy> : xnode_caster_base
{
public:
    typedef bool ValueType;

    static bool cast_to_value(ValueType &output, void **storage, int srcTypeCode)
    {
        bool result = true;

        switch (srcTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            output = xnode_get_scalar<bool>(storage);
            break;
        }
        case xnode_type_code<float>::value:
        {
            float value = xnode_get_scalar<float>(storage);
            if (!std::isnan(value))
            {
                output = static_cast<ValueType>(value != 0.0f);
            }
            else
            {
                result = false;
            }
            break;
        }
        case xnode_type_code<double>::value:
        {
            double value = xnode_get_scalar<double>(storage);
            if (!std::isnan(value))
            {
                output = static_cast<ValueType>(value != 0.0);
            }
            else
            {
                result = false;
            }
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            output = (xnode_get_scalar<int>(storage) != 0);
            break;
        }
        case xnode_type_code<long>::value:
        {
            output = (xnode_get_scalar<long>(storage) != 0);
            break;
        }
        case xnode_type_code<char>::value:
        {
            output = xnode_get_scalar<char>(storage) != 0;
            break;
        }
        case xnode_type_code<short>::value:
        {
            output = xnode_get_scalar<short>(storage) != 0;
            break;
        }
        case xnode_type_code<long long>::value:
        {
            output = xnode_get_scalar<long long>(storage) != 0;
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            output = (xnode_get_scalar<unsigned int>(storage) != 0);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            output = (xnode_get_scalar<unsigned long>(storage) != 0);
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            output = xnode_get_scalar<unsigned char>(storage) != 0;
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            output = xnode_get_scalar<unsigned short>(storage) != 0;
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            output = xnode_get_scalar<unsigned long long>(storage) != 0;
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            std::string *pstr = xnode_get_ptr<std::string>(storage);
            bool bvalue = (*pstr == xnode_bool_to_str<true>::get_value());
            if (!bvalue)
                bvalue = (*pstr == "1");
            output = bvalue;
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            output = ValueType();
            break;
        }
        default:
        {
            result = false;
            break;
        }
        }

        return result;
    }

    static bool cast_from_value(void **storage, int destTypeCode, const ValueType &value)
    {
        bool result = true;

        switch (destTypeCode)
        {
        case xnode_type_code<bool>::value:
        {
            xnode_set_scalar<bool>(storage, value);
            break;
        }
        case xnode_type_code<float>::value:
        {
            xnode_set_scalar<float>(storage, static_cast<float>(value));
            break;
        }
        case xnode_type_code<double>::value:
        {
            xnode_set_scalar<double>(storage, static_cast<double>(value));
            break;
        }
        // -- signed integers --
        case xnode_type_code<int>::value:
        {
            xnode_set_scalar<int>(storage, value);
            break;
        }
        case xnode_type_code<long>::value:
        {
            xnode_set_scalar<long>(storage, value);
            break;
        }
        case xnode_type_code<char>::value:
        {
            xnode_set_scalar<char>(storage, static_cast<char>(value));
            break;
        }
        case xnode_type_code<short>::value:
        {
            xnode_set_scalar<short>(storage, static_cast<short>(value));
            break;
        }
        case xnode_type_code<long long>::value:
        {
            xnode_set_scalar<long long>(storage, static_cast<long long>(value));
            break;
        }
        // -- unsigned integers --
        case xnode_type_code<unsigned int>::value:
        {
            xnode_set_scalar<unsigned int>(storage, value);
            break;
        }
        case xnode_type_code<unsigned long>::value:
        {
            xnode_set_scalar<unsigned long>(storage, value);
            break;
        }
        case xnode_type_code<unsigned char>::value:
        {
            xnode_set_scalar<unsigned char>(storage, static_cast<unsigned char>(value));
            break;
        }
        case xnode_type_code<unsigned short>::value:
        {
            xnode_set_scalar<unsigned short>(storage, static_cast<unsigned short>(value));
            break;
        }
        case xnode_type_code<unsigned long long>::value:
        {
            xnode_set_scalar<unsigned long long>(storage, static_cast<unsigned long long>(value));
            break;
        }
        // -- rest --
        case xnode_type_code<std::string>::value:
        {
            *static_cast<std::string *>(*storage) = (value ? xnode_bool_to_str<true>::get_value() : xnode_bool_to_str<false>::get_value());
            break;
        }
        case xnode_type_code<xnode_null_value>::value:
        {
            // empty - does nothing
            break;
        }
        default:
        {
            result = false;
        }
        }

        return result;
    }
};

#endif