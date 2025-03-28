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

#include "Core/Memory.hpp"

#include <string>
#include <fstream>
#include <iostream>

namespace GBcc
{
    Memory::Memory()
    {
        std::string testRomPath = "../roms/cpu_instrs/01-special.gb";
        std::ifstream testRomFile(testRomPath, std::ios::binary);

        testRomFile.read(
            reinterpret_cast<char *>(m_BootRom.data()),
            m_Rom.size()
        );
    }

    u8 Memory::ReadWord(const u16 address)
    {
        u16 trueAddress;
        if (address <= GB_BOOTROM_SIZE)
        {
            if (m_BootRomEnable)
            {
                return m_BootRom[address];
            }
            else
            {
                return m_Rom[address];
            }
        }
        else if (address <= GB_CART_SPACE_END)
        {
            return m_Rom[address];
        }
        else if (address >= 0xC000U && address <= 0xDFFFU)
        {
            trueAddress = address - 0xC000U;
            return m_WorkRam[trueAddress];
        }
        else if (address <= 0xFDFF)
        {
            trueAddress = address - 0xE000U;
            return m_WorkRam[trueAddress];
        }
        else if (address >= 0xFF00 && address <= 0xFF7F)
        {
            if (address == 0xFF01)
            {
                return m_SerialRegs[0];
            }
            else if (address == 0xFF02)
            {
                return m_SerialRegs[1];
            }
        }
    }

    u16 Memory::ReadDoubleWord(const u16 address)
    {
        return (
            (static_cast<u16>(ReadWord(address + 1U)) << 8U) | 
            ReadWord(address)
        );
    }

    void Memory::WriteWord(const u16 address, const u8 data)
    {
        u16 trueAddress;
        if (address >= 0xC000U && address <= 0xDFFFU)
        {
            trueAddress = address - 0xC000U;
            m_WorkRam[trueAddress] = data;
        }
        else if (address <= 0xFDFF)
        {
            trueAddress = address - 0xE000U;
            m_WorkRam[trueAddress] = data;
        }
        else if (address >= 0xFF00 && address <= 0xFF7F)
        {
            if (address == 0xFF50)
            {
                m_BootRomEnable = false;
            }
            else if (address == 0xFF01)
            {
                m_SerialRegs[0] = data;
            }
            else if (address == 0xFF02 && data == 0x81)
            {
                char c = m_SerialRegs[0];
                std::cout << c;
            }
        }
    }

    void Memory::WriteDoubleWord(const u16 address, const u16 data)
    {
        WriteWord(address    , (data & 0x00FF));
        WriteWord(address + 1, (data & 0xFF00) >> 8U);
    }
}