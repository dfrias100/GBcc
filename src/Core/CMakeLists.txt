add_subdirectory("./Sharp")

add_library(Memory Memory.cpp)

target_include_directories(
    Memory PRIVATE
    "../../include"
)