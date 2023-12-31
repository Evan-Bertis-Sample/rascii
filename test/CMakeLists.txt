# Add an executable for tests
add_executable(rascii_test main_test.cpp)

# Explicitly state that this is not a WIN32 executable
set_target_properties(rascii_test PROPERTIES
    WIN32_EXECUTABLE FALSE
)

# Specify include directories
target_include_directories(rascii_test PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Link the test executable with the main library (if needed)
# target_link_libraries(rascii_test PRIVATE rascii)


# output the test executable to the bin directory
set_target_properties(rascii_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)