#include "Core/Sharp/SharpRegister.hpp"
#include "Types.hpp"

#include "TestFunctions.hpp"

using GBcc::u16;
using GBcc::u8;

int main(int argc, char** argv)
{
    GBcc::SharpRegister testRegister(0xEBFC);
    GBcc::SharpRegister testRegister2(0xABED);

    testRegister2 = std::move(testRegister);

    Expect((u16)0xEBFC, testRegister2.GetDoubleWord());
    Expect((u8)0xEB, testRegister2.GetWord(true));
    Expect((u8)0xFC, testRegister2.GetWord(false));

    return 0;
}