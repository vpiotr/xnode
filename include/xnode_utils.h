//----------------------------------------------------------------------------------
// Name:        xnode_utils.h
// Purpose:     utility classes not related directly to xnode class.
// Author:      Piotr Likus
// Created:     01/09/2015
// Last change: 12/09/2015
// License:     BSD
//----------------------------------------------------------------------------------

#ifndef __XNODE_UTILS_H__
#define __XNODE_UTILS_H__

#include <sstream>
#include <string>

template < class T >
std::string to_string(const T &arg)
{
	std::ostringstream out;
	out << arg;
	return(out.str());
}

template <typename T>
T from_string(const std::string &str) {
	std::istringstream is(str);
	T t;
	is >> t;
	return t;
}

template <typename T>
bool from_string(T &output, const std::string &str) {
	std::istringstream is(str);
    return !(!(is >> output));
}

template <typename T>
T from_string_def(const std::string &str, const T &defValue) {
	std::istringstream is(str);
	T res;
	if (!(is >> res))
		res = defValue;
	return res;
}

template<bool Parameter>
struct xnSelector {
	static const bool value = Parameter;
};

template<bool Condition, class T>
struct xnEnableIfType {
	typedef T type;
};

template<class T>
struct xnEnableIfType<false, T> {
};

template<class Condition, class T>
struct xnEnableIf : public xnEnableIfType<Condition::value, T> {
};

template<class Condition, class T>
struct xnDisableIf : public xnEnableIfType<(!Condition::value), T> {
};

template<typename T>
struct xn_is_pointer { static const bool value = false; };

template<typename T>
struct xn_is_pointer<T*> { static const bool value = true; };

// from SO, red Jun 30 '11 at 14:17, iammilind
namespace XN_CHECK_EQUALS  // namespace to let "operator ==" not become global
{
	typedef char no[7];
	template<typename T> no& operator == (const T&, const T&);

	template <typename T>
	struct opEqualExists // *(T*)(0) can be replaced by *new T[1] also
	{
		enum { value = (sizeof(*(T*)(0) == *(T*)(0)) != sizeof(no)) };
	};
}

namespace XN_CHECK_LESS
{
	typedef char no[7];
	template<typename T> no& operator < (const T&, const T&);

	template <typename T>
	struct opLessExists
	{
		enum { value = (sizeof(*(T*)(0) < *(T*)(0)) != sizeof(no)) };
	};
}

template<typename T>
struct has_equals_operator {
    static const bool value = (XN_CHECK_EQUALS::opEqualExists<T>::value != 0);
};

template<typename T>
struct has_less_operator {
    static const bool value = (XN_CHECK_LESS::opLessExists<T>::value != 0);
};

// checks if objects are equal, throws if operator == not implemented
template<class T>
typename xnEnableIf<
	xnSelector<
	has_equals_operator<T>::value>, bool>::type
	xn_equals(const T& lhs, const T& rhs)
{
	return lhs == rhs;
}

// checks if objects are equal, throws if operator == not implemented
template<class T>
typename xnEnableIf<
	xnSelector<
	!has_equals_operator<T>::value>, bool>::type
	xn_equals(const T& lhs, const T& rhs)
{
	throw std::runtime_error(std::string("Equals not implemented, type: ") + typeid(T).name());
}

// checks if objects left is less than right, throws if operator == not implemented
template<class T>
typename xnEnableIf<
	xnSelector<
	has_less_operator<T>::value>, bool>::type
	xn_less(const T& lhs, const T& rhs)
{
	return lhs < rhs;
}

// checks if objects are equal, throws if operator == not implemented
template<class T>
typename xnEnableIf<
	xnSelector<
	!has_less_operator<T>::value>, bool>::type
	xn_less(const T& lhs, const T& rhs)
{
	throw std::runtime_error(std::string("Less operator not implemented, type: ") + typeid(T).name());
}

#endif
