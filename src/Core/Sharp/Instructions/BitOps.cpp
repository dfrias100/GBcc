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

namespace GBcc
{
    u8 Sharp::ShiftLeftArithmetic(const u8 value)
    {
        const bool shouldSetCarry = TestBit(value, 7U);
        const u8 newValue = value << 1U;
        
        const bool shouldSetZero = newValue == 0U;

        UpdateFlag(SharpFlags::ZERO, shouldSetZero);
        ResetFlag(SharpFlags::NOT_ADD);
        ResetFlag(SharpFlags::HALF);
        UpdateFlag(SharpFlags::CARRY, shouldSetCarry);

        return newValue;
    }

    u8 Sharp::ShiftRightArithmetic(const u8 value)
    {
        const bool shouldSetCarry = TestBit(value, 0U);
        const u8 signBitMask = value & 0x80U;
        const u8 newValue = (value >> 1U) | signBitMask;

        const bool shouldSetZero = newValue == 0U;

        UpdateFlag(SharpFlags::ZERO, shouldSetZero);
        ResetFlag(SharpFlags::NOT_ADD);
        ResetFlag(SharpFlags::HALF);
        UpdateFlag(SharpFlags::CARRY, shouldSetCarry);

        return newValue;
    }

    u8 Sharp::ShiftRightLogical(const u8 value)
    {
        const bool shouldSetCarry = TestBit(value, 0U);
        const u8 newValue = value >> 1U;
        
        const bool shouldSetZero = newValue == 0U;

        UpdateFlag(SharpFlags::ZERO, shouldSetZero);
        ResetFlag(SharpFlags::NOT_ADD);
        ResetFlag(SharpFlags::HALF);
        UpdateFlag(SharpFlags::CARRY, shouldSetCarry);

        return newValue;
    }

    u8 Sharp::SwapNibbles(const u8 value)
    {
        const u8 newHighNibble = (value & 0x0'FU) << 4U;
        const u8 newLowNibble  = (value & 0xF'0U) >> 4U;

        const bool isZero = value == 0;
        UpdateFlag(SharpFlags::ZERO, isZero);
        ResetFlag(SharpFlags::NOT_ADD);
        ResetFlag(SharpFlags::HALF);
        ResetFlag(SharpFlags::CARRY);

        return newHighNibble | newLowNibble;
    }

    void Sharp::BitInstruction(const u8 index, const u8 value)
    {
        bool shouldSetZero = !TestBit(value, index);
        UpdateFlag(SharpFlags::ZERO, shouldSetZero);
        ResetFlag(SharpFlags::NOT_ADD);
        SetFlag(SharpFlags::HALF);
    }

        u8 Sharp::ResetBit(const u8 index, const u8 value)
    {
        const u8 bitMask = ~(0x1U << index);
        const u8 newValue = value & bitMask;

        return newValue;
    }

    u8 Sharp::SetBit(const u8 index, const u8 value)
    {
        const u8 bitMask = 0x1U << index;
        const u8 newValue = value | bitMask;

        return newValue;
    }

    u8 Sharp::RotateLeft(const u8 value, const bool bCircular)
    {
        const u8 outBit = (value & 0b1000'0000) >> 7U;
        const u8 inBit  = bCircular ? outBit : FlagIsSet(SharpFlags::CARRY);
        
        u8 newValue = value << 1U;
        newValue |= inBit;

        const bool isZero = newValue == 0; 

        UpdateFlag(SharpFlags::ZERO, isZero);
        ResetFlag(SharpFlags::NOT_ADD);
        ResetFlag(SharpFlags::HALF);
        UpdateFlag(SharpFlags::CARRY, outBit);

        return newValue;
    }

    u8 Sharp::RotateRight(const u8 value, const bool bCircular)
    {
        const u8 outBit = value & 0b1;
        const u8 inBit = bCircular ? outBit : FlagIsSet(SharpFlags::CARRY);

        u8 newValue = value >> 1U;
        newValue |= (inBit << 7U);

        const bool isZero = newValue == 0; 

        UpdateFlag(SharpFlags::ZERO, isZero);
        ResetFlag(SharpFlags::NOT_ADD);
        ResetFlag(SharpFlags::HALF);
        UpdateFlag(SharpFlags::CARRY, outBit);

        return newValue;
    }
}