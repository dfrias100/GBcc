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
    void Sharp::AndAccumulator(const u8 value)
    {
        u8 currentAccumulator = m_A.GetValue();
        currentAccumulator &= value;

        bool isZero = currentAccumulator == 0U; 
        UpdateFlag(SharpFlags::ZERO, isZero);
        UpdateFlag(SharpFlags::NOT_ADD, false);
        UpdateFlag(SharpFlags::HALF, true);
        UpdateFlag(SharpFlags::CARRY, false);

        m_A.SetValue(currentAccumulator);
    }

    void Sharp::XorAccumulator(const u8 value)
    {        
        const u8 currentAccumulator = m_A.GetValue() ^ value;
        
        const bool isZero = currentAccumulator == 0U; 
        UpdateFlag(SharpFlags::ZERO, isZero);
        UpdateFlag(SharpFlags::NOT_ADD, false);
        UpdateFlag(SharpFlags::HALF, false);
        UpdateFlag(SharpFlags::CARRY, false);

        m_A.SetValue(currentAccumulator);
    }

    void Sharp::OrAccumulator(const u8 value)
    {
        const u8 currentAccumulator = m_A.GetValue() | value;

        const bool isZero = currentAccumulator == 0U; 
        UpdateFlag(SharpFlags::ZERO, isZero);
        UpdateFlag(SharpFlags::NOT_ADD, false);
        UpdateFlag(SharpFlags::HALF, false);
        UpdateFlag(SharpFlags::CARRY, false);

        m_A.SetValue(currentAccumulator);
    }

    void Sharp::RotateLeftAccumulator(const bool bCircular)
    {
        const u8 accumulatorRotated = RotateLeft(m_A.GetValue(), bCircular);
        m_A.SetValue(accumulatorRotated);
        ResetFlag(SharpFlags::ZERO);
    }

    void Sharp::RotateRightAccumulator(const bool bCircular)
    {
        const u8 accumulatorRotated = RotateRight(m_A.GetValue(), bCircular);
        m_A.SetValue(accumulatorRotated);
        ResetFlag(SharpFlags::ZERO);
    }

    void Sharp::DecimalAdjustAccumulator()
    {
        u8 adjustment = 0U;
        u8 currentAccumulator = m_A.GetValue();
        if (FlagIsSet(SharpFlags::NOT_ADD))
        {
            adjustment += FlagIsSet(SharpFlags::HALF)  ? 0x06U : 0U;
            adjustment += FlagIsSet(SharpFlags::CARRY) ? 0x60U : 0U;
            currentAccumulator -= adjustment;
        }
        else
        {
            if (
                FlagIsSet(SharpFlags::HALF) || 
                ((currentAccumulator & 0x0FU) > 0x09)
            ) {
                adjustment += 0x06U;
            }

            if (
                FlagIsSet(SharpFlags::CARRY) 
                || (currentAccumulator > 0x99)
            ) {
                adjustment += 0x60U;
                SetFlag(SharpFlags::CARRY);
            }
            else
            {
                ResetFlag(SharpFlags::CARRY);
            }

            currentAccumulator += adjustment;
        }

        const bool isZero = currentAccumulator == 0;
        UpdateFlag(SharpFlags::ZERO, isZero);
        ResetFlag(SharpFlags::HALF);
        m_A.SetValue(currentAccumulator);
    }

    void Sharp::AddRegisterWordToAccumulator(const ByteRegister& source)
    {
        const u8 operand = source.GetValue();
        const u8 result = UnsignedAddWord(m_A.GetValue(), operand);
        m_A.SetValue(result);
    }

    void Sharp::ComplementAccumulator()
    {
        const u8 accumulatorComplement = ~m_A.GetValue();
        m_A.SetValue(accumulatorComplement);
        SetFlag(SharpFlags::HALF);
        SetFlag(SharpFlags::NOT_ADD);
    }
}