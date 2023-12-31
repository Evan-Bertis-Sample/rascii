# Gather all .cpp files in your src directory
file(GLOB SOURCES "*.cpp")

# Add an executable with your sources
add_executable(rascii ${SOURCES})

# Explicitly state that this is not a WIN32 executable
set_target_properties(rascii PROPERTIES
    WIN32_EXECUTABLE FALSE
)

# Specify include directories
target_include_directories(rascii PUBLIC "${PROJECT_SOURCE_DIR}/include")


# output the executable to the bin directory
set_target_properties(rascii PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin"
)