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
    Sharp::Sharp(Memory* const pMemBus) : 
        m_AF(m_A, m_F),
        m_BC(m_B, m_C),
        m_DE(m_D, m_E),
        m_HL(m_H, m_L),
        m_pMemBus(pMemBus)
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
            m_ExecLog << std::hex << std::uppercase << "A:"  << std::setw(2) << std::setfill('0') << (u16)m_A.GetValue() 
											 << " F:"  << std::setw(2) << std::setfill('0') << (u16)m_F.GetValue() 
											 << " B:"  << std::setw(2) << std::setfill('0') << (u16)m_B.GetValue() 
											 << " C:"  << std::setw(2) << std::setfill('0') << (u16)m_C.GetValue() 
											 << " D:"  << std::setw(2) << std::setfill('0') << (u16)m_D.GetValue() 
											 << " E:"  << std::setw(2) << std::setfill('0') << (u16)m_E.GetValue() 
											 << " H:"  << std::setw(2) << std::setfill('0') << (u16)m_H.GetValue() 
											 << " L:"  << std::setw(2) << std::setfill('0') << (u16)m_L.GetValue() 
											 << " SP:" << std::setw(4) << std::setfill('0') << m_SP
										     << " PC:" << std::setw(4) << std::setfill('0') << m_PC << std::setw(2) << " PCMEM:" 
													   << std::setw(2) << std::setfill('0') << (u16)memLog[0] << "," 
													   << std::setw(2) << std::setfill('0') << (u16)memLog[1] << "," 
													   << std::setw(2) << std::setfill('0') << (u16)memLog[2] << "," 
													   << std::setw(2) << std::setfill('0') << (u16)memLog[3] << std::endl;
        }
    }

    void Sharp::FetchWord()
    {
        m_Operand.as8 = m_pMemBus->ReadWord(m_PC++);
    }

    void Sharp::FetchDoubleWord()
    {
        m_Operand.as16 = m_pMemBus->ReadDoubleWord(m_PC);
        m_PC += 2;
    }

    void Sharp::FetchHL() 
    {
        const u16 address = m_HL.GetDoubleWord();
        const u8 fetchedVal = m_pMemBus->ReadWord(address);
        m_HL_Memory.SetValue(fetchedVal);
    }

    void Sharp::WriteHL() 
    {
        const u16 address = m_HL.GetDoubleWord();
        const u8 valToWrite = m_HL_Memory.GetValue();
        m_pMemBus->WriteWord(address, valToWrite);
    }

    ByteRegister& Sharp::GetRegisterFromIndex(const u8 index)
    {
        switch (index)
        {
            case 0:
                return m_B;
            case 1:
                return m_C;
            case 2:
                return m_D;
            case 3:
                return m_E;
            case 4:
                return m_H;
            case 5:
                return m_L;
            case 6:
                return m_HL_Memory;
            case 7:
                return m_A;
            default:
                std::cerr << "Invalid register index specified, cannot return any register! Index supplied: " << index << std::endl;
                exit(-1);
        }
    }

    SharpRegister& Sharp::GetRegisterPairFromIndex(const u8 index)
    {
        switch (index)
        {
            case 0:
                return m_BC;
            case 1:
                return m_DE;
            case 2:
                return m_HL;
            case 3:
                return m_AF;
            default:
                std::cerr << "Invalid register index specified, cannot return any register! Index supplied: " << index << std::endl;
                exit(-1);
        }
    }

    void Sharp::ExecuteOpcode(const u8 opcode)
    {
        if (opcode == GB_INSTR_PREFIX_CB)
        {
            FetchWord();
            DecodePrefixCB(m_Operand.as8);
            return;
        }

        const u8 blockNum = GetValueFromMask(opcode, GB_INSTR_BLOCK_MASK);
        switch (blockNum)
        {
            case 0:
                DecodeBlock0(opcode);
                break;
            case 1:
                DecodeBlock1(opcode);
                break;
            case 2:
                DecodeBlock2(opcode);
                break;
            case 3:
                DecodeBlock3(opcode);
                break;
        }
    }

    u64 Sharp::Step()
    {
        //DumpRegs();
        const u8 opcode = m_pMemBus->ReadWord(m_PC++);
        ExecuteOpcode(opcode);
        return 0;
    }
}