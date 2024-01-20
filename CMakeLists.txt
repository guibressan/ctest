# Set CMAKE and C versions
cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile commands for clangd LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# The project name and version
project(ctest VERSION 0.0.0)

# Directories containing header files
include_directories(src test include)

# Process CMakeLists.txt in the subdirs
add_subdirectory(src)
add_subdirectory(test)
