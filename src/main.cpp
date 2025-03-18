#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>
#include <cstdlib>

#include "Video/Video.hpp"

int main(void)
{
    auto& v = GBcc::Video::getInstance();
    std::array<uint8_t, 160U * 144U * 3U> framebuffer;

    auto gen_static = [&framebuffer]() {
        for (size_t i = 0; i < framebuffer.size(); i += 3)
        {
            uint8_t col = (rand() % 4) * 85;
            std::fill(framebuffer.begin() + i, framebuffer.begin() + i + 3, col);
        }
    };

    while (!v.ShouldClose())
    {
        gen_static();
        v.UpdateTexture(framebuffer);
        v.Draw();
    }

    return 0;
}