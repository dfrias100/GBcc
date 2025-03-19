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
#include "Core/Sharp/SharpRegister.hpp"
#include "Core/Bitmasks.hpp"

#include <utility>

namespace GBcc 
{
    SharpRegister::SharpRegister() : m_HighByte(0), m_LowByte(0) {};

    SharpRegister::SharpRegister(const u16& doubleWord)
    {
        m_HighByte = (doubleWord & GB_HIGH_BYTE) >> 8U;
        m_LowByte  =  doubleWord & GB_LOW_BYTE;
    };

    SharpRegister::SharpRegister(const u8& highByte, const u8& lowByte) 
        : m_HighByte(highByte), m_LowByte(lowByte) {};

    SharpRegister::SharpRegister(const SharpRegister& reg)
        : m_HighByte(reg.m_HighByte), m_LowByte(reg.m_LowByte) {};
        
    SharpRegister& SharpRegister::operator=(const SharpRegister& reg)
    {
        this->m_HighByte = reg.m_HighByte;
        this->m_LowByte = reg.m_LowByte;
        
        return *this;
    }

    SharpRegister& SharpRegister::operator=(SharpRegister&& reg)
    {
        this->m_HighByte = std::move(reg.m_HighByte);
        this->m_LowByte  = std::move(reg.m_LowByte);

        return *this;
    }

    u16 SharpRegister::GetDoubleWord()
    {
        return (static_cast<u16>(m_HighByte) << 8U) | (m_LowByte);
    }

    u8 SharpRegister::GetWord(const bool highByte)
    {
        return highByte ? m_HighByte : m_LowByte;
    }

    void SharpRegister::SetDoubleWord(const u16& word)
    {
        m_HighByte = (word & GB_HIGH_BYTE) >> 8U;
        m_LowByte  =  word & GB_LOW_BYTE;
    }

    void SharpRegister::SetWord(const bool highByte, const u8& word)
    {
        if (highByte)
        {
            m_HighByte = word;
        }
        else
        {
            m_LowByte = word;
        }
    }

    void SharpFlagsRegister::SetFlag(const SharpFlags flag, const bool raise)
    {
        if (raise)
        {
            m_LowByte |= static_cast<u8>(flag);
        }
        else
        {
            m_LowByte &= ~static_cast<u8>(flag);
        }
    }

    bool SharpFlagsRegister::FlagIsRaised(const SharpFlags flag)
    {
        return m_LowByte & static_cast<u8>(flag);
    }

    void SharpFlagsRegister::SetWord(const bool highByte, const u8& word)
    {
        if (highByte)
        {
            m_HighByte = word;
        }
        else
        {
            m_LowByte = word & 0xF0;
        }
    }

    void SharpFlagsRegister::SetDoubleWord(const u16& doubleWord)
    {
        m_HighByte = (doubleWord & GB_HIGH_BYTE) >> 8U;
        m_LowByte =   doubleWord & 0xF0;
    }
}
