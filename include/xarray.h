//----------------------------------------------------------------------------------
// Name:        xarray.h
// Purpose:     Custom array class for xnode objects.
// Author:      Piotr Likus
// Created:     01/09/2015
// Last change: 15/05/2025
// License:     BSD
//----------------------------------------------------------------------------------

#ifndef __XNODE_ARRAY_H__
#define __XNODE_ARRAY_H__

#include <vector>
#include <initializer_list>
#include <cassert>
#include "xnode.h"

// Forward declaration
class xarray;

// Type code registration for xarray
template<>
struct xnode_type_code<xarray> {
    enum { value = 16 }; // Ensure this doesn't conflict with other type codes
};

/**
 * Custom array class for xnode objects.
 * Provides a limited array interface with static initializer "of" that accepts variable number of xnode objects.
 */
class xarray {
public:
    // Type definitions
    using value_type = xnode;
    using container_type = std::vector<value_type>;
    using reference = container_type::reference;
    using const_reference = container_type::const_reference;
    using iterator = container_type::iterator;
    using const_iterator = container_type::const_iterator;
    using size_type = container_type::size_type;
    using difference_type = container_type::difference_type;

    // Constructors
    xarray() = default;
    
    // Copy constructor
    xarray(const xarray& other) = default;
    
    // Move constructor
    xarray(xarray&& other) noexcept = default;
    
    // Construct from initializer list of xnodes
    xarray(std::initializer_list<value_type> init) : data_(init) {}
    
    // Assignment operators
    xarray& operator=(const xarray& other) = default;
    xarray& operator=(xarray&& other) noexcept = default;
    
    // Basic functions
    bool empty() const { return data_.empty(); }
    size_type size() const { return data_.size(); }
    
    // Element access
    reference at(size_type pos) { return data_.at(pos); }
    const_reference at(size_type pos) const { return data_.at(pos); }
    
    reference operator[](size_type pos) { return data_[pos]; }
    const_reference operator[](size_type pos) const { return data_[pos]; }
    
    // Iterators
    iterator begin() { return data_.begin(); }
    const_iterator begin() const { return data_.begin(); }
    const_iterator cbegin() const { return data_.cbegin(); }
    
    iterator end() { return data_.end(); }
    const_iterator end() const { return data_.end(); }
    const_iterator cend() const { return data_.cend(); }
    
    // Capacity manipulation
    void reserve(size_type new_cap) { data_.reserve(new_cap); }
    size_type capacity() const { return data_.capacity(); }
    
    // Modifiers
    void clear() { data_.clear(); }
    void push_back(const value_type& value) { data_.push_back(value); }
    void push_back(value_type&& value) { data_.push_back(std::move(value)); }
    iterator insert(const_iterator pos, const value_type& value) { return data_.insert(pos, value); }
    iterator insert(const_iterator pos, value_type&& value) { return data_.insert(pos, std::move(value)); }
    iterator erase(const_iterator pos) { return data_.erase(pos); }
    void resize(size_type count, const value_type& value = value_type()) { data_.resize(count, value); }
      // C++17 and above: Static factory function that accepts variable number of arguments
    // and converts them directly to xnode objects    
    
    #if __cplusplus >= 201703L
    // C++17 version that auto-converts values to xnode objects
    template <typename... Args>
    static xarray of(Args&&... args) {
        xarray result;
        result.reserve(sizeof...(args));
        (result.push_back(xnode::value_of(std::forward<Args>(args))), ...); // Fold expression (C++17)
        return result;
    }
    
    // C++17 version for direct xnode objects (to avoid unnecessary conversion)
    template <typename... Args>
    static xarray of_nodes(const xnode& first, Args&&... args) {
        xarray result;
        result.reserve(sizeof...(args) + 1);
        result.push_back(first);
        (result.push_back(std::forward<Args>(args)), ...); // Fold expression (C++17)
        return result;
    }
    
    // Specialization for empty array
    static xarray of_nodes() {
        return xarray();
    }
    #else
    // Pre-C++17: Recursive variadic template implementations
    
    // Version that requires explicit xnode arguments
    static xarray of() {
        return xarray();
    }

    template <typename... Args>
    static xarray of(const xnode& first, const Args&... rest) {
        xarray result;
        result.reserve(sizeof...(rest) + 1);
        result.push_back(first);
        appendToArray(result, rest...);
        return result;
    }

    // Alias for consistency with the C++17 version
    static xarray of_nodes() {
        return xarray();
    }

    template <typename... Args>
    static xarray of_nodes(const xnode& first, const Args&... rest) {
        return of(first, rest...);
    }

    // Helper function for pre-C++17 implementation
    private:
    static void appendToArray(xarray& /* array */) {
        // Base case: no more elements to add
    }

    template <typename... Args>
    static void appendToArray(xarray& array, const xnode& first, const Args&... rest) {
        array.push_back(first);
        appendToArray(array, rest...);
    }
    public:
    #endif

    // Compatibility with algorithms that expect STL containers
    bool operator==(const xarray& other) const { return data_ == other.data_; }
    bool operator!=(const xarray& other) const { return data_ != other.data_; }
    bool operator<(const xarray& other) const { return data_ < other.data_; }

private:
    container_type data_;
};

#endif // __XNODE_ARRAY_H__

