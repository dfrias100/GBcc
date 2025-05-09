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
    constexpr size_t GB_BOOTROM_SIZE   = 256ULL;
    constexpr size_t GB_BOOTROM_END    = GB_BOOTROM_SIZE - 1U;
    constexpr u16    GB_CART_SPACE_END = 0x7FFFULL;
    constexpr u8     GB_X_INDEX_MASK   = 0b11'00'00'00U;
    constexpr u8     GB_Y_INDEX_MASK   = 0b00'11'10'00U;
    constexpr u8     GB_P_INDEX_MASK   = 0b00'11'00'00U;
    constexpr u8     GB_Q_INDEX_MASK   = 0b00'00'10'00U;
    constexpr u8     GB_Z_INDEX_MASK   = 0b00'00'01'11U;
}