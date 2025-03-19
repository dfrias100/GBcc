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
#include "Emulator/Emulator.hpp"
#include "Video/VideoConstants.hpp"

#include <cstdlib>

namespace GBcc {
    Emulator::Emulator() : m_Video(Video::getInstance()) {}
    
    Emulator::~Emulator() { }

    void Emulator::CapFramerate(const float fps)
    {
        float expectedFrametime = 1e9f / fps;
        auto chronoExpectedFrametime = std::chrono::nanoseconds(static_cast<u64>(expectedFrametime));
        auto expectedEndTime = m_StartFrame + chronoExpectedFrametime;
        while (m_Timer.now() < expectedEndTime) {}
    }

    void Emulator::Run()
    {    
        std::array<u8, 160U * 144U * 3U> framebuffer;
        std::array<std::array<u8, 3U>, 4U> color_lut ={
            {
                {0x08, 0x18, 0x20},
                {0x34, 0x68, 0x56},
                {0x88, 0xC0, 0x70},
                {0xE0, 0xF8, 0xD0}
            }
        };

        u64 color1 = 0;
        u64 color2 = 2;

        for (size_t y = 0; y < 18; y++)
        {
            for (size_t x = 0; x < 20; x++)
            {
                constexpr u64 box_pixel_stride = 3U * 8U;
                constexpr u64 box_row_stride = 20U * 8U * box_pixel_stride;
                size_t start = y * box_row_stride + x * box_pixel_stride;

                u64 color;
                if (x % 2 == 0)
                    color = color1;
                else
                    color = color2;

                for (size_t box_y = 0; box_y < 8; box_y++)
                {
                    for (size_t box_x = 0; box_x < 8; box_x++)
                    {
                        constexpr u64 pixel_stride = 3U;
                        constexpr u64 row_stride = VideoConstants::GAMEBOY_SCREEN_WIDTH * pixel_stride;
                        size_t real_index = start + box_y * row_stride + box_x * pixel_stride;

                        std::copy(color_lut[color].begin(), color_lut[color].end(), framebuffer.begin() + real_index);
                    }
                }
                color ^= 1;
            }
            std::swap(color1, color2);
        }

        framebuffer[0] = 0xFF;
        framebuffer[1] = 0xFF;
        framebuffer[2] = 0xFF;

        framebuffer[framebuffer.size() - 3] = 0xFF;
        framebuffer[framebuffer.size() - 2] = 0xFF;
        framebuffer[framebuffer.size() - 1] = 0xFF;

        while (!m_Video.ShouldClose())
        {
            m_StartFrame = m_Timer.now();

            m_Video.UpdateTexture(framebuffer);
            m_Video.Draw();

            CapFramerate(VideoConstants::GAMEBOY_REFRESH_RATE);
        }
    }
}