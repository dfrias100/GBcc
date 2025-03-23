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
    ByteRegister::ByteRegister() : m_Value(0) {}
    ByteRegister::ByteRegister(const u8& initializerValue) : m_Value(initializerValue) {}
    
    ByteRegister::ByteRegister(const ByteRegister& reg)
    {
        this->m_Value = reg.m_Value;
    }

    ByteRegister& ByteRegister::operator=(const ByteRegister& reg)
    {
        this->m_Value = reg.m_Value;
        return *this;    
    }

    void ByteRegister::SetValue(const u8& value)
    {
        m_Value = value;
    }

    u8 ByteRegister::GetValue() const
    {
        return m_Value;
    }

    void ByteRegister::SetBit(size_t bitIndex)
    {
        if (bitIndex > 7) 
        {
            return;
        }

        m_Value |= (1U << bitIndex);
    }

    void ByteRegister::ResetBit(size_t bitIndex)
    {
        if (bitIndex > 7) 
        {
            return;
        }

        m_Value &= ~(1U << bitIndex);
    }

    bool ByteRegister::BitIsSet(size_t bitIndex) const
    {
        return (m_Value & (1U << bitIndex));
    }

    SharpRegister::SharpRegister(ByteRegister& highRegister, ByteRegister& lowRegister) 
        : m_HighRegister(highRegister), m_LowRegister(lowRegister) {};
        
    u16 SharpRegister::GetDoubleWord() const
    {
        return (
            (static_cast<u16>(m_HighRegister.GetValue()) << 8U) |
            m_LowRegister.GetValue()
        );
    }

    void SharpRegister::SetDoubleWord(const u16& doubleWord)
    {
        m_HighRegister.SetValue((doubleWord & GB_HIGH_BYTE) >> 8U);
        m_LowRegister.SetValue(doubleWord & GB_LOW_BYTE);
    }
}
