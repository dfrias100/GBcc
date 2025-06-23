add_subdirectory("./Sharp")

add_library(Memory Memory.cpp)
add_library(System System.cpp)

target_include_directories(
    Memory PRIVATE
    "../../include"
)

target_include_directories(
    System PRIVATE
    "../../include"
)

target_link_libraries(System Memory Sharp)