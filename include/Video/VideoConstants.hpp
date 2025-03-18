/*
* GBcc: Game Boy (DMG) Emulator
* Copyright (C) 2025 Daniel Frias
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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

    constexpr float GAMEBOY_REFRESH_RATE = 59.7275f;
}