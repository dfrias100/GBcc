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

namespace GBcc
{
    void Sharp::HandleRelJumpMisc(const u8 opcode)
    {
        const u8 yIndex = GetValueFromMask(opcode, GB_Y_INDEX_MASK);

        if (yIndex == GB_INSTR_STR_SP_IMM_PTR)
        {
            FetchDoubleWord();
            m_pMemBus->WriteDoubleWord(m_Operand.as16, m_SP);
        }
        else if (
            (yIndex >= GB_INSTR_JMP_REL_MIN) && 
            (yIndex <= GB_INSTR_JMP_REL_MAX)
        ) {
            FetchWord();
            const i8 conditionCode = (i8) yIndex - 4;
            const bool bConditionMet = EvaluateCondition(conditionCode);
            JumpRelative(bConditionMet);
        }
    }

    void Sharp::HandleIncrementDecrement(const u8 opcode, const u8 zIndex)
    {
        if (zIndex == GB_INSTR_BLOCK_INC_DEC_DW)
        {
            const bool decrement = GetValueFromMask(opcode, GB_Q_INDEX_MASK);
            const u8 registerIndex = GetValueFromMask(opcode, GB_P_INDEX_MASK);

            if (registerIndex == GB_CPU_REGISTER_SP)
            {
                m_SP += decrement ? -1 : 1;
                return;
            }

            auto& registerPair = GetRegisterPairFromIndex(registerIndex);

            if (decrement)
            {
                DecrementRegisterDoubleWord(registerPair);
            }
            else 
            {
                IncrementRegisterDoubleWord(registerPair);
            }
        }
        else 
        {
            const u8 registerIndex = GetValueFromMask(opcode, GB_Y_INDEX_MASK);

            if (registerIndex == GB_CPU_DEREF_HL_PTR)
            {
                FetchHL();
            }

            auto& cpuRegister = GetRegisterFromIndex(registerIndex);
            const bool decrement = zIndex & 1;

            if (decrement)
            {
                DecrementRegisterWord(cpuRegister);
            } 
            else
            {
                IncrementRegisterWord(cpuRegister);
            }

            if (registerIndex == GB_CPU_DEREF_HL_PTR)
            {
                WriteHL();
            }
        }
    }

    void Sharp::HandleLoadStoreIndirect(const u8 opcode)
    {
        const u8 pIndex = GetValueFromMask(opcode, GB_P_INDEX_MASK);
        const u8 qIndex = GetValueFromMask(opcode, GB_Q_INDEX_MASK);

        PointerOperation ptrOp;
        SharpRegister* pRegisterOperand;

        switch (pIndex)
        {
            case 0:
                ptrOp = PointerOperation::Nothing;
                pRegisterOperand = &m_BC;
                break;
            case 1:
                ptrOp = PointerOperation::Nothing;
                pRegisterOperand = &m_DE;
                break;
            case 2:
                ptrOp = PointerOperation::Increment;
                pRegisterOperand = &m_HL;
                break;
            case 3:
                ptrOp = PointerOperation::Decrement;
                pRegisterOperand = &m_HL;
                break;
        }

        if (qIndex == 0)
        {
            StoreWordToMemory(pRegisterOperand, m_A, ptrOp);
        }
        else
        {
            LoadWordFromAddress(pRegisterOperand, m_A, ptrOp);
        }
    }

    void Sharp::HandleLoadDouble(const u8 opcode)
    {
        const u8 qIndex = GetValueFromMask(opcode, GB_Q_INDEX_MASK);
        const u8 registerPairIndex = GetValueFromMask(opcode, GB_P_INDEX_MASK);

        if (qIndex == 0)
        {
            FetchDoubleWord();

            if (registerPairIndex == GB_CPU_REGISTER_SP)
            {
                m_SP = m_Operand.as16;
                return;
            }

            auto& registerPair = GetRegisterPairFromIndex(registerPairIndex);
            registerPair.SetDoubleWord(m_Operand.as16);
        }
        else
        {
            const u16 currentHL = m_HL.GetDoubleWord();

            if (registerPairIndex == GB_CPU_REGISTER_SP)
            {
                m_Operand.as16 = m_SP;
            }
            else
            {
                auto& registerPair = GetRegisterPairFromIndex(registerPairIndex);
                m_Operand.as16 = registerPair.GetDoubleWord();
            }

            const u16 result = UnsignedAddDoubleWord(currentHL, m_Operand.as16);
            m_HL.SetDoubleWord(result);
        }
    }

    void Sharp::LoadImmediateWord(const u8 opcode)
    {
        FetchWord();
        const u8 registerIndex = GetValueFromMask(opcode, GB_Y_INDEX_MASK);

        if (registerIndex == GB_CPU_DEREF_HL_PTR)
        {
            FetchHL();
        }

        auto& destinationRegister = GetRegisterFromIndex(registerIndex);
        destinationRegister.SetValue(m_Operand.as8);

        if (registerIndex == GB_CPU_DEREF_HL_PTR)
        {
            WriteHL();
        }
    }

    void Sharp::HandleMiscAccumulator(const u8 opcode) 
    {
        const u8 accumulatorOp = GetValueFromMask(opcode, GB_Y_INDEX_MASK);

        switch (accumulatorOp)
        {
            case 0:
                RotateLeftAccumulator(true);
                break;
            case 1:
                RotateRightAccumulator(true);
                break;
            case 2:
                RotateLeftAccumulator(false);
                break;
            case 3:
                RotateRightAccumulator(false);
                break;
            case 4:
                DecimalAdjustAccumulator();
                break;
            case 5:
                ComplementAccumulator();
                break;
            case 6:
                SetCarry();
                break;
            case 7:
                ComplementCarry();
                break;
        }
    }
    
    void Sharp::HandleIOLoadAndStackALU(const u8 yIndex)
    {
        FetchWord();

        switch (yIndex)
        {
            case 4:
            {
                m_Operand.as16 = 0xFF00U | m_Operand.as8;
                StoreWordToMemory(nullptr, m_A);
            }
                break;
            case 5:
                AddSignedWordToSP();
                break;
            case 6:
            {
                m_Operand.as16 = 0xFF00U | m_Operand.as8;
                LoadWordFromAddress(nullptr, m_A);
            }
                break;
            case 7:
                LoadToHL_SP_WithOffset();
                break;
        }
    }

    void Sharp::HandlePopMisc(const u8 yIndex)
    {
        const u8 qIndex = yIndex & 1U;
        const u8 pIndex = GetValueFromMask(yIndex, (u8)0b110U);

        if (qIndex == 0)
        {
            auto& registersToRestore = GetRegisterPairFromIndex(pIndex);
            PopRegisters(registersToRestore);
            if (pIndex == 3)
            {
                m_F.SetValue(m_F.GetValue() & 0xF0);
            }
        }
        else 
        {
            switch (pIndex)
            {
                case 1:
                    // TODO: Interrupts
                case 0:
                    Return();
                    break;
                case 2:
                    Jump(true, true);
                    break;
                case 3:
                    m_SP = m_HL.GetDoubleWord();
                    break;
            }
        }
    }

    void Sharp::HandleAbsoluteJump(const u8 yIndex)
    {
        if (yIndex <= GB_CPU_MAX_COND_CODE)
        {
            FetchDoubleWord();
            const bool bConditionMet = EvaluateCondition(yIndex);
            Jump(bConditionMet, false);
        }
        else
        {
            if ((~yIndex) & 1U)
            {
                m_Operand.as8 = m_C.GetValue();
                m_Operand.as16 = 0xFF00U | (u16)m_Operand.as8;
            }
            else
            {
                FetchDoubleWord();
            }

            switch (yIndex)
            {
                case 4:
                case 5:
                    StoreWordToMemory(nullptr, m_A);
                    break;
                case 6:
                case 7:
                    LoadWordFromAddress(nullptr, m_A);
                    break;
            }
        }
    }

    void Sharp::HandleAccumulatorALU(const u8 aluCode)
    {
        const u8 currentAccumulatorValue = m_A.GetValue();
        u8 newAccumulatorValue = 0;

        switch (aluCode)
        {
            case GB_INSTR_ADD_OPCODE:
            case GB_INSTR_ADC_OPCODE:
                newAccumulatorValue = UnsignedAddWord(
                    currentAccumulatorValue, 
                    m_Operand.as8, 
                    aluCode & 0b1
                );
                m_A.SetValue(newAccumulatorValue);
                break;
            case GB_INSTR_SUB_OPCODE:
            case GB_INSTR_SBC_OPCODE:
                newAccumulatorValue = UnsignedSubtractWord(
                    currentAccumulatorValue, 
                    m_Operand.as8, 
                    aluCode & 0b1
                );
                m_A.SetValue(newAccumulatorValue);
                break;
            case GB_INSTR_AND_OPCODE:
                AndAccumulator(m_Operand.as8);
                break;
            case GB_INSTR_XOR_OPCODE:
                XorAccumulator(m_Operand.as8);
                break;
            case GB_INSTR_OR_OPCODE:
                OrAccumulator(m_Operand.as8);
                break;
            case GB_INSTR_CP_OPCODE:
                UnsignedSubtractWord(currentAccumulatorValue, m_Operand.as8);
                break;
        }
    }

    void Sharp::RotateShiftHelper(ByteRegister& workingRegister, const u8 operation)
    {
        switch (operation)
        {
            case 0:
                workingRegister.SetValue(
                    RotateLeft(workingRegister.GetValue(), true)
                );
                break;
            case 1:
                workingRegister.SetValue(
                    RotateRight(workingRegister.GetValue(), true)
                );
                break;
            case 2:
                workingRegister.SetValue(
                    RotateLeft(workingRegister.GetValue(), false)
                );
                break;
            case 3:
                workingRegister.SetValue(
                    RotateRight(workingRegister.GetValue(), false)
                );
                break;
            case 4:
                workingRegister.SetValue(
                    ShiftLeftArithmetic(workingRegister.GetValue())
                );
                break;
            case 5:
                workingRegister.SetValue(
                    ShiftRightArithmetic(workingRegister.GetValue())
                );
                break;
            case 6:
                workingRegister.SetValue(
                    SwapNibbles(workingRegister.GetValue())
                );
                break;
            case 7:
                workingRegister.SetValue(
                    ShiftRightLogical(workingRegister.GetValue())
                );
                break;
        }
    }
}