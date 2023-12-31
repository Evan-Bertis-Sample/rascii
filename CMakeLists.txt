cmake_minimum_required(VERSION 3.10)
project(rascii VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
add_subdirectory(src)
add_subdirectory(test)
