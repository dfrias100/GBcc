#pragma once
#include "Types.hpp"

namespace GBcc::VideoConstants
{
    constexpr i32 GAMEBOY_SCREEN_WIDTH  = 160U;
    constexpr i32 GAMEBOY_SCREEN_HEIGHT = 144U;

    constexpr u64 GAMEBOY_SCREEN_SCALE = 5U;

    constexpr i32 GL_VERSION_MAJOR = 3;
    constexpr i32 GL_VERSION_MINOR = 3;

    constexpr u64 FRAMEBUFFER_SIZE = GAMEBOY_SCREEN_WIDTH * GAMEBOY_SCREEN_HEIGHT * 3ULL;
}