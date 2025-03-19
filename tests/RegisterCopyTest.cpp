#include "Core/Sharp/SharpRegister.hpp"
#include "Types.hpp"

#include "TestFunctions.hpp"

using GBcc::u16;
using GBcc::u8;

int main(int argc, char** argv)
{
    GBcc::SharpRegister testRegister(0xEBFC);
    GBcc::SharpRegister testRegister2(testRegister);

    Expect((u16)0xEBFC, testRegister2.GetDoubleWord());
    Expect((u8)0xEB, testRegister2.GetWord(true));
    Expect((u8)0xFC, testRegister2.GetWord(false));

    GBcc::SharpRegister testRegister3;

    testRegister3 = testRegister2;

    Expect((u16)0xEBFC, testRegister3.GetDoubleWord());
    Expect((u8)0xEB, testRegister3.GetWord(true));
    Expect((u8)0xFC, testRegister3.GetWord(false));

    return 0;
}