#include "Core/Sharp/SharpRegister.hpp"
#include "Types.hpp"

#include "TestFunctions.hpp"

using GBcc::u16;
using GBcc::u8;

int main(int argc, char** argv)
{
    GBcc::ByteRegister testReg;

    testReg.SetBit(7U);
    ExpectTrue(testReg.BitIsSet(7));
    Expect(u8(0b1000'0000), testReg.GetValue());

    testReg.SetBit(0U);
    ExpectTrue(testReg.BitIsSet(0));
    Expect(u8(0b1000'0001), testReg.GetValue());

    testReg.ResetBit(0U);
    ExpectFalse(testReg.BitIsSet(0));
    Expect(u8(0b1000'0000), testReg.GetValue());

    return 0;
}