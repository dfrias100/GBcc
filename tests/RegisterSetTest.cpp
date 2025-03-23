#include "Core/Sharp/SharpRegister.hpp"
#include "Types.hpp"

#include "TestFunctions.hpp"

using GBcc::u16;
using GBcc::u8;

int main(int argc, char** argv)
{
    GBcc::ByteRegister testReg;

    testReg.SetValue(0x5F);
    Expect(u8(0x5F), testReg.GetValue());

    GBcc::ByteRegister testReg2;
    GBcc::SharpRegister testLargeRegister(testReg, testReg2);

    testLargeRegister.SetDoubleWord(0xACDC);
    Expect(u16(0xACDC), testLargeRegister.GetDoubleWord());
    Expect(u8(0xAC), testReg.GetValue());
    Expect(u8(0xDC), testReg2.GetValue());

    testReg2.SetValue(0xBD);
    Expect(u16(0xACBD), testLargeRegister.GetDoubleWord());
    Expect(u8(0xAC), testReg.GetValue());
    Expect(u8(0xBD), testReg2.GetValue());

    return 0;
}