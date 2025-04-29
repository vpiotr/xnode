add_executable(xnode_test xnode_test.cpp)
target_link_libraries(xnode_test PRIVATE xnode)

# Add test to CTest
add_test(NAME xnode_test COMMAND xnode_test)

# Install the test executable if needed (optional)
install(TARGETS xnode_test
    RUNTIME DESTINATION bin
    OPTIONAL
)