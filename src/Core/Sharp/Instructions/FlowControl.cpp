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

namespace GBcc
{
    void Sharp::Call(const bool bConditionMet) 
    {
        if (!bConditionMet)
            return;
        
        m_SP -= 2;
        m_pMemBus->WriteDoubleWord(m_SP, m_PC);
        m_PC = m_Operand.as16;

        m_CyclesTaken += 12ULL;
    }

    void Sharp::Jump(const bool bConditionMet, const bool bAddressInHL)
    {
        if (!bConditionMet)
            return;
        m_PC = bAddressInHL ? m_HL.GetDoubleWord() : m_Operand.as16;
        m_CyclesTaken += 4ULL;
    }

    void Sharp::JumpRelative(const bool bConditionMet)
    {
        if (!bConditionMet)
            return;
        const i8 offset = m_Operand.as8;
        m_PC += offset;
        m_CyclesTaken += 4ULL;
    }

    void Sharp::Return(const bool bConditionMet)
    {
        if (!bConditionMet)
            return;
        m_PC = m_pMemBus->ReadDoubleWord(m_SP);
        m_SP += 2;
        m_CyclesTaken += 12ULL;
    }

    void Sharp::ResetToVector(const u16 resetVector)
    {
        m_SP -= 2U;
        m_pMemBus->WriteDoubleWord(m_SP, m_PC);
        m_PC = resetVector;
    }
}