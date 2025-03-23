#include "Core/Sharp/SharpRegister.hpp"
#include "Types.hpp"

#include "TestFunctions.hpp"

using GBcc::u16;
using GBcc::u8;

int main(int argc, char** argv)
{
    GBcc::ByteRegister testReg(0xFC);
    Expect(u8(0xFC), testReg.GetValue());

    GBcc::ByteRegister testReg2;
    Expect(u8(0x00), testReg2.GetValue());

    GBcc::SharpRegister testLargeReg(testReg, testReg2);
    Expect(u16(0xFC00), testLargeReg.GetDoubleWord());

    GBcc::ByteRegister testReg3(0xBC);
    GBcc::ByteRegister testReg4(0xEA);
    GBcc::SharpRegister testLargeReg2(testReg3, testReg4);

    Expect(u16(0xBCEA), testLargeReg2.GetDoubleWord());

    return 0;
}