#include "Core/Sharp/SharpRegister.hpp"
#include "Types.hpp"

#include "TestFunctions.hpp"

using GBcc::u16;
using GBcc::u8;

int main(int argc, char** argv)
{
    GBcc::SharpRegister testRegister;

    Expect((u16)0, testRegister.GetDoubleWord());
    Expect((u8)0, testRegister.GetWord(true));
    Expect((u8)0, testRegister.GetWord(false));

    u16 val = 0xEBFC;
    GBcc::SharpRegister testRegister2(val);

    Expect(val, testRegister2.GetDoubleWord());
    Expect((u8)0xEB, testRegister2.GetWord(true));
    Expect((u8)0xFC, testRegister2.GetWord(false));

    u8 valHigh = 0xAD;
    u8 valLow = 0x8C;

    u16 val2 = static_cast<u16>(valHigh) << 8U | valLow;

    GBcc::SharpRegister testRegister3(valHigh, valLow);

    Expect(val2, testRegister3.GetDoubleWord());
    Expect(valHigh, testRegister3.GetWord(true));
    Expect(valLow, testRegister3.GetWord(false));

    return 0;
}