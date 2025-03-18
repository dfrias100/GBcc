cmake_minimum_required(VERSION 3.10)

project(
    GBcc
    DESCRIPTION "C++ based Nintendo Game Boy Emulator using GLFW"
    LANGUAGES C CXX 
)

add_subdirectory("./external/glfw")

set(CMAKE_CXX_STANDARD 17)

IF (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
ELSE()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
ENDIF()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)
