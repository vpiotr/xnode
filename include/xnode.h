//----------------------------------------------------------------------------------
// Name:        xnode.h
// Purpose:     Union-like dynamic data type able to store any scalar or structure.
// Author:      Piotr Likus
// Created:     01/09/2015
// License:     BSD
//----------------------------------------------------------------------------------

#ifndef __XNODE_H__
#define __XNODE_H__
#include <typeinfo>
#include <iostream>
#include <type_traits>
#include <cassert>
#include <string>
#include <memory>

/// \file xnode.h
/// Union-like data type able to store any scalar or structure.
/// Similar to boost::any.
///
/// Possible value types
/// - null
/// - pointer
/// - scalar (float, integer): stored in pointer
/// - dynamic value, allocated by new, with automatic destruction: scalar, struct or class object
//
/// Classic, C-like build-in arrays are not supported.
//
/// Benefits
/// - any-type variable (dynamic typing)
/// - automatic conversion supported between bool, char, string, floating point & integer values
/// - custom conversion methods can be implemented (defined as politics)
/// - no dynamic allocation for scalar values
/// - move semantics implemented
//
/// Use cases
/// - optional named parameters
/// - dynamic structures with optional parts
/// - suitable for storing values for JSON/XML (with help of included containers)
/// - see unit tests for detailed use cases.

#include "xnode_type_ext.h"
#include "xnode_utils.h"

class xnode_null_value {
public:
};

template<typename T>
struct xnode_is_null {
	static const bool value = false;
};

template<>
struct xnode_is_null<xnode_null_value>{
	static const bool value = true;
};

enum xnode_storage_type { xstNull, xstPointer, xstCasted, xstOwned, xstUndefined };

template<typename T>
struct xnode_type_meta {
	static const bool store_as_null = xnode_is_null<T>::value;
	static const bool store_as_pointer = (!store_as_null) && xn_is_pointer<T>::value;
	static const bool store_as_casted = (!store_as_null) && (!store_as_pointer) && (sizeof(void *) >= sizeof(T)) && (std::is_scalar<T>::value);
	static const bool store_as_owned = !store_as_casted && !xnode_is_null<T>::value;
};

template<typename T>
struct xnode_storage_meta {
	static const int storage_type = xnode_is_null<T>::value ? xstNull :
		(xnode_type_meta<T>::store_as_casted ? xstCasted :
			(xnode_type_meta<T>::store_as_pointer ? xstPointer :
				(xnode_type_meta<T>::store_as_owned ? xstOwned :
				xstUndefined)
			));
};

template<>
struct xnode_storage_meta<xnode_null_value> {
	static const int storage_type = xstNull;
};

template<typename T, int storageType>
class xnode_deleter {
public:
/* Required interface:

	static void destroy(void *ptr) {
		delete static_cast<T *>(ptr);
	}

	static bool needs_deleter() {
		return true;
	}
*/
};

template<typename T>
class xnode_deleter<T, xstNull> {
public:
	static void destroy(void *ptr) {
		// empty
	}

	static bool needs_deleter() {
		return false;
	}
};

template<typename T>
class xnode_deleter<T, xstPointer> {
public:
	static void destroy(void *ptr) {
		// empty
	}

	static bool needs_deleter() {
		return false;
	}
};

template<typename T>
class xnode_deleter<T, xstCasted> {
public:
	static void destroy(void *ptr) {
		// empty
	}

	static bool needs_deleter() {
		return false;
	}
};

template<typename T>
class xnode_deleter<T, xstOwned> {
public:
	static void destroy(void *ptr) {
		delete static_cast<T *>(ptr);
	}

	static bool needs_deleter() {
		return true;
	}
};

template<typename T, int storageType>
class xnode_setter {
public:
	static bool supports_copy() {
		return false;
	}

	// copy value to already existing object using assign operator
	static void copy(void *dest, const void *src) {
		// empty
	}

	// copy from value, optionally invoking change of object pointer at destination side
	static void assign_from_value(void **dest, const void *src) {
		// by default, copy as casted
		*dest = reinterpret_cast<void *>(*static_cast<const T *>(src));
	}

	// accepts pointer to value, takes ownership of it
	static void hold_ptr(void **dest, void *src) {
		std::auto_ptr<T> holder(static_cast<T *>(src));
		*dest = reinterpret_cast<void *>(*holder);
	}

	// copy from another node, ignore old contents (for contructors)
	static void init_from_node(void **dest, const void *src) {
		// by default, raw copy of pointers
		*dest = const_cast<void *>(src);
	}

	// copy from another node using assign operator, assume old value holds pointer to existing object
	static void copy_from_node(void *dest, const void *src) {
		// empty
	}
};

template<typename T>
class xnode_setter<T, xstNull> {
public:
	static bool supports_copy() {
		return false;
	}

	static void copy(void *dest, const void *src) {
		// empty
	}

	static void assign_from_value(void **dest, const void *src) {
		*dest = nullptr;
	}

	static void hold_ptr(void **dest, void *src) {
		std::auto_ptr<T> holder(static_cast<T *>(src));
		*dest = nullptr;
	}

	static void init_from_node(void **dest, const void *src) {
		*dest = const_cast<void *>(src);
	}

	static void copy_from_node(void *dest, const void *src) {
		// empty
	}
};

template<typename T>
class xnode_setter<T, xstCasted> {
public:
	static bool supports_copy() {
		return false;
	}

	// copy value
	static void copy(void *dest, const void *src) {
		// empty
	}

	// copy value, optionally change object pointer at destination side
	static void assign_from_value(void **dest, const void *src) {
		T *dptr = reinterpret_cast<T *>(dest);
		*dptr = *reinterpret_cast<const T *>(src);
	}

	static void hold_ptr(void **dest, void *src) {
		std::auto_ptr<T> holder(static_cast<T *>(src));
		T *dptr = reinterpret_cast<T *>(dest);
		*dptr = *holder;
	}

	static void init_from_node(void **dest, const void *src) {
		// raw copy of pointers
		*dest = const_cast<void *>(src);
	}

	static void copy_from_node(void *dest, const void *src) {
		// empty
	}
};


template<typename T>
class xnode_setter<T, xstPointer> {
public:
	static bool supports_copy() {
		return false;
	}

	// copy value
	static void copy(void *dest, const void *src) {
		// empty
	}

	// copy value, optionally change object pointer at destination side
	static void assign_from_value(void **dest, const void *src) {
		*dest = *reinterpret_cast<T **>(const_cast<void *>(src));
	}

	static void hold_ptr(void **dest, void *src) {
		std::auto_ptr<T> holder(static_cast<T *>(src));
		*dest = const_cast<void *>(reinterpret_cast<const void *>(*holder));
	}

	static void init_from_node(void **dest, const void *src) {
		*dest = const_cast<void *>(src);
	}

	static void copy_from_node(void *dest, const void *src) {
		// empty
	}
};


template<typename T>
class xnode_setter<T, xstOwned>{
public:
	static bool supports_copy() {
		return true;
	}

	// copy value
	static void copy(void *dest, const void *src) {
		T *ndest = static_cast<T *>(dest);
		T *nsrc = static_cast<T *>(const_cast<void *>(src));
		if (ndest)
			*ndest = *nsrc;
		else
			assert(false);
	}

	// copy value, optionally change object pointer at destination side
	static void assign_from_value(void **dest, const void *src) {
		T **ndest = reinterpret_cast<T **>(dest);
		T *nsrc = reinterpret_cast<T *>(const_cast<void *>(src));
		if (*ndest) {
			**ndest = *nsrc;
		}
		else {
			*ndest = new T(*nsrc);
		}
	}

	static void hold_ptr(void **dest, void *src) {
		*dest = src;
	}

	static void init_from_node(void **dest, const void *src) {
		T **ndest = reinterpret_cast<T **>(dest);
		T *nsrc = reinterpret_cast<T *>(const_cast<void *>(src));
		*ndest = new T(*nsrc);
	}

	static void copy_from_node(void *dest, const void *src) {
		T *ndest = reinterpret_cast<T *>(dest);
		T *nsrc = reinterpret_cast<T *>(const_cast<void *>(src));
		*ndest = *nsrc;
	}
};

// default, same as null
template<typename T, int storageType>
class xnode_getter {
public:
	// returns true if value_ptr can be invoked
	static bool supports_ptr() {
		return false;
	}

	// returns true if read_value can be invoked
	static bool supports_read_value() {
		return false;
	}

	// returns pointer to stored value
	static void *value_ptr(void **src) {
		return nullptr;
	}

	static void read_value(void *dest, void **src) {
		// empty
	}

	static bool equals(void *lhs, void *rhs) {
		return lhs == rhs;
	}

	static bool less(void *lhs, void *rhs) {
		return lhs < rhs;
	}
};

template<typename T>
class xnode_getter<T, xstPointer> {
public:
	static bool supports_ptr() {
		return true;
	}

	static bool supports_read_value() {
		return true;
	}

	// returns pointer to value
	static void *value_ptr(void **src) {
		return reinterpret_cast<void *>(src);
	}

	static void read_value(void *dest, void **src) {
		T *destPtr = reinterpret_cast<T *>(dest);
		*destPtr = reinterpret_cast<T>(*src);
	}

	static bool equals(void *lhs, void *rhs) {
		return lhs == rhs;
	}

	static bool less(void *lhs, void *rhs) {
		return lhs < rhs;
	}
};

template<typename T>
class xnode_getter<T, xstCasted> {
public:
	static bool supports_ptr() {
		return true;
	}

	static bool supports_read_value() {
		return true;
	}

	// returns pointer to value
	static void *value_ptr(void **src) {
		return reinterpret_cast<void *>(reinterpret_cast<T *>(src));
	}

	static void read_value(void *dest, void **src) {
		T *destPtr = reinterpret_cast<T *>(dest);
		*destPtr = *reinterpret_cast<T *>(src);
	}

	static bool equals(void *lhs, void *rhs) {
		return lhs == rhs;
	}

	static bool less(void *lhs, void *rhs) {
		T *lptr = reinterpret_cast<T *>(&lhs);
		T *rptr = reinterpret_cast<T *>(&rhs);
		return *lptr  < *rptr;
	}
};

template<typename T>
class xnode_getter<T, xstOwned> {
public:
	static bool supports_ptr() {
		return true;
	}

	static bool supports_read_value() {
		return true;
	}

	// returns pointer to value
	static void *value_ptr(void **src) {
		return static_cast<void *>(static_cast<T *>(*src));
	}

	static void read_value(void *dest, void **src) {
		T *destPtr = reinterpret_cast<T *>(dest);
		*destPtr = *reinterpret_cast<T *>(*src);
	}

	static bool equals(void *lhs, void *rhs) {
		T *lptr = reinterpret_cast<T *>(lhs);
		T *rptr = reinterpret_cast<T *>(rhs);
		return xn_equals(*lptr, *rptr);
	}

	static bool less(void *lhs, void *rhs) {
		T *lptr = reinterpret_cast<T *>(lhs);
		T *rptr = reinterpret_cast<T *>(rhs);
		return xn_less(*lptr, *rptr);
	}
};

struct xnode_vtable {
	int type_code_;
	const std::type_info& value_type_id_;
	void(*deleter_)(void *);
	void(*copy_)(void *, const void *);
	void(*assign_)(void **, const void *);
	void(*hold_)(void **, void *);
	void(*init_from_node_)(void **, const void *);
	void(*copy_from_node_)(void *, const void *);
	void *(*value_ptr_)(void **);
	void (*read_value_)(void *, void **);
	bool(*equals_)(void *, void *);
	bool(*less_)(void *, void *);
};

template<typename T>
const xnode_vtable *xnode_get_vtable() {
    static const xnode_vtable pt = {
        xnode_type_code<T>::value,
        typeid(T),
        (xnode_deleter<T, xnode_storage_meta<T>::storage_type>::needs_deleter() ?
            &xnode_deleter<T, xnode_storage_meta<T>::storage_type>::destroy :
            nullptr
            ),
        (xnode_setter<T, xnode_storage_meta<T>::storage_type>::supports_copy() ?
            &xnode_setter<T, xnode_storage_meta<T>::storage_type>::copy :
            nullptr
        ),
        &xnode_setter<T, xnode_storage_meta<T>::storage_type>::assign_from_value,
        &xnode_setter<T, xnode_storage_meta<T>::storage_type>::hold_ptr,
        &xnode_setter<T, xnode_storage_meta<T>::storage_type>::init_from_node,
        (xnode_setter<T, xnode_storage_meta<T>::storage_type>::supports_copy() ?
            &xnode_setter<T, xnode_storage_meta<T>::storage_type>::copy_from_node :
            nullptr
        ),
        (xnode_getter<T, xnode_storage_meta<T>::storage_type>::supports_ptr() ?
            &xnode_getter<T, xnode_storage_meta<T>::storage_type>::value_ptr :
            nullptr
        ),
		&xnode_getter<T, xnode_storage_meta<T>::storage_type>::read_value,
		&xnode_getter<T, xnode_storage_meta<T>::storage_type>::equals,
		&xnode_getter<T, xnode_storage_meta<T>::storage_type>::less
	};

	return &pt;
}

template<typename T>
T *xnode_get_ptr(void **storage)
{
	return static_cast<T *>(xnode_getter<T, xnode_storage_meta<T>::storage_type>::value_ptr(storage));
}

template<typename T>
T xnode_get_scalar(void **storage) {
	return *static_cast<T *>(xnode_getter<T, xnode_storage_meta<T>::storage_type>::value_ptr(storage));
}

template<typename T>
void xnode_set_scalar(void **storage, T value) {
	xnode_setter<T, xnode_storage_meta<T>::storage_type>::assign_from_value(storage, &value);
}

std::string xnode_pack_value(const char *text) {
    return std::string(text);
}


/// Main class for xnode library,
/// based on generic_t from Advanced C++ Metaprogramming by Davide Di Gennaro.
/// ValuePolicy used for selection of value casting classes.
template<class ValuePolicy = xnode_def_value_policy>
class basic_xnode {
public:
	typedef basic_xnode<ValuePolicy> this_type;
	typedef ValuePolicy value_policy;

	basic_xnode() :
		vtable_(xnode_get_vtable<xnode_null_value>()), value_(nullptr)
	{
	}

	basic_xnode(const basic_xnode& src)
	{
		vtable_ = src.vtable_;
		vtable_->init_from_node_(&value_, src.value_);
	}

	basic_xnode(basic_xnode &&src) :
		vtable_(xnode_get_vtable<xnode_null_value>()), value_(nullptr)
	{
		std::swap(vtable_, src.vtable_);
		std::swap(value_, src.value_);
	}

    ~basic_xnode() {
        destroy();
    }

    /// assigns type & value of right-hand argument
    basic_xnode &operator=(const basic_xnode& src)
	{
		if (this == &src)
			return *this;

		if ((type() == src.type()) && (vtable_->copy_from_node_ != nullptr)) {
			vtable_->copy_from_node_(value_, src.value_);
		}
		else {
			destroy();
			vtable_ = src.vtable_;
			vtable_->init_from_node_(&value_, src.value_);
		}

		return *this;
	}

	basic_xnode &operator=(basic_xnode&& src)
	{
		if (this != &src) {
			std::swap(vtable_, src.vtable_);
			std::swap(value_, src.value_);
		}

		return *this;
	}

    /// compares type and value of nodes
	bool operator==(const basic_xnode& rhs) const {
		if (get_type_code() != rhs.get_type_code())
			return false;
		if (type() != rhs.type())
			return false;
		return vtable_->equals_(value_, rhs.value_);
	}

    /// compares type and value of nodes
	bool operator!=(const basic_xnode& rhs) const {
		return !(*this == rhs);
	}

    /// required for sorting
	bool operator<(const basic_xnode& rhs) const {
		if (get_type_code() != rhs.get_type_code())
			return get_type_code() < rhs.get_type_code(); // for ordering by type in multi-type container
		if (type() != rhs.type())
			return false;
		return vtable_->less_(value_, rhs.value_);
	}

/* Not recommended to use / define (because of casting):
	bool operator<=(const basic_xnode& rhs) const {
		return (*this == rhs) || (*this < rhs);
	}

	bool operator>(const basic_xnode& rhs) const {
		return !(*this <= rhs);
	}

	bool operator>=(const basic_xnode& rhs) const {
		return !(*this < rhs);
	}
*/

    /// returns code of type of value stored in node, =0 for types which have no casting defined
	int get_type_code() const {
		return vtable_->type_code_;
	}

    /// returns type of value stored in node, compatible with built-in typeid()
	const std::type_info& type() const {
		return vtable_->value_type_id_;
	}

    /// checks if node has a null value
	bool is_null() const {
		return (value_ == nullptr) && (vtable_->value_type_id_ == typeid(xnode_null_value));
	}

    /// remove contents of node
	void reset() {
		destroy();
		value_ = nullptr;
		vtable_ = xnode_get_vtable<xnode_null_value>();
	}

	/// set value and type of node
	template<typename T>
	void set_as(const T&value) {
		if ((typeid(T) == type()) && (xnode_setter<T, xnode_storage_meta<T>::storage_type>::supports_copy())) {
			vtable_->copy_(value_, static_cast<const void *>(&value));
		}
		else {
			rebuild_as(value);
		}
	}

	/// set value without changing assigned type
	template<typename T>
	void set_value(const T&value) {
		if ((typeid(T) == type()) && (xnode_setter<T, xnode_storage_meta<T>::storage_type>::supports_copy())) {
			vtable_->copy_(value_, static_cast<const void *>(&value));
		}
		else {
            if (!xnode_caster<T, typename value_policy::cast_policy>::cast_from_value(&value_, vtable_->type_code_, value))
                throwWrongCastFromValue<T>();
		}
	}

    /// get copy of value with optional conversion
	template<typename T>
	T get_as() const {
		T result;
		if (xnode_getter<T, xnode_storage_meta<T>::storage_type>::supports_read_value() && (typeid(T) == type())) {
			vtable_->read_value_(&result, const_cast<void **>(&value_));
			return result;
		}

        if (!xnode_caster<T, typename value_policy::cast_policy>::cast_to_value(result, const_cast<void **>(&value_), vtable_->type_code_)) {
            throwWrongCastToValue<T>();
	    }
		return result;
	}

    /// get copy of value with optional conversion
	template<typename T>
	T &get_as(T &output) {
		if(xnode_getter<T, xnode_storage_meta<T>::storage_type>::supports_read_value() && (typeid(T) == type())) {
			vtable_->read_value_(&output, &value_);
			return output;
		}

		if (!xnode_caster<T, typename value_policy::cast_policy>::cast_to_value(output, &value_, vtable_->type_code_))
            throwWrongCastToValue<T>();

        return output;
	}

    /// get copy of value with optional conversion
    /// does not throw on wrong cast
    /// \param[in] def_value value to be used when stored value cannot be retrieved
	template<typename T>
	T get_as_def(const T& def_value) const {
		T result;
		if (xnode_getter<T, xnode_storage_meta<T>::storage_type>::supports_read_value() && (typeid(T) == type())) {
			vtable_->read_value_(&result, const_cast<void **>(&value_));
			return result;
		}

        if (xnode_caster<T, typename value_policy::cast_policy>::cast_to_value(result, const_cast<void **>(&value_), vtable_->type_code_))
            return result;

		return def_value;
	}

    /// get copy of value with optional conversion
    /// does not throw on wrong cast
    /// \param[in] def_value value to be used when stored value cannot be retrieved
	template<typename T>
	T &get_as_def(T &output, const T& def_value) {
		if(xnode_getter<T, xnode_storage_meta<T>::storage_type>::supports_read_value() && (typeid(T) == type())) {
			vtable_->read_value_(&output, &value_);
			return output;
		}

        if (xnode_caster<T, typename value_policy::cast_policy>::cast_to_value(output, &value_, vtable_->type_code_))
            return output;

        return def_value;
	}

    /// returns true if node can be read as a given type
	template<typename T>
    bool is_convertable_to() {
        if (xnode_getter<T, xnode_storage_meta<T>::storage_type>::supports_read_value() && (typeid(T) == type())) {
            return true;
        } else {
            T tmp;
            return xnode_caster<T, typename value_policy::cast_policy>::cast_to_value(tmp, &value_, vtable_->type_code_);
        }
    }

	/// returns pointer to stored value, null if type is not matched or cannot be pointed
	template<typename T>
	T *get_ptr() {
		if (!xnode_getter<T, xnode_storage_meta<T>::storage_type>::supports_ptr())
			return nullptr;

		if (typeid(T) == type())
            return static_cast<T *>(vtable_->value_ptr_(&value_));

	    return nullptr;
	}

	/// returns pointer to stored value, null if type is not matched or cannot be pointed
	template<typename T>
	const T *get_ptr() const {
		if (!xnode_getter<T, xnode_storage_meta<T>::storage_type>::supports_ptr())
			return nullptr;
		if (typeid(T) != type())
			return nullptr;
		return static_cast<T *>(vtable_->value_ptr_(const_cast<void **>(&value_)));
	}

    /// returns (void *) pointer to value if supported, otherwise null
	void *get_vptr() {
		if (!vtable_->value_ptr_)
			return nullptr;

        return vtable_->value_ptr_(&value_);
	}

    /// returns (void *) pointer to value if supported, otherwise null
	const void *get_vptr() const {
		if (!vtable_->value_ptr_)
			return nullptr;

        return vtable_->value_ptr_(&value_);
	}

	/// returns reference to stored value, throws if cannot be retrieved
	template<typename T>
	T &get_ref() {
        T *ptr = get_ptr<T>();

        if (!ptr)
            throwRefReadFail();

	    return *ptr;
	}

	/// returns reference to stored value, throws if cannot be retrieved
	template<typename T>
	const T &get_ref() const {
        const T *ptr = get_ptr<T>();

        if (!ptr)
            throwRefReadFail();

	    return *ptr;
	}

	/// returns reference to stored value, throws if cannot be retrieved
    /// \param[in] def_value value to be used when stored value cannot be retrieved
	template<typename T>
	const T &get_ref_def(const T& def_value) const {
        const T *ptr = get_ptr<T>();

        if (ptr)
            return *ptr;
        else
            return def_value;
	}

	/// release ownership of object and return pointer to it,
	/// \result returns null for non-owned values or if type is incorrect
	template<typename T>
	T *release() {
		std::auto_ptr<T> result;

		if (vtable_->deleter_ != nullptr) {
			result.reset(get_ptr<T>());
		}

		value_ = nullptr;
		vtable_ = xnode_get_vtable<xnode_null_value>();

		if (result.get())
			return result.release();
		else
			return nullptr;
	}

	/// take ownership of dynamic object
	template<typename T>
	void hold(T *value) {
		std::auto_ptr<T> holder(value);
		destroy();
		vtable_ = xnode_get_vtable<T>();
		vtable_->hold_(&value_, holder.release());
	}

    /// returns true if type is matching value stored in xnode
	template<typename T>
	bool is() const {
		return typeid(T) == type();
	}

    /// object builder function with optional type casting
    /// \param[in] DestType final value type
    /// \param[in] ValueType input value type
    /// \param[in] value input value to be converted to xnode
    /// \result Returns xnode of type DestType
    template<typename DestType, typename ValueType>
	static this_type value_of(const ValueType &value) {
		this_type valueNode;
		valueNode.set_as(value);

		DestType tempValue;
		this_type result;
		result.set_as<DestType>(valueNode.get_as(tempValue));
		return result;
	}

    /// object builder function
    /// \param[in] ValueType input value type
    /// \param[in] value input value to be converted to xnode
    /// \result Returns xnode of type ValueType
    template<typename ValueType>
	static this_type value_of(const ValueType &value, typename std::enable_if<!std::is_array<ValueType>::value >::type* = 0) {
		this_type result;
		result.set_as(value);
		return result;
	}

    /// object builder function for arrays
    /// \param[in] ValueType input value type
    /// \param[in] value input value to be converted to xnode
    /// \result Returns xnode of type ValueType
    template<typename ValueType>
	static this_type value_of(const ValueType &value, typename std::enable_if<std::is_array<ValueType>::value >::type* = 0) {
		this_type result;
		result.set_as(xnode_pack_value(value));
		return result;
	}

protected:
	void destroy() {
		if (vtable_->deleter_ != nullptr) {
			vtable_->deleter_(value_);
		}
	}

	template<typename T>
	void rebuild_as(const T&value) {
		destroy();
		value_ = nullptr;
		vtable_ = xnode_get_vtable<T>();
		vtable_->assign_(&value_, static_cast<const void *>(&value));
	}

    void throwRefReadFail() const {
        throw std::runtime_error(std::string("Reference read failed, data type: ") + to_string(vtable_->type_code_) + ", name: " + vtable_->value_type_id_.name());
    }

    template<typename T>
    void throwWrongCastToValue() const {
        throw std::runtime_error(std::string("Conversion to value failed, storage type: [ code: ") + to_string(vtable_->type_code_) + ", name: " + vtable_->value_type_id_.name() + " ], value type name: " + typeid(T).name());
    }

    template<typename T>
    void throwWrongCastFromValue() const {
        throw std::runtime_error(std::string("Conversion from value failed, storage type: [ code: ") + to_string(vtable_->type_code_) + ", name: " + vtable_->value_type_id_.name() + " ], value type name: " + typeid(T).name());
    }

private:
	const xnode_vtable *vtable_;
	void *value_;
};

typedef basic_xnode<> xnode;

#include "details/xnode_builtins.h"

#endif
