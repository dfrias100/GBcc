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

namespace GBcc
{
    void Sharp::DecodeBlock0(const u8 opcode)
    {
        if (opcode == GB_INSTR_NOP_OPCODE) [[unlikely]]
        {
            return;
        }

        const u8 zIndex = GetValueFromMask(opcode, GB_Z_INDEX_MASK);

        switch (zIndex)
        {
            case GB_INSTR_BLOCK_REL_JMP:
                HandleRelJumpMisc(opcode);
                break;
            case GB_INSTR_BLOCK_LDD_IMM:
                HandleLoadDouble(opcode);
                break;
            case GB_INSTR_BLOCK_LDW_IND:
                HandleLoadStoreIndirect(opcode);
                break;
            case GB_INSTR_BLOCK_INC_DEC_DW:
            case GB_INSTR_BLOCK_INC_WRD:
            case GB_INSTR_BLOCK_DEC_WRD:
                HandleIncrementDecrement(opcode, zIndex);
                break;
            case GB_INSTR_BLOCK_LDW_IMM:
                LoadImmediateWord(opcode);
                break;
            case GB_INSTR_BLOCK_ACC_MISC:
                HandleMiscAccumulator(opcode);
                break;
        }
    }

    void Sharp::DecodeBlock1(const u8 opcode)
    {
        if (opcode == GB_INSTR_HALT_OPCODE) [[unlikely]]
        {
            // Halt not implemented right now
            return;
        }

        // ============ 8-bit register-to-register loads and HL load/store ============

        const u8 sourceRegisterIndex = GetValueFromMask(opcode, GB_Z_INDEX_MASK);
        const u8 destinationRegisterIndex = GetValueFromMask(opcode, GB_Y_INDEX_MASK);

        if (sourceRegisterIndex == GB_CPU_DEREF_HL_PTR)
        {
            FetchHL();
        }
        
        auto& sourceRegister = GetRegisterFromIndex(sourceRegisterIndex);
        auto& destinationRegister = GetRegisterFromIndex(destinationRegisterIndex);

        RegisterToRegisterWord(sourceRegister, destinationRegister);

        if (destinationRegisterIndex == GB_CPU_DEREF_HL_PTR)
        {
            WriteHL();
        }
    }

    void Sharp::DecodeBlock2(const u8 opcode)
    {
        // 8-bit Arithmetic Logic Unit operations on Accumulator with Registers as Operands
        const u8 registerOperandIndex = GetValueFromMask(opcode, GB_Z_INDEX_MASK);
        const u8 aluOperation = GetValueFromMask(opcode, GB_Y_INDEX_MASK);

        if (registerOperandIndex == GB_CPU_DEREF_HL_PTR)
        {
            FetchHL();
        }

        const auto& registerOperand = GetRegisterFromIndex(registerOperandIndex);
        m_Operand.as8 = registerOperand.GetValue();
        HandleAccumulatorALU(aluOperation);
    }

    void Sharp::DecodeBlock3(const u8 opcode)
    {
        const u8 zIndex = GetValueFromMask(opcode, GB_Z_INDEX_MASK);
        const u8 yIndex = GetValueFromMask(opcode, GB_Y_INDEX_MASK);

        switch (zIndex)
        {
            case GB_INSTR_CRET_MMLD_STACK:
            {
                if (yIndex <= GB_CPU_MAX_COND_CODE)
                {
                    const bool bCondtionMet = EvaluateCondition(yIndex);
                    Return(bCondtionMet);
                }
                else 
                {
                    HandleIOLoadAndStackALU(yIndex);
                }
            }
                break;
            case GB_INSTR_POP_MISC:
                HandlePopMisc(yIndex);
                break;
            case GB_INSTR_COND_JUMP:
                HandleAbsoluteJump(yIndex);
                break;
            case GB_INSTR_MISC_OPS:
                {
                    if (yIndex == 0)
                    {
                        FetchDoubleWord();
                        Jump(true, false);
                    }
                    else if (yIndex == 6)
                    {
                        // TODO: Disable interrupts
                    }
                    else if (yIndex == 7)
                    {
                        // TODO: Enable interrupts
                    }
                    else
                    {
                        std::cerr << "Invalid opcode! Got " 
                            << std::showbase << std::hex << (u16) opcode << " @ PC = " 
                            << std::showbase << std::hex << m_PC - 1U << ". Quitting." << std::endl;
                        exit(-1);
                    }
                }
                break;
            case GB_INSTR_COND_CALL:
                {
                    if(yIndex <= GB_CPU_MAX_COND_CODE)
                    {
                        FetchDoubleWord();
                        const bool bConditionMet = EvaluateCondition(yIndex);
                        Call(bConditionMet);
                    }
                    else
                    {
                        std::cerr << "Invalid opcode! Got " 
                            << std::showbase << std::hex << (u16) opcode << " @ PC = " 
                            << std::showbase << std::hex << m_PC - 1U << ". Quitting." << std::endl;
                        exit(-1);
                    }
                }
                break;
            case GB_INSTR_PUSH_MISC:
                {
                    const u8 qIndex = GetValueFromMask(opcode, GB_Q_INDEX_MASK);
                    const u8 pIndex = GetValueFromMask(opcode, GB_P_INDEX_MASK);
                    if (qIndex == 0)
                    {
                        const auto& registersToPush = GetRegisterPairFromIndex(pIndex);
                        PushRegisters(registersToPush);
                    }
                    else if (pIndex == 0)
                    {
                        FetchDoubleWord();
                        Call();
                    }
                    else
                    {
                        std::cerr << "Invalid opcode! Got " 
                            << std::showbase << std::hex << (u16) opcode << " @ PC = " 
                            << std::showbase << std::hex << m_PC - 1U << ". Quitting." << std::endl;
                        exit(-1);
                    }
                }
                break;
            case GB_INSTR_ALU_IMM:
                FetchWord();
                HandleAccumulatorALU(yIndex);
                break;
            case GB_INSTR_RESET:
                ResetToVector((u16)yIndex * 8U);
                break;
        }
    }

    void Sharp::DecodePrefixCB(const u8 opcode)
    {
        const u8 registerSourceIndex = GetValueFromMask(opcode, GB_Z_INDEX_MASK);
        const u8 xIndex = GetValueFromMask(opcode, GB_INSTR_BLOCK_MASK);

        if (registerSourceIndex == GB_CPU_DEREF_HL_PTR)
        {
            FetchHL();
        }

        auto& registerSource = GetRegisterFromIndex(registerSourceIndex);
        const u8 bitIndex = GetValueFromMask(opcode, GB_Y_INDEX_MASK);

        switch (xIndex)
        {
            case 0:
                RotateShiftHelper(registerSource, bitIndex);
                break;
            case 1:
                BitInstruction(bitIndex, registerSource.GetValue());
                return;
            case 2:
                registerSource.SetValue(
                    ResetBit(bitIndex, registerSource.GetValue())
                );
                break;
            case 3:
                registerSource.SetValue(
                    SetBit(bitIndex, registerSource.GetValue())
                );
                break;
        }

        if (registerSourceIndex == GB_CPU_DEREF_HL_PTR)
        {
            WriteHL();
        }
    }
}