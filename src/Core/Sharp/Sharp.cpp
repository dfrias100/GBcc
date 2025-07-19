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
#include "Utility.hpp"

#include "Core/Sharp/Sharp.hpp"
#include "Core/Memory.hpp"
#include "Core/Bitmasks.hpp"

#include <iostream>
#include <iomanip>


namespace GBcc 
{
    Sharp::Sharp(Memory* const pMemBus, std::function<void(u64)> syncCallback) : 
        m_AF(m_A, m_F),
        m_BC(m_B, m_C),
        m_DE(m_D, m_E),
        m_HL(m_H, m_L),
        m_pMemBus(pMemBus),
        Synchronize(syncCallback)
    {
        m_A.SetValue(0x01U);
        SetFlag(SharpFlags::ZERO);
        ResetFlag(SharpFlags::NOT_ADD);
        SetFlag(SharpFlags::HALF);
        SetFlag(SharpFlags::CARRY);

        m_BC.SetDoubleWord(0x0013);
        m_DE.SetDoubleWord(0x00D8);
        m_HL.SetDoubleWord(0x014D);
        m_PC = 0x0100;
        m_SP = 0xFFFE;
        m_ExecLog.open("execlog.txt");
    }

    void Sharp::DumpRegs()
    {
        std::array<u8, 4> memLog = { 0 };

        for (size_t i = 0; i < 4; i++)
            memLog[i] = m_pMemBus->ReadWord(m_PC + i);

        if (m_ExecLog)
        {
            m_ExecLog << std::hex << std::uppercase 
                <<  "A:"  << std::setw(2) << std::setfill('0') << (u16)m_A.GetValue() 
                << " F:"  << std::setw(2) << std::setfill('0') << (u16)m_F.GetValue() 
                << " B:"  << std::setw(2) << std::setfill('0') << (u16)m_B.GetValue() 
                << " C:"  << std::setw(2) << std::setfill('0') << (u16)m_C.GetValue() 
                << " D:"  << std::setw(2) << std::setfill('0') << (u16)m_D.GetValue() 
                << " E:"  << std::setw(2) << std::setfill('0') << (u16)m_E.GetValue() 
                << " H:"  << std::setw(2) << std::setfill('0') << (u16)m_H.GetValue() 
                << " L:"  << std::setw(2) << std::setfill('0') << (u16)m_L.GetValue() 
                << " SP:" << std::setw(4) << std::setfill('0') << m_SP
                << " PC:" << std::setw(4) << std::setfill('0') << m_PC << std::setw(2) 
                << " PCMEM:" << std::setw(2) << std::setfill('0') << (u16)memLog[0] << "," 
                             << std::setw(2) << std::setfill('0') << (u16)memLog[1] << "," 
                             << std::setw(2) << std::setfill('0') << (u16)memLog[2] << "," 
                             << std::setw(2) << std::setfill('0') << (u16)memLog[3] << std::endl;
        }
    }

    void Sharp::Step()
    {
        //DumpRegs();
        const u8 opcode = FetchInstruction();
    }
}