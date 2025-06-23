add_library(Emulator Emulator.cpp)
target_link_libraries(Emulator Video System)
target_include_directories(
    Emulator PRIVATE
    "../../include/"
    "../../external/glfw/include"
    "../../external/glad/include"
)