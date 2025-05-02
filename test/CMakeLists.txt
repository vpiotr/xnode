add_executable(xnode_test xnode_test.cpp)
target_link_libraries(xnode_test PRIVATE xnode)

# Add test to CTest
add_test(NAME xnode_test COMMAND xnode_test)

# Add conversion tests
add_executable(xnode_convert_test xnode_convert_test.cpp)
target_link_libraries(xnode_convert_test PRIVATE xnode)

# Add conversion test to CTest
add_test(NAME xnode_convert_test COMMAND xnode_convert_test)

# Install the test executable if needed (optional)
install(TARGETS xnode_test xnode_convert_test
    RUNTIME DESTINATION bin
    OPTIONAL
)