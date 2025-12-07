cmake_minimum_required(VERSION 3.10.0)

project(swift-cast VERSION 1.0.0 LANGUAGES C CXX)

# Output all runtimes to a common directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Generate compile-commands.json for Clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(swift-cast src/main.cpp)

include(CTest)
enable_testing()

