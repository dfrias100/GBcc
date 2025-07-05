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

namespace GBcc
{
    void Sharp::LoadWordFromAddress(
        SharpRegister* const addressSourceRegister,
        ByteRegister& destination,
        const PointerOperation ptrOp
    ) {
        u16 address;
        if (addressSourceRegister != nullptr)
        {
            address = addressSourceRegister->GetDoubleWord();

            if (ptrOp == PointerOperation::Increment)
            {
                addressSourceRegister->SetDoubleWord(address + 1U);
            }
            else if (ptrOp == PointerOperation::Decrement)
            {
                addressSourceRegister->SetDoubleWord(address - 1U);
            }
        }
        else
        {
            address = m_Operand.as16;
        }

        const u8 val = m_pMemBus->ReadWord(address);
        destination.SetValue(val);
    }

    void Sharp::StoreWordToMemory (
        SharpRegister* const addressDestinationRegister,
        const ByteRegister& source, 
        const PointerOperation ptrOp
    ) {
        u16 address;
        if (addressDestinationRegister != nullptr)
        {;
            address = addressDestinationRegister->GetDoubleWord();

            if (ptrOp == PointerOperation::Increment)
            {
                addressDestinationRegister->SetDoubleWord(address + 1U);
            }
            else if (ptrOp == PointerOperation::Decrement)
            {
                addressDestinationRegister->SetDoubleWord(address - 1U);
            }
        }
        else
        {
            address = m_Operand.as16;
        }

        const u8 val = source.GetValue();
        m_pMemBus->WriteWord(address, val);
    }

    void Sharp::RegisterToRegisterWord(const ByteRegister& source, ByteRegister& destination)    
    {
        const u8 val = source.GetValue();
        destination.SetValue(val);
    }

    void Sharp::LoadDoubleWordToRegister(SharpRegister& destination)
    {
        destination.SetDoubleWord(m_Operand.as16);
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

    void Sharp::LoadToHL_SP_WithOffset()
    {
        const u16 tempSP = m_SP;

        AddSignedWordToSP();
        m_HL.SetDoubleWord(m_SP);

        m_SP = tempSP;
    }
}