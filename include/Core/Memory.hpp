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
#include <array>

#include "Types.hpp"
#include "SystemConstants.hpp"

namespace GBcc {
    class Memory
    {
        private:
        std::array<u8, GB_BOOTROM_SIZE> m_BootRom;
        std::array<u8, 32U * 1024U> m_Rom;

        std::array<u8, 8U * 1024U> m_WorkRam;
        std::array<u8, 127U> m_HighRam;

        std::array<u8, 8U * 1024U> m_VideoRam; // Temporary

        bool m_BootRomEnable = true;

        std::array<u8, 2U> m_SerialRegs;

        public:
        Memory();
        ~Memory() = default;

        void WriteWord(const u16 address, const u8 data);
        void WriteDoubleWord(const u16 address, const u16 data);

        u8 ReadWord(const u16 address);
        u16 ReadDoubleWord(const u16 address);
    };
}
