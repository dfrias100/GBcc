#include "Core/Sharp/SharpRegister.hpp"
#include "Types.hpp"

#include "TestFunctions.hpp"

using GBcc::u16;
using GBcc::u8;

int main(int argc, char** argv)
{
    GBcc::ByteRegister testReg(0xFC);
    GBcc::ByteRegister testReg2(testReg);
    Expect(u8(0xFC), testReg2.GetValue());

    GBcc::ByteRegister testReg3 = testReg2;
    Expect(u8(0xFC), testReg3.GetValue());

    GBcc::ByteRegister testReg4(0xFE);
    GBcc::SharpRegister testLargeRegister(testReg, testReg2);
    GBcc::SharpRegister testLargeRegister2(testReg3, testReg4);

    testReg.SetValue(0xAB);
    testReg2.SetValue(0xCD);

    testReg3.SetValue(0xEF);
    testReg4.SetValue(0x12);
    
    testReg2 = testReg4;
    Expect(u16(0xAB12), testLargeRegister.GetDoubleWord());

    testReg4 = testReg;
    Expect(u16(0xEFAB), testLargeRegister2.GetDoubleWord());

    testReg3 = testReg;
    Expect(u16(0xABAB), testLargeRegister2.GetDoubleWord());
    
    return 0;
}