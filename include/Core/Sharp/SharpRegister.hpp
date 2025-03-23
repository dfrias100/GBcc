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

#include "Types.hpp"

namespace GBcc
{
    class ByteRegister
    {
        private:
        u8 m_Value;

        public:
        ByteRegister();
        ByteRegister(const u8& initializerValue);
        ByteRegister(const ByteRegister& reg);
        
        ByteRegister& operator=(const ByteRegister& reg);
        ByteRegister& operator=(ByteRegister&& reg) = delete;

        ~ByteRegister() = default;

        void SetValue(const u8& value);
        u8 GetValue();

        void SetBit(size_t bitIndex);
        void ResetBit(size_t bitIndex);

        bool BitIsSet(size_t bitIndex);
    };

    class SharpRegister
    {
        protected:
        ByteRegister& m_HighRegister;
        ByteRegister& m_LowRegister;

        public:
        SharpRegister() = delete;
        SharpRegister(ByteRegister& highRegister, ByteRegister& lowRegister);
        SharpRegister(const SharpRegister& reg) = delete;

        SharpRegister& operator=(const SharpRegister& reg) = delete;
        SharpRegister& operator=(SharpRegister&& reg) = delete;

        ~SharpRegister() = default;

        u16 GetDoubleWord();

        void SetDoubleWord(const u16& doubleWord);
    };
};