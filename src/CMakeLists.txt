add_executable(GBcc main.cpp "../external/glad/src/glad.c")
target_include_directories(
    GBcc PRIVATE
    "../external/glad/include"
    "../external/glfw/include"
)
target_link_libraries(GBcc glfw)