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
    constexpr size_t GB_BOOTROM_SIZE        = 256ULL;
    constexpr size_t GB_BOOTROM_END         = GB_BOOTROM_SIZE - 1U;
    constexpr u16    GB_CART_SPACE_END      = 0x7FFFULL;

    constexpr u8     GB_CPU_DEREF_HL_PTR    = 6U;
    constexpr u8     GB_CPU_REGISTER_SP     = 3U;
    constexpr i8     GB_CPU_CONDITION_NZ    = 0;
    constexpr i8     GB_CPU_CONDITION_Z     = 1;
    constexpr i8     GB_CPU_CONDITION_NC    = 2;
    constexpr i8     GB_CPU_CONDITION_C     = 3;

    constexpr u8     GB_INSTR_PREFIX_CB     = 0xCBU;
    constexpr u8     GB_INSTR_BLOCK_MASK        = 0b11'00'00'00U;
    constexpr u8     GB_INSTR_HALT_OPCODE       = 0b01'11'01'10U;
    constexpr u8     GB_INSTR_NOP_OPCODE        = 0U;
    constexpr u8     GB_INSTR_ADD_OPCODE        = 0U;
    constexpr u8     GB_INSTR_ADC_OPCODE        = 1U;
    constexpr u8     GB_INSTR_SUB_OPCODE        = 2U;
    constexpr u8     GB_INSTR_SBC_OPCODE        = 3U;
    constexpr u8     GB_INSTR_AND_OPCODE        = 4U;
    constexpr u8     GB_INSTR_XOR_OPCODE        = 5U;
    constexpr u8     GB_INSTR_OR_OPCODE         = 6U;
    constexpr u8     GB_INSTR_CP_OPCODE         = 7U;
    constexpr u8     GB_INSTR_STR_SP_IMM_PTR    = 1U;
    constexpr u8     GB_INSTR_JMP_REL_MIN       = 3U;
    constexpr u8     GB_INSTR_JMP_REL_MAX       = 7U;

    constexpr u8     GB_INSTR_BLOCK_REL_JMP     = 0U;
    constexpr u8     GB_INSTR_BLOCK_LDD_IMM     = 1U;
    constexpr u8     GB_INSTR_BLOCK_LDW_IND     = 2U;
    constexpr u8     GB_INSTR_BLOCK_INC_DEC_DW  = 3U;
    constexpr u8     GB_INSTR_BLOCK_INC_WRD     = 4U;
    constexpr u8     GB_INSTR_BLOCK_DEC_WRD     = 5U;
    constexpr u8     GB_INSTR_BLOCK_LDW_IMM     = 6U;
    constexpr u8     GB_INSTR_BLOCK_ACC_MISC    = 7U;

    constexpr u8     GB_INSTR_BLOCK_STR_IMM_PTR = 1U;
    
    constexpr u8     GB_Y_INDEX_MASK        = 0b00'11'10'00U;
    constexpr u8     GB_P_INDEX_MASK        = 0b00'11'00'00U;
    constexpr u8     GB_Q_INDEX_MASK        = 0b00'00'10'00U;
    constexpr u8     GB_Z_INDEX_MASK        = 0b00'00'01'11U;
}