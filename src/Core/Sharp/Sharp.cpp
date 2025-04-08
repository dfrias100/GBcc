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
#include "Core/Sharp/Sharp.hpp"
#include "Core/Memory.hpp"
#include "Core/Bitmasks.hpp"

#include "Utility.hpp"

#include <iostream>

namespace GBcc {
    Sharp::Sharp(Memory* const pMemBus) : 
        m_AF(m_A, m_F),
        m_BC(m_B, m_C),
        m_DE(m_D, m_E),
        m_HL(m_H, m_L),
        m_pMemBus(pMemBus)
    {}

    void Sharp::FetchWord()
    {
        m_Operand.as8 = m_pMemBus->ReadWord(m_PC++);
    }

    void Sharp::FetchDoubleWord()
    {
        m_Operand.as16 = m_pMemBus->ReadDoubleWord(m_PC);
        m_PC += 2;
    }

    void Sharp::FetchHL() 
    {
        const u16 address = m_HL.GetDoubleWord();
        const u8 fetchedVal = m_pMemBus->ReadWord(address);
        m_HL_Memory.SetValue(fetchedVal);
    }

    void Sharp::WriteHL() 
    {
        const u16 address = m_HL.GetDoubleWord();
        const u8 valToWrite = m_HL_Memory.GetValue();
        m_pMemBus->WriteWord(address, valToWrite);
    }

    ByteRegister& Sharp::GetRegisterFromIndex(const u8 index)
    {
        switch (index)
        {
            case 0:
                return m_B;
            case 1:
                return m_C;
            case 2:
                return m_D;
            case 3:
                return m_E;
            case 4:
                return m_H;
            case 5:
                return m_L;
            case 6:
                return m_HL_Memory;
            case 7:
                return m_A;
            default:
                std::cerr << "Invalid register index specified, cannot return any register! Index supplied: " << index << std::endl;
                exit(-1);
        }
    }

    SharpRegister& Sharp::GetRegisterPairFromIndex(const u8 index)
    {
        switch (index)
        {
            case 0:
                return m_BC;
            case 1:
                return m_DE;
            case 2:
                return m_HL;
            case 3:
                return m_AF;
            default:
                std::cerr << "Invalid register index specified, cannot return any register! Index supplied: " << index << std::endl;
                exit(-1);
        }
    }

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

    inline void Sharp::ComplementCarry()
    {
        bool bCurrentCarry = ~FlagIsSet(SharpFlags::CARRY);
        UpdateFlag(SharpFlags::CARRY, bCurrentCarry);
    }

    inline bool Sharp::FlagIsSet(const SharpFlags& flag)
    {
        return m_F.BitIsSet(static_cast<size_t>(flag));
    }

    void Sharp::RegisterToRegisterWord(const ByteRegister& source, ByteRegister& destination)    
    {
        auto val = source.GetValue();
        destination.SetValue(val);
    }
    
    u8 Sharp::UnsignedAddWord(const u8 lhs, const u8 rhs, const bool shouldAddCarry)
    {
        u8 carry = shouldAddCarry ? static_cast<u8>(FlagIsSet(SharpFlags::CARRY)) : 0U;
        u8 lhsLowNibble = lhs & GB_LOW_NIBBLE;
        u8 rhsLowNibble = rhs & GB_LOW_NIBBLE;

        u8 halfAdd = lhsLowNibble + rhsLowNibble + carry;
        bool shouldSetHalfCarry = TestBit(halfAdd, 4U);
        UpdateFlag(SharpFlags::HALF, shouldSetHalfCarry);

        u16 result16 = lhs + rhs + carry;
        bool shouldSetCarry = TestBit(result16, 8U);
        UpdateFlag(SharpFlags::CARRY, shouldSetCarry);

        u8 result = result16 & GB_LOW_BYTE;
        bool isZero = result == 0;
        UpdateFlag(SharpFlags::ZERO, isZero);

        UpdateFlag(SharpFlags::NOT_ADD, false);

        return result;
    }

    u8 Sharp::UnsignedSubtractWord(const u8 lhs, const u8 rhs, const bool shouldBorrow)
    {
        const u8 rhsNegative = ~rhs + (
            shouldBorrow ?
            static_cast<u8>(FlagIsSet(SharpFlags::CARRY)) :
            1U
        );
        const u8 result = UnsignedAddWord(lhs, rhsNegative, false);
        UpdateFlag(SharpFlags::NOT_ADD, true);

        return result;
    }

    u8 Sharp::RotateLeft(const u8 value, const bool bCircular)
    {
        const u8 outBit = (value & 0b1000'0000) >> 7U;
        const u8 inBit  = bCircular ? outBit : FlagIsSet(SharpFlags::CARRY);
        
        u8 newValue = value << 1U;
        newValue |= inBit;

        UpdateFlag(SharpFlags::ZERO, false);
        UpdateFlag(SharpFlags::NOT_ADD, false);
        UpdateFlag(SharpFlags::HALF, false);
        UpdateFlag(SharpFlags::CARRY, outBit);

        return newValue;
    }

    u8 Sharp::RotateRight(const u8 value, const bool bCircular)
    {
        const u8 outBit = value & 0b1;
        const u8 inBit = bCircular ? outBit : FlagIsSet(SharpFlags::CARRY);

        u8 newValue = value >> 1U;
        newValue |= (inBit << 7U);

        UpdateFlag(SharpFlags::ZERO, false);
        UpdateFlag(SharpFlags::NOT_ADD, false);
        UpdateFlag(SharpFlags::HALF, false);
        UpdateFlag(SharpFlags::CARRY, outBit);

        return newValue;
    }

    void Sharp::AndAccumulator(const u8 value)
    {
        u8 currentAccumulator = m_A.GetValue();
        currentAccumulator &= value;

        bool isZero = currentAccumulator == 0U; 
        UpdateFlag(SharpFlags::ZERO, isZero);
        UpdateFlag(SharpFlags::NOT_ADD, false);
        UpdateFlag(SharpFlags::HALF, true);
        UpdateFlag(SharpFlags::CARRY, false);

        m_A.SetValue(currentAccumulator);
    }

    void Sharp::XorAccumulator(const u8 value)
    {        
        const u8 currentAccumulator = m_A.GetValue() ^ value;
        
        const bool isZero = currentAccumulator == 0U; 
        UpdateFlag(SharpFlags::ZERO, isZero);
        UpdateFlag(SharpFlags::NOT_ADD, false);
        UpdateFlag(SharpFlags::HALF, false);
        UpdateFlag(SharpFlags::CARRY, false);

        m_A.SetValue(currentAccumulator);
    }

    void Sharp::OrAccumulator(const u8 value)
    {
        const u8 currentAccumulator = m_A.GetValue() | value;

        const bool isZero = currentAccumulator == 0U; 
        UpdateFlag(SharpFlags::ZERO, isZero);
        UpdateFlag(SharpFlags::NOT_ADD, false);
        UpdateFlag(SharpFlags::HALF, false);
        UpdateFlag(SharpFlags::CARRY, false);

        m_A.SetValue(currentAccumulator);
    }

    void Sharp::DecimalAdjustAccumulator()
    {
        u8 adjustment = 0U;
        u8 currentAccumulator = m_A.GetValue();
        if (FlagIsSet(SharpFlags::NOT_ADD))
        {
            adjustment += FlagIsSet(SharpFlags::HALF)  ? 0x06U : 0U;
            adjustment += FlagIsSet(SharpFlags::CARRY) ? 0x60U : 0U;
            const bool shouldSetCarry = adjustment > currentAccumulator;
            UpdateFlag(SharpFlags::CARRY, shouldSetCarry);
            currentAccumulator -= adjustment;
        }
        else
        {
            if (
                FlagIsSet(SharpFlags::HALF) 
                || ((currentAccumulator & 0x0FU) > 0x09)
            )
            {
                adjustment += 0x06U;
            }

            if (
                FlagIsSet(SharpFlags::CARRY) 
                || (currentAccumulator > 0x9F)
            )
            {
                adjustment += 0x60U;
            }

            const u16 result16 = currentAccumulator + adjustment;
            const bool shouldSetCarry = TestBit(result16, 8U);
            currentAccumulator += adjustment;
        }

        m_A.SetValue(currentAccumulator);
    }

    void Sharp::DecrementRegisterWord(ByteRegister& reg)
    {
        const bool bCurrentCarry = FlagIsSet(SharpFlags::CARRY);

        const u8 currentRegisterValue = reg.GetValue();
        const u8 decrementedRegister = UnsignedSubtractWord(currentRegisterValue, 1U);

        reg.SetValue(decrementedRegister);

        UpdateFlag(SharpFlags::CARRY, bCurrentCarry);
    }

    void Sharp::IncrementRegisterWord(ByteRegister& reg)
    {
        const bool bCurrentCarry = FlagIsSet(SharpFlags::CARRY);

        const u8 currentRegisterValue = reg.GetValue();
        const u8 incrementedRegister = UnsignedAddWord(currentRegisterValue, 1U);
        
        reg.SetValue(incrementedRegister);

        UpdateFlag(SharpFlags::CARRY, bCurrentCarry); 
    }

    void Sharp::AddRegisterWordToAccumulator(const ByteRegister& source)
    {
        const u8 operand = source.GetValue();
        const u8 result = UnsignedAddWord(m_A.GetValue(), operand);
        m_A.SetValue(result);
    }

    void Sharp::LoadWordToRegister(ByteRegister& destination)
    {
        const u8 value = m_Operand.as8;
        destination.SetValue(value);
    }

    void Sharp::LoadWordFromAddress(
        SharpRegister* const addressSourceRegister,
        ByteRegister& destination,
        const PointerOperation ptrOp
    )
    {
        u16 address;
        if (addressSourceRegister != nullptr)
        {
            address = addressSourceRegister->GetDoubleWord();

            if (ptrOp == PointerOperation::Increment)
            {
                address++;
                addressSourceRegister->SetDoubleWord(address);
            }
            else if (ptrOp == PointerOperation::Decrement)
            {
                address--;
                addressSourceRegister->SetDoubleWord(address);
            }
        }
        else
        {
            address = m_Operand.as16;
        }

        const u8 val = m_pMemBus->ReadWord(address);
        destination.SetValue(val);
    }

    void Sharp::StoreWordToMemory(
        SharpRegister* const addressDestinationRegister,
        const ByteRegister& source, 
        const PointerOperation ptrOp
    )
    {
        u16 address;
        if (addressDestinationRegister != nullptr)
        {;
            address = addressDestinationRegister->GetDoubleWord();

            if (ptrOp == PointerOperation::Increment)
            {
                address++;
                addressDestinationRegister->SetDoubleWord(address);
            }
            else if (ptrOp == PointerOperation::Decrement)
            {
                address--;
                addressDestinationRegister->SetDoubleWord(address);
            }
        }
        else
        {
            address = m_Operand.as16;
        }

        const u8 val = source.GetValue();
        m_pMemBus->WriteWord(address, val);
    }

    void Sharp::LoadDoubleWordToRegister(SharpRegister& destination)
    {
        destination.SetDoubleWord(m_Operand.as16);
    }

    void Sharp::IncrementRegisterDoubleWord(SharpRegister& reg)
    {
        const u16 incrementedRegister = reg.GetDoubleWord() + 1U;
        reg.SetDoubleWord(incrementedRegister);
    }

    void Sharp::DecrementRegisterDoubleWord(SharpRegister& reg)
    {
        const u16 decrementedRegister = reg.GetDoubleWord() - 1U;
        reg.SetDoubleWord(decrementedRegister);
    }

    u16 Sharp::UnsignedAddDoubleWord(const u16 lhs, const u16 rhs)
    {
        u16 lhsMasked = lhs & 0x0F'FFU;
        u16 rhsMasked = rhs & 0x0F'FFU;
        
        u16 halfAdd = lhsMasked + rhsMasked;
        bool shouldSetHalfCarry = TestBit(halfAdd, 12U);
        UpdateFlag(SharpFlags::HALF, shouldSetHalfCarry);

        u32 result32 = lhs + rhs;
        bool shouldSetCarry = TestBit(result32, 16U);
        UpdateFlag(SharpFlags::CARRY, shouldSetCarry);

        u16 result = result32 & U32_LOW_HALF;

        UpdateFlag(SharpFlags::NOT_ADD, false);

        return result;
    }

    void Sharp::PushRegisters(const SharpRegister& reg)
    {
        m_SP -= 2;
        const u16 registerValue = reg.GetDoubleWord();
        m_pMemBus->WriteDoubleWord(m_SP, registerValue);
    }

    void Sharp::PopRegisters(SharpRegister& reg)
    {
        const u16 registerValue = m_pMemBus->ReadDoubleWord(m_SP);
        reg.SetDoubleWord(registerValue);
        m_SP += 2;
    }

    void Sharp::StoreSP_ToMemory()
    {
        m_pMemBus->WriteDoubleWord(m_Operand.as16, m_SP);
    }

    void Sharp::LoadHL_ToSP()
    {
        m_HL.SetDoubleWord(m_SP);
    }

    void Sharp::Call(const bool bConditionMet) 
    {
        if (!bConditionMet)
            return;
        
        m_SP -= 2;
        m_pMemBus->WriteDoubleWord(m_SP, m_PC);
        m_PC = m_Operand.as16;

        m_CyclesTaken += 12ULL;
    }

    void Sharp::Jump(const bool bConditionMet, const bool bAddressInHL)
    {
        if (!bConditionMet)
            return;
        m_PC = bAddressInHL ? m_HL.GetDoubleWord() : m_Operand.as16;
        m_CyclesTaken += 4ULL;
    }

    void Sharp::JumpRelative(const bool bConditionMet)
    {
        if (!bConditionMet)
            return;
        const i8 offset = m_Operand.as8;
        m_PC += offset;
        m_CyclesTaken += 4ULL;
    }

    void Sharp::Return(const bool bConditionMet)
    {
        if (!bConditionMet)
            return;
        m_PC = m_pMemBus->ReadDoubleWord(m_SP);
        m_SP += 2;
        m_CyclesTaken += 12ULL;
    }

    void Sharp::AddSignedWordToSP()
    {
        const i8 offset = m_Operand.as8;

        const u8 spLowNibble = m_SP & GB_LOW_NIBBLE;
        const u8 offsetLowNibble = offset & GB_LOW_NIBBLE;

        const u8 halfAdd = spLowNibble + offsetLowNibble;
        const bool shouldSetHalfCarry = TestBit(halfAdd, 4U);
        UpdateFlag(SharpFlags::HALF, shouldSetHalfCarry);

        const u8 spLowWord = m_SP & GB_LOW_BYTE;
        const u16 fullAdd = spLowWord + m_Operand.as8;
        const bool shouldSetCarry = TestBit(fullAdd, 8U);
        
        UpdateFlag(SharpFlags::ZERO, false);
        UpdateFlag(SharpFlags::NOT_ADD, false);

        m_SP += offset;
    }

    void Sharp::LoadToHL_SP_WithOffset()
    {
        AddSignedWordToSP();
        m_HL.SetDoubleWord(m_SP);
    }

    void Sharp::ResetToVector(const u16 resetVector)
    {
        m_SP -= 2U;
        m_pMemBus->WriteDoubleWord(m_SP, m_PC);
        m_PC = resetVector;
    }

    u8 Sharp::ShiftLeftArithmetic(const u8 value)
    {
        const bool shouldSetCarry = TestBit(value, 7U);
        const u8 newValue = value << 1U;
        
        const bool shouldSetZero = newValue == 0U;

        UpdateFlag(SharpFlags::ZERO, shouldSetZero);
        UpdateFlag(SharpFlags::NOT_ADD, false);
        UpdateFlag(SharpFlags::HALF, false);
        UpdateFlag(SharpFlags::CARRY, shouldSetCarry);

        return newValue;
    }

    u8 Sharp::ShiftRightArithmetic(const u8 value)
    {
        const bool shouldSetCarry = TestBit(value, 0U);
        const u8 signBitMask = value & 0x80U;
        const u8 newValue = (value >> 1U) | signBitMask;

        const bool shouldSetZero = newValue == 0U;

        UpdateFlag(SharpFlags::ZERO, shouldSetZero);
        UpdateFlag(SharpFlags::NOT_ADD, false);
        UpdateFlag(SharpFlags::HALF, false);
        UpdateFlag(SharpFlags::CARRY, shouldSetCarry);

        return newValue;
    }

    u8 Sharp::ShiftRightLogical(const u8 value)
    {
        const bool shouldSetCarry = TestBit(value, 0U);
        const u8 newValue = value >> 1U;
        
        const bool shouldSetZero = newValue == 0U;

        UpdateFlag(SharpFlags::ZERO, shouldSetZero);
        UpdateFlag(SharpFlags::NOT_ADD, false);
        UpdateFlag(SharpFlags::HALF, false);
        UpdateFlag(SharpFlags::CARRY, shouldSetCarry);

        return newValue;
    }

    u8 Sharp::SwapNibbles(const u8 value)
    {
        const u8 newHighNibble = (value & 0x0'FU) << 4U;
        const u8 newLowNibble  = (value & 0xF'0U) >> 4U;

        return newHighNibble | newLowNibble;
    }

    void Sharp::BitInstruction(const u8 index, const u8 value)
    {
        bool shouldSetZero = !TestBit(value, index);
        UpdateFlag(SharpFlags::ZERO, shouldSetZero);
        UpdateFlag(SharpFlags::NOT_ADD, false);
        UpdateFlag(SharpFlags::HALF, true);
    }

    u8 Sharp::ResetBit(const u8 index, const u8 value)
    {
        const u8 bitMask = ~(0x1U << index);
        const u8 newValue = value & bitMask;

        return newValue;
    }

    u8 Sharp::SetBit(const u8 index, const u8 value)
    {
        const u8 bitMask = 0x1U << index;
        const u8 newValue = value | bitMask;

        return newValue;
    }

    void Sharp::ExecuteOpcode(const u8 opcode)
    {
        const u8 xIndex = GetValueFromMask(opcode, GB_X_INDEX_MASK);
        switch (xIndex)
        {
            case 0:
                DecodeX_Zero(opcode);
            case 1:
                Load8Bit(opcode);
            case 2:
                ArithmeticLogicUnit(opcode);
            case 3:
                DecodeX_Three(opcode);
        }
    }

    void Sharp::DecodeX_Zero(const u8 opcode)
    {
        const u8 zIndex = GetValueFromMask(opcode, GB_Z_INDEX_MASK);

        switch (zIndex)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            default:
                return;
        }
    }

    void Sharp::Load8Bit(const u8 opcode)
    {

    }

    void Sharp::ArithmeticLogicUnit(const u8 opcode)
    {

    }

    void Sharp::DecodeX_Three(const u8 opcode)
    {

    }
}