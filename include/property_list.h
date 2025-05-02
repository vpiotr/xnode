//----------------------------------------------------------------------------------
// Name:        property_list.h
// Purpose:     Key-value storage class with support for access order of insertion.
// Author:      Piotr Likus
// Created:     01/09/2015
// License:     BSD
//----------------------------------------------------------------------------------

#ifndef __PROP_LIST_H__
#define __PROP_LIST_H__

#include <unordered_map>
#include <vector>
#include <set>
#include <string>
#include <stdexcept>

#include "xnode_utils.h"

/// key-value pair container where key is unique
/// container supports reading in insert order
/// reading & deleting by key has O(1) complexity
/// value is stored in just one place
template<typename KeyType, typename ValueType>
class property_list {
	typedef std::vector<KeyType> key_container_type;
	typedef std::unordered_map<KeyType, ValueType> value_container_type;
public:
	typedef std::vector<KeyType> key_list_type;
    typedef property_list<KeyType, ValueType> this_type;

	property_list() : dirty_keys_(false) {}

    property_list(const property_list &src):
        dirty_keys_(src.dirty_keys_),
        keys_(src.keys_),
        values_(src.values_)
    {

    }

    property_list(property_list &&src) :
        dirty_keys_(src.dirty_keys_),
        keys_(std::move(src.keys_)),
        values_(std::move(src.values_))
    {

    }

    property_list& operator=(const property_list &rhs)
    {
        dirty_keys_ = rhs.dirty_keys_;
        keys_ = rhs.keys_;
        values_ = rhs.values_;
        return *this;
    }

    property_list& operator=(property_list &&rhs)
    {
        dirty_keys_ = rhs.dirty_keys_;
        keys_ = std::move(rhs.keys_);
        values_ = std::move(rhs.values_);
        return *this;
    }

    bool operator==(const this_type &rhs) const
    {
        if (this == &rhs)
            return true;

        if (
            (values_ == rhs.values_)
            &&
            (keys_ == rhs.keys_)
            )
        {
            return true;
        }
        else {
            return false;
        }
    }

    /// inserts value in storage, if key already exists, old value will be replaced
    /// returns true if item was already there
	bool put(const KeyType &key, const ValueType &value) {
		typename value_container_type::iterator found = values_.find(key);
        bool notFound = (found == values_.end());
        if (notFound) {
			keys_.push_back(key);
		}
		values_[key] = value;
        return !notFound;
	}

    /// inserts value in storage, if key already exists, old value will be replaced
    /// returns true if item was already there
	bool put(const KeyType &key, ValueType &&value) {
		typename value_container_type::iterator found = values_.find(key);
        bool notFound = (found == values_.end());
        if (notFound) {
			keys_.push_back(key);
            values_.insert({ key, value });
        }
        else {
            found->second = value;
        }
        return !notFound;
	}

    /// returns value selected by key
    /// throws error if key not found
	ValueType &get(const KeyType &key) {
		typename value_container_type::iterator found = values_.find(key);
		if (found == values_.end()) {
			throwNotFound(key);
		}
		return found->second;
	}

    /// returns value selected by key
    /// throws error if key not found
	const ValueType &get(const KeyType &key) const {
		typename value_container_type::const_iterator found = values_.find(key);
		if (found == values_.end()) {
			throwNotFound(key);
		}
		return found->second;
	}

    /// returns value selected by key
    /// returns provided default value if key not found
	const ValueType get_def(const KeyType &key, const ValueType &defValue) const {
		typename value_container_type::const_iterator found = values_.find(key);
		if (found == values_.end()) {
            return defValue;
		}
		return found->second;
	}

    /// returns value selected by key
    /// throws error if key not found
	ValueType &get(const KeyType &key, ValueType &output) const {
		typename value_container_type::iterator found = values_.find(key);
		if (found == values_.end()) {
			throwNotFound(key);
		}
      	output = *found;
		return output;
	}

    /// returns pointer to value selected by key
    /// returns null if value not found
	ValueType *get_ptr(const KeyType &key) {
		typename value_container_type::iterator found = values_.find(key);
		if (found != values_.end()) {
			return &(found->second);
		}
		else {
			return nullptr;
		}
	}

    /// removes value selected by key
	void remove(const KeyType &key) {
		typename value_container_type::iterator found = values_.find(key);
		if (found != values_.end()) {
			values_.erase(found);
			dirty_keys_ = true;
		}
	}

    /// removes all items stored in container
	void clear() {
		values_.clear();
		keys_.clear();
		dirty_keys_ = false;
	}

    /// returns number of stored values
	size_t size() const {
		return values_.size();
	}

    /// returns true if there are no values in the storage
	bool empty() const {
		return values_.empty();
	}

    /// returns true if reorg is needed to be executed
    bool needs_reorg() const {
        return dirty_keys_;
    }

    /// reorganize structure after heavy changes
	void reorg() {
		if (dirty_keys_)
			purge_keys();
	}

    /// returns true if container holds value for a given key
	bool contains(const KeyType &key) const {
		typename value_container_type::const_iterator found = values_.find(key);
		return found != values_.end();
	}

	/// return keys in order of insertion
	key_list_type get_keys() {
		if (dirty_keys_)
			purge_keys();
		key_list_type result(keys_);
		return result;
	}

	/// return keys in order of insertion
    /// param[in] helper, buffer optionally to be used
    /// return reference to key storage
	const key_list_type &get_keys(key_list_type &helper) {
		if (dirty_keys_)
			purge_keys();
		return keys_;
	}

	/// return values in order of insertion
	std::vector<ValueType> get_values() {
		std::vector<ValueType> result;
		if (dirty_keys_)
			purge_keys();
		for (KeyType key : keys_)
			result.push_back(values_[key]);
		return result;
	}

protected:
	void purge_keys() {
		key_container_type new_keys;
		for (KeyType key : keys_) {
			if (values_.find(key) != values_.end())
				new_keys.push_back(key);
		}
		keys_ = std::move(new_keys);
		dirty_keys_ = false;
	}

	void throwNotFound(const KeyType &key) const {
		throw std::runtime_error("key not found: " + to_string(key));
	}
private:
	bool dirty_keys_;
	key_container_type keys_;
	value_container_type values_;
};

#endif
