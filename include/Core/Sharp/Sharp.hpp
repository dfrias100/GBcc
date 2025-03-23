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

#include <variant>

namespace GBcc
{
    enum PointerOperation
    {
        Increment = 0x1,
        Decrement = 0x2,
        Nothing   = 0x3
    };

    enum class SharpFlags : u8
    {
        ZERO    = 7,
        NOT_ADD = 6,
        HALF    = 5,
        CARRY   = 4
    };
    
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

        u8  FetchWord();
        u16 FetchDoubleWord();

        void RegisterToRegisterWord(const ByteRegister& source, const ByteRegister& destination);
        void SetRegisterWord(const ByteRegister& destination, const u8& word);        
        void UnsignedAddWord(const u8& lhs, const u8& rhs);
        void SignedAddWord(const u8& lhs, const u8& rhs);
        void AddRegisterToAccumulator(const ByteRegister& source);
        void LoadValueToRegisterWord(const ByteRegister& destination);
        void LoadIndirectToRegisterWord(
            const SharpRegister& addressSource,
            const ByteRegister& destination, 
            const PointerOperation ptrOp, 
            const bool addressIsImmediate = false
        );
        void StoreRegisterToMemoryWord(
            const SharpRegister& addressDestination,
            const ByteRegister& source, 
            const PointerOperation ptrOp, 
            const bool addressIsImmediate = false
        );

        void RegisterToRegisterDoubleWord(const SharpRegister& source, const SharpRegister& destination);
        void SetRegisterDoubleWord(const SharpRegister& destination, const u16& doubleWord);
        void UnsignedAddDoubleWord(const u16& lhs, const u16& rhs);
        
        public:
        Sharp();

        u64 StepExecution();
    };
};