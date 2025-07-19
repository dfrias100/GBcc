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
        std::string testRomPath = "../roms/cpu_instrs/individual/01-special.gb";
        std::ifstream testRomFile(testRomPath, std::ios::binary);

        testRomFile.read(
            reinterpret_cast<char *>(m_Rom.data()),
            m_Rom.size()
        );

        std::string bootRomPath = "../roms/dmg_boot.bin";
        std::ifstream bootRomFile(bootRomPath, std::ios::binary);

        bootRomFile.read(
            reinterpret_cast<char *>(m_BootRom.data()),
            m_BootRom.size()
        );
        m_BootRomEnable = false;

        std::fill(m_HighRam.begin(), m_HighRam.end(), 0x00U);
        std::fill(m_WorkRam.begin(), m_WorkRam.end(), 0x00U);
        std::fill(m_VideoRam.begin(), m_VideoRam.end(), 0x00U);
    }

    u8 Memory::ReadWord(const u16 address)
    {
        u16 trueAddress;
        if (address < GB_BOOTROM_SIZE)
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
        else if (address >= 0x8000U && address <= 0x9FFFU)
        {
            trueAddress = address - 0x8000U;
            return m_VideoRam[trueAddress];
        }
        else if (address >= 0xC000U && address <= 0xFDFFU)
        {
            trueAddress = address - 0xC000U - (address >= 0xE000 ? 0x2000U : 0U);
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
            else if (address == 0xFF44)
            {
                return 0x90;
            }
            else
            {
                trueAddress = address - 0xFF00;
                return m_IO_Ram[trueAddress];
            }
        }
        else if (address >= 0xFF80 && address <= 0xFFFE)
        {
            trueAddress = address - 0xFF80;
            return m_HighRam[trueAddress];
        }

        return 0;
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
        if (address >= 0x8000U && address <= 0x9FFFU)
        {
            trueAddress = address - 0x8000U;
            m_VideoRam[trueAddress] = data;
        } 
        else if (address >= 0xC000U && address <= 0xFDFFU)
        {
            trueAddress = address - 0xC000U - (address >= 0xE000 ? 0x2000U : 0U);
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
            else
            {
                trueAddress = address - 0xFF00;
                m_IO_Ram[trueAddress] = data;
            }
        }
        else if (address >= 0xFF80 && address <= 0xFFFE)
        {
            trueAddress = address - 0xFF80;
            m_HighRam[trueAddress] = data;
        }
    }

    void Memory::WriteDoubleWord(const u16 address, const u16 data)
    {
        WriteWord(address    , (data & 0x00FF));
        WriteWord(address + 1, (data & 0xFF00) >> 8U);
    }
}