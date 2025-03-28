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

#include <optional>

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

        SharpRegister m_AF;
        SharpRegister m_BC;
        SharpRegister m_DE;
        SharpRegister m_HL;
        
        u16 m_SP = 0ULL;
        u16 m_PC = 0ULL;

        u16 m_Operand;

        Memory* const m_pMemBus;

        u8  FetchWord();
        u16 FetchDoubleWord();

        template <typename T>
        bool TestBit(const T val, size_t bitIndex) const;

        inline void SetFlag(const SharpFlags& flag);
        inline void ResetFlag(const SharpFlags& flag);
        inline void UpdateFlag(const SharpFlags& flag, const bool set);
        inline bool FlagIsSet(const SharpFlags& flag);

        void RegisterToRegisterWord(const ByteRegister& source, ByteRegister& destination);
        u8   UnsignedAddWord(const u8 lhs, const u8 rhs, const bool shouldAddCarry = false);
        u8   UnsignedSubtractWord(const u8 lhs, const u8 rhs, const bool shouldBorrow = false);
        void DecrementRegisterWord(ByteRegister& reg);
        void IncrementRegisterWord(ByteRegister& reg);
        void IncrementWordAtHL();
        void AddRegisterWordToAccumulator(const ByteRegister& source);
        void LoadWordToRegister(ByteRegister& destination);
        void LoadWordFromAddress(
            SharpRegister const* addressSourceRegister,
            ByteRegister& destination, 
            const PointerOperation ptrOp
        );
        void StoreWordToMemory(
            SharpRegister const* addressDestinationRegister,
            const ByteRegister& source, 
            const PointerOperation ptrOp
        );

        void LoadDoubleWordToRegister(SharpRegister& destination);
        void IncrementRegisterDoubleWord(SharpRegister& reg);
        void DecrementRegisterDoubleWord(SharpRegister& reg);
        u16  UnsignedAddDoubleWord(const u16 lhs, const u16 rhs);
        void PushRegisters(const SharpRegister& reg);
        void PopRegisters(SharpRegister& reg);
        
        public:
        Sharp(Memory* const pMemBus);

        u64 StepExecution();
    };
};