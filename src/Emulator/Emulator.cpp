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
#include <sstream>

namespace GBcc {
    Emulator::Emulator() : m_Video(Video::GetInstance()) {}
    
    Emulator::~Emulator() { }

    void Emulator::LimitFramerate(const float fps)
    {
        float expectedFrametime = 1e9f / fps;
        auto chronoExpectedFrametime = std::chrono::nanoseconds(static_cast<u64>(expectedFrametime));
        auto expectedEndTime = m_StartFrame + chronoExpectedFrametime;
        while (m_Timer.now() < expectedEndTime) {}
    }

    void Emulator::Run()
    {    
        std::array<u8, 160U * 144U * 3U> framebuffer;
        std::array<std::array<u8, 3U>, 4U> colorLut ={
            {
                {0x08, 0x18, 0x20},
                {0x34, 0x68, 0x56},
                {0x88, 0xC0, 0x70},
                {0xE0, 0xF8, 0xD0}
            }
        };

        u64 hScroll = 0;
        u64 vScroll = 0;

        auto DrawChecker = [&framebuffer, colorLut](const u64 hScroll, const u64 vScroll)
        {
            u64 color1 = 0;
            u64 color2 = 2;

            for (size_t y = 0; y < 18; y++)
            {
                for (size_t x = 0; x < 20; x++)
                {
                    constexpr u64 boxPixelStride = 3U * 8U;
                    constexpr u64 boxRowStride = 20U * 8U * boxPixelStride;
                    size_t start = y * boxRowStride + x * boxPixelStride;

                    u64 color;
                    if (x % 2 == 0)
                        color = color1;
                    else
                        color = color2;

                    for (size_t boxY = 0; boxY < 8; boxY++)
                    {
                        for (size_t boxX = 0; boxX < 8; boxX++)
                        {
                            constexpr u64 pixelStride = 3U;
                            constexpr u64 rowStride = VideoConstants::GAMEBOY_SCREEN_WIDTH * pixelStride;
                            size_t realIndex = start + boxY * rowStride + boxX * pixelStride + hScroll * 3;

                            size_t rowBegin = y * boxRowStride + boxY * rowStride;
                            size_t rowEnd = rowBegin + rowStride - 1; 

                            if (realIndex > rowEnd)
                                realIndex -= rowStride;

                            realIndex += vScroll * rowStride;

                            constexpr u64 framebufferEnd = (
                                (
                                    VideoConstants::GAMEBOY_SCREEN_HEIGHT * 
                                    VideoConstants::GAMEBOY_SCREEN_WIDTH * 
                                    3U
                                ) - 
                                1
                            );

                            if (realIndex > framebufferEnd)
                                realIndex -= (framebufferEnd + 1);

                            std::copy(colorLut[color].begin(), colorLut[color].end(), framebuffer.begin() + realIndex);
                        }
                    }
                    color ^= 1;
                }
                std::swap(color1, color2);
            }
        };

        while (!m_Video.ShouldClose())
        {
            m_StartFrame = m_Timer.now();

            DrawChecker(hScroll, vScroll);
            m_Video.UpdateTexture(framebuffer);
            m_Video.Draw();

            hScroll = (hScroll + 1) % VideoConstants::GAMEBOY_SCREEN_WIDTH;
            vScroll = (vScroll + 1) % VideoConstants::GAMEBOY_SCREEN_HEIGHT;
            
            LimitFramerate(VideoConstants::GAMEBOY_REFRESH_RATE);
        }
    }
}