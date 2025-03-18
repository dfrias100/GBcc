add_library(Video Video.cpp)
target_link_libraries(Video glfw)
target_include_directories(
    Video PRIVATE
    "../../include/"
    "../../external/glfw/include"
    "../../external/glad/include"
)