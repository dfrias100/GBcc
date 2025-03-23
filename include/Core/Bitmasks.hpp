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
    constexpr u16 GB_HIGH_BYTE = 0xFF'00U;
    constexpr u16  GB_LOW_BYTE = 0x00'FFU;

    constexpr u8 GB_HIGH_NIBBLE = 0xF'0U;
    constexpr u8  GB_LOW_NIBBLE = 0x0'FU;

    constexpr u32 U32_HIGH_HALF = 0xFFFF'0000U;
    constexpr u32  U32_LOW_HALF = 0x0000'FFFFU;
}