#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utils.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(UtilsTest)
{
public:
		
	TEST_METHOD(Sync_Safe_Min_Size_Calculated_Correctly)
	{
        unsigned char bytes[4] {0x00, 0x00, 0x00, 0x00};
        Assert::AreEqual((unsigned int)0, Utils::SyncSafeBytesToUnsignedInt(bytes));
	}

    TEST_METHOD(Sync_Safe_Size_Calculated_Correctly)
    {
        unsigned char bytes[4] {0x01, 0x01, 0x01, 0x01};
        Assert::AreEqual((unsigned int)2113665, Utils::SyncSafeBytesToUnsignedInt(bytes));
    }

    TEST_METHOD(Sync_Safe_Max_Size_Calculated_Correctly)
    {
        unsigned char bytes[4] {0x7F, 0x7F, 0x7F, 0x7F};
        Assert::AreEqual((unsigned int)268435455, Utils::SyncSafeBytesToUnsignedInt(bytes));
    }

    TEST_METHOD(Bytes_To_Int_Min_Size_Calculated_Correctly)
    {
        unsigned char bytes[4] {0x00, 0x00, 0x00, 0x00};
        Assert::AreEqual((unsigned int)0, Utils::BytesToUnsignedInt(bytes));
    }

    TEST_METHOD(Bytes_To_Int_Size_Calculated_Correctly)
    {
        unsigned char bytes[4] {0x01, 0x01, 0x01, 0x01};
        Assert::AreEqual((unsigned int)16843009, Utils::BytesToUnsignedInt(bytes));
    }

    TEST_METHOD(Bytes_To_Int_Max_Size_Calculated_Correctly)
    {
        unsigned char bytes[4] {0xFF, 0xFF, 0xFF, 0xFF};
        Assert::AreEqual(UINT_MAX, Utils::BytesToUnsignedInt(bytes));
    }

    TEST_METHOD(Sync_Safe_Bytes_To_Long_Max_Size_Calculated_Correctly)
    {
        unsigned char bytes[8] {0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F};
        Assert::AreEqual((unsigned long)72057594037927935L, Utils::SyncSafeBytesToUnsignedLong(bytes, 8));
    }

    TEST_METHOD(Sync_Safe_Bytes_To_Long_Min_Size_Calculated_Correctly)
    {
        unsigned char bytes[8] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        Assert::AreEqual((unsigned long)0, Utils::SyncSafeBytesToUnsignedLong(bytes, 8));
    }

    TEST_METHOD(Sync_Safe_5_Bytes_To_Long_Size_Calculated_Correctly)
    {
        unsigned char bytes[5] {0x01, 0x01, 0x01, 0x01, 0x01};
        Assert::AreEqual((unsigned long)270549121L, Utils::SyncSafeBytesToUnsignedLong(bytes, 5));
    }

    TEST_METHOD(De_Unsynchronizing_A_Buffer_Works) 
    {
        unsigned char bytes[12] {0xFF, 0x00, 0xF2, 0x12, 0x12, 0xFF, 0x00, 0x00, 0x12, 0xFF, 0x00, 0xC0};
        unsigned char expected[9] {0xFF, 0xF2, 0x12, 0x12, 0xFF, 0x00, 0x12, 0xFF, 0xC0};

        unsigned int newSize;
        unsigned char* result = Utils::DeUnsynchronize(bytes, 12, newSize);

        Assert::AreEqual((unsigned int)9, newSize);
        for (int i = 0; i < 9; i++) {
            Assert::AreEqual(expected[i], result[i]);
        }

        delete[] result;
    }

    TEST_METHOD(De_Unsynchronizing_Another_Buffer_Works)
    {
        unsigned char bytes[25] {0x01, 0xFF, 0x00, 0xFE, 0x00, 0x6F, 0x00, 0x6B, 0x00, 0xFF, 0x00, 0xE0, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xEE, 0x00, 0xFF, 0x00, 0xF0, 0x00};
        unsigned char expected[20] {0x01, 0xFF, 0xFE, 0x00, 0x6F, 0x00, 0x6B, 0x00, 0xFF, 0xE0, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xEE, 0x00, 0xFF, 0xF0, 0x00};

        unsigned int newSize;
        unsigned char* result = Utils::DeUnsynchronize(bytes, 25, newSize);

        Assert::AreEqual((unsigned int)20, newSize);
        for (int i = 0; i < 20; i++) {
            Assert::AreEqual(expected[i], result[i]);
        }

        delete[] result;
    }

    TEST_METHOD(De_Unsynchronizing_A_One_Byte_Buffer_Works)
    {
        unsigned char bytes[] {0xFF};
        unsigned char expected[] {0xFF};

        unsigned int newSize;
        unsigned char* result = Utils::DeUnsynchronize(bytes, 1, newSize);

        Assert::AreEqual((unsigned char)0xFF, result[0]);

        Assert::AreEqual((unsigned int)1, newSize);

        delete[] result;
    }

    TEST_METHOD(De_Unsynchronizing_A_Zero_Byte_Buffer_Does_Nothing)
    {
        unsigned char* bytes = nullptr;;

        unsigned int newSize;
        unsigned char* result = Utils::DeUnsynchronize(bytes, 0, newSize);

        Assert::AreEqual((unsigned int)0, newSize);

        delete[] result;
    }
};


