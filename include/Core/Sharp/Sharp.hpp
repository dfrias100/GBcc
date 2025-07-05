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
#pragma once

#include "Types.hpp"
#include "Core/Sharp/SharpRegister.hpp"
#include "Core/SystemConstants.hpp"

#include <iostream>
#include <fstream>

namespace GBcc
{
    enum PointerOperation
    {
        Increment = 1,
        Decrement = 2,
        Nothing   = 3
    };

    enum class SharpFlags
    {
        ZERO    = 7,
        NOT_ADD = 6,
        HALF    = 5,
        CARRY   = 4
    };
    
    class Memory;

    class Sharp
    {
        private:
        ByteRegister m_A;
        ByteRegister m_F;

        ByteRegister m_B;
        ByteRegister m_C;

        ByteRegister m_D;
        ByteRegister m_E;

        ByteRegister m_H;
        ByteRegister m_L;

        ByteRegister m_HL_Memory;

        SharpRegister m_AF;
        SharpRegister m_BC;
        SharpRegister m_DE;
        SharpRegister m_HL;
        
        u16 m_SP = 0ULL;
        u16 m_PC = 0ULL;

        union
        {
            u16 as16;
            u8  as8;
        } m_Operand;

        Memory* const m_pMemBus;

        u64 m_CyclesTaken = 0ULL;

        std::ofstream m_ExecLog;

        void FetchWord();
        void FetchDoubleWord();

        void FetchHL();
        void WriteHL();

        ByteRegister& GetRegisterFromIndex(const u8 index);
        SharpRegister& GetRegisterPairFromIndex(const u8 index);

        template <typename T>
        bool TestBit(const T val, size_t bitIndex) const;

        inline void SetFlag(const SharpFlags& flag);
        inline void ResetFlag(const SharpFlags& flag);
        inline void UpdateFlag(const SharpFlags& flag, const bool set);
        inline bool FlagIsSet(const SharpFlags& flag);
        
        inline bool EvaluateCondition(const i8 conditionCode);

        void RegisterToRegisterWord(const ByteRegister& source, ByteRegister& destination);
        u8   UnsignedAddWord(const u8 lhs, const u8 rhs, const bool shouldAddCarry = false);
        u8   UnsignedSubtractWord(const u8 lhs, const u8 rhs, const bool shouldBorrow = false);
        u8   RotateLeft(const u8 value, const bool bCircular);
        u8   RotateRight(const u8 value, const bool bCircular);
        void AndAccumulator(const u8 value);
        void OrAccumulator(const u8 value);
        void XorAccumulator(const u8 value);
        void RotateLeftAccumulator(const bool bCircular);
        void RotateRightAccumulator(const bool bCircular);
        void DecimalAdjustAccumulator();
        void ComplementAccumulator();
        void ComplementCarry();
        void SetCarry();
        void DecrementRegisterWord(ByteRegister& reg);
        void IncrementRegisterWord(ByteRegister& reg);
        void AddRegisterWordToAccumulator(const ByteRegister& source);
        void LoadWordFromAddress(
            SharpRegister* const addressSourceRegister,
            ByteRegister& destination, 
            const PointerOperation ptrOp = PointerOperation::Nothing
        );
        void StoreWordToMemory(
            SharpRegister* const addressDestinationRegister,
            const ByteRegister& source, 
            const PointerOperation ptrOp = PointerOperation::Nothing
        );

        void LoadDoubleWordToRegister(SharpRegister& destination);
        void IncrementRegisterDoubleWord(SharpRegister& reg);
        void DecrementRegisterDoubleWord(SharpRegister& reg);
        u16  UnsignedAddDoubleWord(const u16 lhs, const u16 rhs);
        void PushRegisters(const SharpRegister& reg);
        void PopRegisters(SharpRegister& reg);
        void StoreSP_ToMemory();
        void LoadHL_ToSP();

        void Call(const bool bConditionMet = true);
        void Jump(const bool bConditionMet, const bool bAddressInHL);
        void JumpRelative(const bool bConditionMet = true);
        void Return(const bool bConditionMet = true);

        void AddSignedWordToSP();
        void LoadToHL_SP_WithOffset();

        void ResetToVector(const u16 resetVector);

        u8 ShiftLeftArithmetic(const u8 value);
        u8 ShiftRightArithmetic(const u8 value);
        u8 ShiftRightLogical(const u8 value);

        u8 SwapNibbles(const u8 value);
        void BitInstruction(const u8 index, const u8 value);
        u8 ResetBit(const u8 index, const u8 value);
        u8 SetBit(const u8 index, const u8 value);

        void ExecuteOpcode(const u8 opcode);

        void DecodeBlock0(const u8 opcode);
        void DecodeBlock1(const u8 opcode);
        void DecodeBlock2(const u8 opcode);
        void DecodeBlock3(const u8 opcode);
        void DecodePrefixCB(const u8 opcode);

        void HandleRelJumpMisc(const u8 opcode);
        void HandleIncrementDecrement(const u8 opcode, const u8 zIndex);
        void HandleLoadStoreIndirect(const u8 opcode);
        void HandleLoadDouble(const u8 opcode);
        void HandleMiscAccumulator(const u8 opcode);
        void HandleIOLoadAndStackALU(const u8 yIndex);
        void HandlePopMisc(const u8 yIndex);
        void HandleAbsoluteJump(const u8 yIndex);
        void HandleAccumulatorALU(const u8 aluCode);
        void LoadImmediateWord(const u8 opcode);
        void RotateShiftHelper(ByteRegister& workingRegister, const u8 operation);

        void DumpRegs();
        
        public:
        Sharp(Memory* const pMemBus);
        ~Sharp()
        {
            m_ExecLog.close();
        }

        u64 Step();
    };

    template <typename T>
    bool Sharp::TestBit(const T val, const size_t bitIndex) const
    {
        if (bitIndex > ((sizeof(T) * 8U) - 1))
        {
            return false;
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

    inline void Sharp::UpdateFlag(const SharpFlags& flag, const bool set)
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

    inline bool Sharp::FlagIsSet(const SharpFlags& flag)
    {
        return m_F.BitIsSet(static_cast<size_t>(flag));
    }

    inline bool Sharp::EvaluateCondition(const i8 conditionCode)
    {
        if (conditionCode < 0) return true;

        switch (conditionCode)
        {
            case GB_CPU_CONDITION_NZ:
                return !FlagIsSet(SharpFlags::ZERO);
            case GB_CPU_CONDITION_Z:
                return FlagIsSet(SharpFlags::ZERO);
            case GB_CPU_CONDITION_NC:
                return !FlagIsSet(SharpFlags::CARRY);
            case GB_CPU_CONDITION_C:
                return FlagIsSet(SharpFlags::CARRY);
            default:
                std::cerr << "Invalid condition code, cannot evaluate instruction condtion! Got: " << (i16) conditionCode << std::endl;
                exit(1);
        }
    }
};