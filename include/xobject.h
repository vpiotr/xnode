#ifndef XOBJECT_H
#define XOBJECT_H

#include "details/property_list.h"
#include "xnode.h"

typedef property_list<std::string, xnode> xobject;

// Define a specific type code for long double to distinguish it
template<>
struct xnode_type_code<xobject> {
    enum { value = 15 }; // Ensure this doesn't conflict with other type codes
};

#endif // XOBJECT_H