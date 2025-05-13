add_executable(xnode_test xnode_test.cpp)
target_link_libraries(xnode_test PRIVATE xnode)

# Add test to CTest
add_test(NAME xnode_test COMMAND xnode_test)

# Add conversion tests
add_executable(xnode_convert_test xnode_convert_test.cpp)
target_link_libraries(xnode_convert_test PRIVATE xnode)

# Add conversion test to CTest
add_test(NAME xnode_convert_test COMMAND xnode_convert_test)

# Add type tests
add_executable(xnode_type_test xnode_type_test.cpp)
target_link_libraries(xnode_type_test PRIVATE xnode)

# Add test to CTest
add_test(NAME xnode_type_test COMMAND xnode_type_test)

# Add overflow tests
add_executable(xnode_overflow_test xnode_overflow_test.cpp)
target_link_libraries(xnode_overflow_test PRIVATE xnode)

# Add overflow test to CTest
add_test(NAME xnode_overflow_test COMMAND xnode_overflow_test)

# Add xarray tests
add_executable(xarray_test xarray_test.cpp)
target_link_libraries(xarray_test PRIVATE xnode)

# Add xarray test to CTest
add_test(NAME xarray_test COMMAND xarray_test)

# Add xobject tests
add_executable(xobject_test xobject_test.cpp)
target_link_libraries(xobject_test PRIVATE xnode)

# Add xobject test to CTest
add_test(NAME xobject_test COMMAND xobject_test)

# Install the test executable if needed (optional)
install(TARGETS xnode_test xnode_convert_test xnode_type_test xnode_overflow_test xarray_test xobject_test
    RUNTIME DESTINATION bin
    OPTIONAL
)