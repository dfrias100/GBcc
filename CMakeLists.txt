cmake_minimum_required(VERSION 3.13)

project(
    GBcc
    DESCRIPTION "C++ based Nintendo Game Boy Emulator using GLFW"
    LANGUAGES C CXX 
)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

add_subdirectory("./external/glfw")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)

option(GBCC_BUILD_TESTS OFF)

if (GBCC_BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()