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
    constexpr u8 GB_INSTR_BLOCK_MASK    = 0b11'00'00'00U;
    constexpr u8 GB_Y_INDEX_MASK        = 0b00'11'10'00U;
    constexpr u8 GB_P_INDEX_MASK        = 0b00'11'00'00U;
    constexpr u8 GB_Q_INDEX_MASK        = 0b00'00'10'00U;
    constexpr u8 GB_Z_INDEX_MASK        = 0b00'00'01'11U;
    constexpr u8 GB_INSTR_PREFIX_CB     = 0xCBU;

    constexpr u8 GB_CPU_DEREF_HL_PTR    = 6U;
    constexpr u8 GB_CPU_REGISTER_SP     = 3U;
    constexpr i8 GB_CPU_CONDITION_NZ    = 0;
    constexpr i8 GB_CPU_CONDITION_Z     = 1;
    constexpr i8 GB_CPU_CONDITION_NC    = 2;
    constexpr i8 GB_CPU_CONDITION_C     = 3;
    constexpr i8 GB_CPU_MAX_COND_CODE   = GB_CPU_CONDITION_C;

    constexpr u8 GB_INSTR_HALT_OPCODE       = 0b01'11'01'10U;
    constexpr u8 GB_INSTR_NOP_OPCODE        = 0U;
    constexpr u8 GB_INSTR_ADD_OPCODE        = 0U;
    constexpr u8 GB_INSTR_ADC_OPCODE        = 1U;
    constexpr u8 GB_INSTR_SUB_OPCODE        = 2U;
    constexpr u8 GB_INSTR_SBC_OPCODE        = 3U;
    constexpr u8 GB_INSTR_AND_OPCODE        = 4U;
    constexpr u8 GB_INSTR_XOR_OPCODE        = 5U;
    constexpr u8 GB_INSTR_OR_OPCODE         = 6U;
    constexpr u8 GB_INSTR_CP_OPCODE         = 7U;
    constexpr u8 GB_INSTR_STR_SP_IMM_PTR    = 1U;
    constexpr u8 GB_INSTR_JMP_REL_MIN       = 3U;
    constexpr u8 GB_INSTR_JMP_REL_MAX       = 7U;

    constexpr u8 GB_INSTR_BLOCK_REL_JMP     = 0U;
    constexpr u8 GB_INSTR_BLOCK_LDD_IMM     = 1U;
    constexpr u8 GB_INSTR_BLOCK_LDW_IND     = 2U;
    constexpr u8 GB_INSTR_BLOCK_INC_DEC_DW  = 3U;
    constexpr u8 GB_INSTR_BLOCK_INC_WRD     = 4U;
    constexpr u8 GB_INSTR_BLOCK_DEC_WRD     = 5U;
    constexpr u8 GB_INSTR_BLOCK_LDW_IMM     = 6U;
    constexpr u8 GB_INSTR_BLOCK_ACC_MISC    = 7U;

    constexpr u8 GB_INSTR_CRET_MMLD_STACK   = 0U;
    constexpr u8 GB_INSTR_POP_MISC          = 1U;
    constexpr u8 GB_INSTR_COND_JUMP         = 2U;
    constexpr u8 GB_INSTR_MISC_OPS          = 3U;
    constexpr u8 GB_INSTR_COND_CALL         = 4U;
    constexpr u8 GB_INSTR_PUSH_MISC         = 5U;
    constexpr u8 GB_INSTR_ALU_IMM           = 6U;
    constexpr u8 GB_INSTR_RESET             = 7U;

    constexpr u8 GB_INSTR_ALU_OP_RLCA = 0U;
    constexpr u8 GB_INSTR_ALU_OP_RRCA = 1U;
    constexpr u8 GB_INSTR_ALU_OP_RLA  = 2U;
    constexpr u8 GB_INSTR_ALU_OP_RRA  = 3U;
    constexpr u8 GB_INSTR_ALU_OP_DAA  = 4U;
    constexpr u8 GB_INSTR_ALU_OP_CPLA = 5U;
    constexpr u8 GB_INSTR_ALU_OP_SCF  = 6U;
    constexpr u8 GB_INSTR_ALU_OP_CCF  = 7U;

    constexpr u8 GB_INSTR_CB_OP_RLC  = 0U;
    constexpr u8 GB_INSTR_CB_OP_RRC  = 1U;
    constexpr u8 GB_INSTR_CB_OP_RL   = 2U;
    constexpr u8 GB_INSTR_CB_OP_RR   = 3U;
    constexpr u8 GB_INSTR_CB_OP_SLA  = 4U;
    constexpr u8 GB_INSTR_CB_OP_SRA  = 5U;
    constexpr u8 GB_INSTR_CB_OP_SWAP = 6U;
    constexpr u8 GB_INSTR_CB_OP_SRL  = 7U;

    constexpr u8 GB_INSTR_STR_A_MMIO    = 4U;
    constexpr u8 GB_INSTR_STR_A_IMM_PTR = 5U;
    constexpr u8 GB_INSTR_LD_A_MMIO     = 6U;
    constexpr u8 GB_INSTR_LD_A_IMM_PTR  = 7U;  

    constexpr u8 GB_INSTR_ADD_SP_SIGNED     = 5U;
    constexpr u8 GB_INSTR_LOAD_HL_OFFSET_SP = 7U;

    constexpr bool GB_CIRCULAR_ROTATE = true;
    constexpr bool GB_NON_CIRCULAR_ROTATE = false; 
    
    constexpr u16 GB_MMIO_BASE_ADDRESS = 0xFF00U;

    constexpr u8 GB_INSTR_BLOCK_STR_IMM_PTR = 1U;
}