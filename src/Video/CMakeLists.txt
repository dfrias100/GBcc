add_library(Video Video.cpp  "../../external/glad/src/glad.c")
target_link_libraries(Video glfw)
target_include_directories(
    Video PRIVATE
    "../../include/"
    "../../external/glfw/include"
    "../../external/glad/include"
)