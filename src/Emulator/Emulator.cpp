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
        std::array<uint8_t, 160U * 144U * 3U> framebuffer;
        auto gen_static = [&framebuffer]() {
            for (size_t i = 0; i < framebuffer.size(); i += 3)
            {
                uint8_t col = (rand() % 4) * 85;
                std::fill(framebuffer.begin() + i, framebuffer.begin() + i + 3, col);
            }
        };

        while (!m_Video.ShouldClose())
        {
            m_StartFrame = m_Timer.now();

            gen_static();
            m_Video.UpdateTexture(framebuffer);
            m_Video.Draw();

            CapFramerate(VideoConstants::GAMEBOY_REFRESH_RATE);
        }
    }
}