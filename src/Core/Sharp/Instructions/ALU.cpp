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
#include "Core/Bitmasks.hpp"

namespace GBcc
{
    u8 Sharp::UnsignedAddWord(const u8 lhs, const u8 rhs, const bool shouldAddCarry)
    {
        u8 carry = shouldAddCarry ? static_cast<u8>(FlagIsSet(SharpFlags::CARRY)) : 0U;
        u8 lhsLowNibble = lhs & GB_LOW_NIBBLE;
        u8 rhsLowNibble = rhs & GB_LOW_NIBBLE;

        u8 halfAdd = lhsLowNibble + rhsLowNibble + carry;
        bool shouldSetHalfCarry = TestBit(halfAdd, 4U);
        UpdateFlag(SharpFlags::HALF, shouldSetHalfCarry);

        u16 result16 = lhs + rhs + carry;
        bool shouldSetCarry = TestBit(result16, 8U);
        UpdateFlag(SharpFlags::CARRY, shouldSetCarry);

        u8 result = result16 & GB_LOW_BYTE;
        bool isZero = result == 0;
        UpdateFlag(SharpFlags::ZERO, isZero);

        UpdateFlag(SharpFlags::NOT_ADD, false);

        return result;
    }

    u8 Sharp::UnsignedSubtractWord(const u8 lhs, const u8 rhs, const bool shouldBorrow)
    {
        const u8 rhsModified = rhs + (
            shouldBorrow ?
            static_cast<u8>(FlagIsSet(SharpFlags::CARRY)) :
            0U
        );

        const u8 result = lhs - rhsModified;

        u8 lhsLowNibble = lhs & GB_LOW_NIBBLE;
        u8 rhsLowNibble = rhs & GB_LOW_NIBBLE;

        bool shouldSetHalfCarry = lhsLowNibble < rhsLowNibble;
        bool shouldSetCarry = lhs < rhs;

        const bool isCarrySet = FlagIsSet(SharpFlags::CARRY);

        
        if (shouldBorrow)
        {
            shouldSetHalfCarry = shouldSetHalfCarry || (isCarrySet && (lhsLowNibble == rhsLowNibble));
            shouldSetCarry = shouldSetCarry || (isCarrySet && (lhs == rhs));
        }

        UpdateFlag(SharpFlags::HALF, shouldSetHalfCarry);
        UpdateFlag(SharpFlags::CARRY, shouldSetCarry);

        bool isZero = result == 0;
        UpdateFlag(SharpFlags::ZERO, isZero);

        UpdateFlag(SharpFlags::NOT_ADD, true);

        return result;
    }

    void Sharp::DecrementRegisterWord(ByteRegister& reg)
    {
        const bool bCurrentCarry = FlagIsSet(SharpFlags::CARRY);

        const u8 currentRegisterValue = reg.GetValue();
        const u8 decrementedRegister = UnsignedSubtractWord(currentRegisterValue, 1U);

        reg.SetValue(decrementedRegister);
        
        UpdateFlag(SharpFlags::CARRY, bCurrentCarry);
    }

    void Sharp::IncrementRegisterWord(ByteRegister& reg)
    {
        const bool bCurrentCarry = FlagIsSet(SharpFlags::CARRY);

        const u8 currentRegisterValue = reg.GetValue();
        const u8 incrementedRegister = UnsignedAddWord(currentRegisterValue, 1U);
        
        reg.SetValue(incrementedRegister);

        ResetFlag(SharpFlags::NOT_ADD);
        UpdateFlag(SharpFlags::CARRY, bCurrentCarry); 
    }

    u16 Sharp::UnsignedAddDoubleWord(const u16 lhs, const u16 rhs)
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

    void Sharp::IncrementRegisterDoubleWord(SharpRegister& reg)
    {
        const u16 incrementedRegister = reg.GetDoubleWord() + 1U;
        reg.SetDoubleWord(incrementedRegister);
    }

    void Sharp::DecrementRegisterDoubleWord(SharpRegister& reg)
    {
        const u16 decrementedRegister = reg.GetDoubleWord() - 1U;
        reg.SetDoubleWord(decrementedRegister);
    }

    void Sharp::AddSignedWordToSP()
    {
        const i8 offset = m_Operand.as8;

        const u8 spLowNibble = m_SP & GB_LOW_NIBBLE;
        const u8 offsetLowNibble = offset & GB_LOW_NIBBLE;

        const u8 halfAdd = spLowNibble + offsetLowNibble;
        const bool shouldSetHalfCarry = TestBit(halfAdd, 4U);
        UpdateFlag(SharpFlags::HALF, shouldSetHalfCarry);

        const u8 spLowWord = m_SP & GB_LOW_BYTE;
        const u16 fullAdd = spLowWord + m_Operand.as8;
        const bool shouldSetCarry = TestBit(fullAdd, 8U);
        UpdateFlag(SharpFlags::CARRY, shouldSetCarry);
        
        UpdateFlag(SharpFlags::ZERO, false);
        UpdateFlag(SharpFlags::NOT_ADD, false);

        m_SP += offset;
    }

    void Sharp::ComplementCarry()
    {
        const bool carryComplement = !FlagIsSet(SharpFlags::CARRY);
        UpdateFlag(SharpFlags::CARRY, carryComplement);
        ResetFlag(SharpFlags::NOT_ADD);
        ResetFlag(SharpFlags::HALF);
    }

    void Sharp::SetCarry()
    {
        SetFlag(SharpFlags::CARRY);
        ResetFlag(SharpFlags::NOT_ADD);
        ResetFlag(SharpFlags::HALF);
    }
}