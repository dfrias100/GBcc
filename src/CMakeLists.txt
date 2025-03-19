add_subdirectory("./Video")
add_subdirectory("./Emulator")
add_subdirectory("./Core")

add_executable(GBcc main.cpp)

target_include_directories(
    GBcc PRIVATE
    "../external/glad/include"
    "../external/glfw/include"
    "../include"
)

target_link_libraries(GBcc Emulator)