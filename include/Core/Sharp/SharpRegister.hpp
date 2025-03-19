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
    enum class SharpFlags : u8
    {
        ZERO    = 0b1000'0000,
        NOT_ADD = 0b0100'0000,
        HALF    = 0b0010'0000,
        CARRY   = 0b0001'0000
    };

    class SharpRegister
    {
        protected:
        u8 m_HighByte;
        u8 m_LowByte;

        public:
        SharpRegister();
        SharpRegister(const u16& doubleWord);
        SharpRegister(const u8& highByte, const u8& lowByte);
        SharpRegister(const SharpRegister& reg);

        SharpRegister& operator=(const SharpRegister& reg);
        SharpRegister& operator=(SharpRegister&& reg);

        ~SharpRegister() = default;

        u16 GetDoubleWord();
        u8  GetWord(const bool highByte);

        virtual void SetDoubleWord(const u16& word);
        virtual void SetWord(const bool highByte, const u8& word);
    };

    class SharpFlagsRegister : public SharpRegister
    {
        public:
        void SetFlag(const SharpFlags flag, const bool raise);
        bool FlagIsRaised(const SharpFlags flag);
        void SetDoubleWord(const u16& doubleWord) override;
        void SetWord(const bool highByte, const u8& dobuleWord) override;
    };
};