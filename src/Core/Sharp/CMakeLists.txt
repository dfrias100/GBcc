add_library(Sharp Sharp.cpp)
add_library(SharpRegister SharpRegister.cpp)
add_subdirectory("./Instructions")
add_subdirectory("./Decoding")

target_include_directories(
    Sharp PRIVATE
    "../../../include"
)

target_include_directories(
    SharpRegister PRIVATE
    "../../../include"
)

target_link_libraries(Sharp SharpRegister)