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
#include "Core/Sharp/SharpConstants.hpp"

#include <iostream>
#include <fstream>

#include <functional>

namespace GBcc
{
    class Memory;

    class Sharp
    {
        enum class SharpFlags
        {
            ZERO    = 7,
            NOT_ADD = 6,
            HALF    = 5,
            CARRY   = 4
        };
        
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
        std::function<void(u64)> Synchronize;

        std::ofstream m_ExecLog;

        template <typename T>
        bool TestBit(const T val, size_t bitIndex) const;

        inline void SetFlag(const SharpFlags flag);
        inline void ResetFlag(const SharpFlags flag);
        inline void UpdateFlag(const SharpFlags flag, const bool set);
        inline bool FlagIsSet(const SharpFlags flag);
        
        void DumpRegs();

        u8 FetchInstruction();
        
        public:
        Sharp(Memory* const pMemBus, std::function<void(u64)> syncCallback);
        ~Sharp()
        {
            m_ExecLog.close();
        }

        void Step();
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

    inline void Sharp::SetFlag(const SharpFlags flag)
    {
        m_F.SetBit(static_cast<size_t>(flag));
    }

    inline void Sharp::ResetFlag(const SharpFlags flag)
    {
        m_F.ResetBit(static_cast<size_t>(flag));
    }

    inline void Sharp::UpdateFlag(const SharpFlags flag, const bool set)
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

    inline bool Sharp::FlagIsSet(const SharpFlags flag)
    {
        return m_F.BitIsSet(static_cast<size_t>(flag));
    }
};