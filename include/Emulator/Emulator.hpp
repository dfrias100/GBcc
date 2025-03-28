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
#include "Video/Video.hpp"
#include "Types.hpp"

#include <chrono>

namespace GBcc {
    class Emulator
    {
        private:
        Video& m_Video;

        std::chrono::steady_clock m_Timer;
        std::chrono::time_point<std::chrono::steady_clock> m_StartFrame;

        void LimitFramerate(const float fps);

        public:
        Emulator();
        ~Emulator();
        
        void Run();
    };
}