#include "stdafx.h"
#include "CppUnitTest.h"
#include "Frames.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

TEST_CLASS(FramesTest)
{
public:

    TEST_METHOD(Valid_Frame_Id_Char_Is_Allowed)
    {
        char sizeBytes[4] {0x00, 0x00, 0x00, 0x00};
        Assert::AreEqual(true, Frames::IsValidFrameIdChar('A'));
    }

    TEST_METHOD(Invalid_Frame_Id_Char_Is_Not_Allowed)
    {
        char sizeBytes[4] {0x00, 0x00, 0x00, 0x00};
        Assert::AreEqual(false, Frames::IsValidFrameIdChar('a'));
    }

    TEST_METHOD(Valid_Frame_Type_Can_Be_Found)
    {
        string type("TIT1");
        Assert::AreEqual(string("TIT1"), Frames::GetFrameType((unsigned char*)type.c_str()).name);
    }

    TEST_METHOD(Invalid_Frame_Type_Can_Not_Be_Found)
    {
        Assert::AreEqual(string("NONE"), Frames::GetFrameType((unsigned char*)"3456").name);
    }
};