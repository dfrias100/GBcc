#include "Core/Sharp/SharpRegister.hpp"
#include "Types.hpp"

#include "TestFunctions.hpp"

using GBcc::u16;
using GBcc::u8;

int main(int argc, char** argv)
{
    GBcc::SharpFlagsRegister testRegister;

    testRegister.SetFlag(GBcc::SharpFlags::ZERO, true);

    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::ZERO));
    Expect((u16)0x0080, testRegister.GetDoubleWord());
    Expect( (u8)0x00, testRegister.GetWord(true));
    Expect( (u8)0x80, testRegister.GetWord(false));

    testRegister.SetFlag(GBcc::SharpFlags::NOT_ADD, true);

    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::ZERO));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::NOT_ADD));
    Expect((u16)0x00C0, testRegister.GetDoubleWord());
    Expect( (u8)0x00, testRegister.GetWord(true));
    Expect( (u8)0xC0, testRegister.GetWord(false));


    testRegister.SetFlag(GBcc::SharpFlags::HALF, true);

    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::ZERO));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::NOT_ADD));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::HALF));
    Expect((u16)0x00E0, testRegister.GetDoubleWord());
    Expect( (u8)0x00, testRegister.GetWord(true));
    Expect( (u8)0xE0, testRegister.GetWord(false));

    testRegister.SetFlag(GBcc::SharpFlags::CARRY, true);

    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::ZERO));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::NOT_ADD));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::HALF));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::CARRY));
    Expect((u16)0x00F0, testRegister.GetDoubleWord());
    Expect( (u8)0x00, testRegister.GetWord(true));
    Expect( (u8)0xF0, testRegister.GetWord(false));

    testRegister.SetDoubleWord(0xFFAE);
    
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::ZERO));
    ExpectFalse(testRegister.FlagIsRaised(GBcc::SharpFlags::NOT_ADD));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::HALF));
    ExpectFalse(testRegister.FlagIsRaised(GBcc::SharpFlags::CARRY));
    Expect((u16)0xFFA0, testRegister.GetDoubleWord());
    Expect( (u8)0xFF, testRegister.GetWord(true));
    Expect( (u8)0xA0, testRegister.GetWord(false));

    testRegister.SetWord(true, 0xEA);
    
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::ZERO));
    ExpectFalse(testRegister.FlagIsRaised(GBcc::SharpFlags::NOT_ADD));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::HALF));
    ExpectFalse(testRegister.FlagIsRaised(GBcc::SharpFlags::CARRY));
    Expect((u16)0xEAA0, testRegister.GetDoubleWord());
    Expect( (u8)0xEA, testRegister.GetWord(true));
    Expect( (u8)0xA0, testRegister.GetWord(false));

    testRegister.SetWord(false, 0xDF);
    
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::ZERO));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::NOT_ADD));
    ExpectFalse(testRegister.FlagIsRaised(GBcc::SharpFlags::HALF));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::CARRY));
    Expect((u16)0xEAD0, testRegister.GetDoubleWord());
    Expect( (u8)0xEA, testRegister.GetWord(true));
    Expect( (u8)0xD0, testRegister.GetWord(false));

    testRegister.SetFlag(GBcc::SharpFlags::ZERO, false);
    ExpectFalse(testRegister.FlagIsRaised(GBcc::SharpFlags::ZERO));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::NOT_ADD));
    ExpectFalse(testRegister.FlagIsRaised(GBcc::SharpFlags::HALF));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::CARRY));

    testRegister.SetFlag(GBcc::SharpFlags::ZERO, false);
    ExpectFalse(testRegister.FlagIsRaised(GBcc::SharpFlags::ZERO));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::NOT_ADD));
    ExpectFalse(testRegister.FlagIsRaised(GBcc::SharpFlags::HALF));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::CARRY));

    testRegister.SetFlag(GBcc::SharpFlags::NOT_ADD, true);
    ExpectFalse(testRegister.FlagIsRaised(GBcc::SharpFlags::ZERO));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::NOT_ADD));
    ExpectFalse(testRegister.FlagIsRaised(GBcc::SharpFlags::HALF));
    ExpectTrue(testRegister.FlagIsRaised(GBcc::SharpFlags::CARRY));

    return 0;
}