#include "Core/Sharp/SharpRegister.hpp"
#include "Types.hpp"

#include "TestFunctions.hpp"

using GBcc::u16;
using GBcc::u8;

int main(int argc, char** argv)
{
    GBcc::SharpRegister testRegister;

    testRegister.SetWord(false, 0x1E);

    Expect((u16)0x001E, testRegister.GetDoubleWord());
    Expect((u8)0x00, testRegister.GetWord(true));
    Expect((u8)0x1E, testRegister.GetWord(false));

    testRegister.SetWord(true, 0xAB);
    
    Expect((u16)0xAB1E, testRegister.GetDoubleWord());
    Expect((u8)0xAB, testRegister.GetWord(true));
    Expect((u8)0x1E, testRegister.GetWord(false));

    testRegister.SetDoubleWord(0x7766);

    Expect((u16)0x7766, testRegister.GetDoubleWord());
    Expect((u8)0x77, testRegister.GetWord(true));
    Expect((u8)0x66, testRegister.GetWord(false));

    return 0;
}