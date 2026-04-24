
cmake_minimum_required(VERSION 3.10)
project(list_project)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create an executable for testing
add_executable(code main.cpp)

# Include the current directory for header files
target_include_directories(code PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Set compiler options for better warnings and optimization
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Link necessary libraries if needed
# target_link_libraries(code)
