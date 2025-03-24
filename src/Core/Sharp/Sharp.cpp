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
#include "Core/Sharp/Sharp.hpp"
#include "Core/Memory.hpp"
#include "Core/Bitmasks.hpp"

#include <iostream>

namespace GBcc {
    Sharp::Sharp(Memory* const& pMemBus) : 
        m_AF(m_A, m_F),
        m_BC(m_B, m_C),
        m_DE(m_D, m_E),
        m_HL(m_H, m_L),
        m_pMemBus(pMemBus)
    {}

    template <typename T>
    bool Sharp::TestBit(const T& val, const size_t bitIndex) const
    {
        if (bitIndex > ((sizeof(T) * 8U) - 1))
        {
            return;
        } 

        return (val & (1U << bitIndex));
    }

    inline void Sharp::SetFlag(const SharpFlags& flag)
    {
        m_F.SetBit(static_cast<size_t>(flag));
    }

    inline void Sharp::ResetFlag(const SharpFlags& flag)
    {
        m_F.ResetBit(static_cast<size_t>(flag));
    }

    inline void Sharp::UpdateFlag(const SharpFlags& flag, const bool& set)
    {
        if (set)
        {
            SetFlag(flag);
        }
        else
        {
            ResetFlag(flag);
        }
    }

    void Sharp::RegisterToRegisterWord(const ByteRegister& source, ByteRegister& destination)    
    {
        auto val = source.GetValue();
        destination.SetValue(val);
    }
    
    u8 Sharp::UnsignedAddWord(const u8& lhs, const u8& rhs)
    {
        u8 lhsLowNibble = lhs & GB_LOW_NIBBLE;
        u8 rhsLowNibble = rhs & GB_LOW_NIBBLE;

        u8 halfAdd = lhsLowNibble + rhsLowNibble;
        bool shouldSetHalfCarry = TestBit(halfAdd, 4U);
        UpdateFlag(SharpFlags::HALF, shouldSetHalfCarry);

        u16 result16 = lhs + rhs;
        bool shouldSetCarry = TestBit(result16, 8U);
        UpdateFlag(SharpFlags::CARRY, shouldSetCarry);

        u8 result = result16 & GB_LOW_BYTE;
        bool isZero = result == 0;
        UpdateFlag(SharpFlags::ZERO, isZero);

        UpdateFlag(SharpFlags::NOT_ADD, false);

        return result;
    }

    u16 Sharp::UnsignedAddDoubleWord(const u16& lhs, const u16& rhs)
    {
        u16 lhsMasked = lhs & 0x0F'FFU;
        u16 rhsMasked = rhs & 0x0F'FFU;
        
        u16 halfAdd = lhsMasked + rhsMasked;
        bool shouldSetHalfCarry = TestBit(halfAdd, 12U);
        UpdateFlag(SharpFlags::HALF, shouldSetHalfCarry);

        u32 result32 = lhs + rhs;
        bool shouldSetCarry = TestBit(result32, 16U);
        UpdateFlag(SharpFlags::CARRY, shouldSetCarry);

        u16 result = result32 & U32_LOW_HALF;

        UpdateFlag(SharpFlags::NOT_ADD, false);

        return result;
    }

    void Sharp::AddRegisterToAccumulator(const ByteRegister& source)
    {
        const u8 operand = source.GetValue();
        const u8 result = UnsignedAddWord(m_A.GetValue(), operand);
        m_A.SetValue(result);
    }

    void Sharp::LoadValueToRegisterWord(ByteRegister& destination)
    {
        const u8 value = m_Operand & GB_LOW_BYTE;
        destination.SetValue(value);
    }

    void Sharp::LoadAddressToRegisterWord(
        SharpRegister& addressSource,
        ByteRegister& destination,
        const PointerOperation ptrOp,
        const bool addressIsImmediate = false
    )
    {
        u16 address = (
            addressIsImmediate ? 
            m_Operand : addressSource.GetDoubleWord()
        );

        const u8 val = m_pMemBus->ReadWord(address);
        destination.SetValue(val);

        if (ptrOp == PointerOperation::Increment)
        {
            address++;
            addressSource.SetDoubleWord(address);
        }
        else if (ptrOp == PointerOperation::Decrement)
        {
            address--;
            addressSource.SetDoubleWord(address);
        }
    }

    void Sharp::StoreRegisterToMemoryWord(
        SharpRegister& addressDestination,
        const ByteRegister& source, 
        const PointerOperation ptrOp, 
        const bool addressIsImmediate = false
    )
    {
        u16 address = (
            addressIsImmediate ? 
            m_Operand : addressDestination.GetDoubleWord()
        );

        const u8 val = source.GetValue();
        m_pMemBus->WriteWord(address, val);

        if (ptrOp == PointerOperation::Increment)
        {
            address++;
            addressDestination.SetDoubleWord(address);
        }
        else if (ptrOp == PointerOperation::Decrement)
        {
            address--;
            addressDestination.SetDoubleWord(address);
        }
    }
}