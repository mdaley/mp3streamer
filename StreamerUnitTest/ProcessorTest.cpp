#include "stdafx.h"
#include "CppUnitTest.h"
#include "Processor.h"
#include <fstream>
#include "ID3Handler.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ProcessorTest)
{
public:
    TEST_METHOD(Can_Obtain_ID3_Data_From_ID3V2_3_File)
    {
        ifstream s("TestData//Alone_Together_ID3_Data.mp3", ifstream::in | ifstream::binary);
        
        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual(true, data.version == ID3V2_3);
        Assert::AreEqual(false, data.experimental);
        Assert::AreEqual(false, data.extendedHeader);
        Assert::AreEqual(false, data.unsynchronized);
        Assert::AreEqual(false, data.footerPresent);

    }

    TEST_METHOD(Flags_Can_Be_Retrieved_Correctly_From_ID3V2_3_File)
    {
        ifstream s("TestData//ID3_23_FlagsTrue.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual(true, data.version == ID3V2_3);
        Assert::AreEqual(true, data.experimental);
        Assert::AreEqual(true, data.extendedHeader);
        Assert::AreEqual(true, data.unsynchronized);
        Assert::AreEqual(false, data.footerPresent);
        Assert::AreEqual((unsigned int)50, data.size);
    }

    TEST_METHOD(Extended_Footer_Flag_In_ID3V2_4_File_Results_In_Adding_10_To_Size)
    {
        ifstream s("TestData//ID3_24_ExtendedFooter.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual(true, data.version == ID3V2_4);
        Assert::AreEqual(true, data.experimental);
        Assert::AreEqual(true, data.extendedHeader);
        Assert::AreEqual(true, data.unsynchronized);
        Assert::AreEqual(true, data.footerPresent);
        Assert::AreEqual((unsigned int)60, data.size);
    }

    TEST_METHOD(Frames_Can_Be_Retrieved_Correctly_From_ID3V2_3_File)
    {
        ifstream s("TestData//ID3_23_FlagsTrue.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual((unsigned int)2, (unsigned int)data.frames.size());
        
        Assert::AreEqual(string("TIT1"), data.frames.find("TIT1")->second.name);
        Assert::AreEqual((unsigned int)5, data.frames.find("TIT1")->second.size);
        Assert::AreEqual(string("TENC"), data.frames.find("TENC")->second.name);
        Assert::AreEqual((unsigned int)25, data.frames.find("TENC")->second.size);
    }

    TEST_METHOD(Frame_Flags_Are_Correctly_Retrieved_For_ID3V2_3_File)
    {
        ifstream s("TestData//ID3_23_FlagsTrue.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual((unsigned int)2, (unsigned int)data.frames.size());

        Assert::AreEqual(string("TIT1"), data.frames.find("TIT1")->second.name);
        Assert::AreEqual(false, data.frames.find("TIT1")->second.tagAlterPreservation);
        Assert::AreEqual(false, data.frames.find("TIT1")->second.fileAlterPreservation);
        Assert::AreEqual(false, data.frames.find("TIT1")->second.readOnly);
        Assert::AreEqual(false, data.frames.find("TIT1")->second.grouped);
        Assert::AreEqual(false, data.frames.find("TIT1")->second.encrypted);
        Assert::AreEqual(false, data.frames.find("TIT1")->second.compressed);

        Assert::AreEqual(string("TENC"), data.frames.find("TENC")->second.name);
        Assert::AreEqual(true, data.frames.find("TENC")->second.tagAlterPreservation);
        Assert::AreEqual(true, data.frames.find("TENC")->second.fileAlterPreservation);
        Assert::AreEqual(true, data.frames.find("TENC")->second.readOnly);
        Assert::AreEqual(true, data.frames.find("TENC")->second.grouped);
        Assert::AreEqual(true, data.frames.find("TENC")->second.encrypted);
        Assert::AreEqual(true, data.frames.find("TENC")->second.compressed);
    }

    TEST_METHOD(Frame_Flags_Are_Correctly_Retrieved_For_ID3V2_4_File)
    {
        ifstream s("TestData//ID3_24_ExtendedFooter.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual((unsigned int)2, (unsigned int)data.frames.size());

        Assert::AreEqual(string("TIT1"), data.frames.find("TIT1")->second.name);
        Assert::AreEqual(false, data.frames.find("TIT1")->second.grouped);
        Assert::AreEqual(false, data.frames.find("TIT1")->second.encrypted);
        Assert::AreEqual(false, data.frames.find("TIT1")->second.compressed);
        Assert::AreEqual(false, data.frames.find("TIT1")->second.unsynchronized);
        Assert::AreEqual(false, data.frames.find("TIT1")->second.dataLengthIndicator);

        Assert::AreEqual(string("TENC"), data.frames.find("TENC")->second.name);
        Assert::AreEqual(true, data.frames.find("TENC")->second.grouped);
        Assert::AreEqual(true, data.frames.find("TENC")->second.encrypted);
        Assert::AreEqual(true, data.frames.find("TENC")->second.compressed);
        Assert::AreEqual(true, data.frames.find("TENC")->second.unsynchronized);
        Assert::AreEqual(true, data.frames.find("TENC")->second.dataLengthIndicator);
    }

    TEST_METHOD(ID3_V2_3_File_Has_Extended_Header_With_No_CRC)
    {
        ifstream s("TestData//ID3_23_ExtendedHeaderNoCRC.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual(true, data.extendedHeader);
        Assert::AreEqual(false, data.hasCrc);
        Assert::AreEqual((unsigned int)2, data.frames.size());
    }

    TEST_METHOD(ID3_V2_3_File_Has_Extended_Header_With_CRC)
    {
        ifstream s("TestData//ID3_23_ExtendedHeaderWithCRC.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual(true, data.extendedHeader);
        Assert::AreEqual(true, data.hasCrc);
        Assert::AreEqual((unsigned long)0x12345678, data.crc);
        Assert::AreEqual((unsigned int)2, data.frames.size());
    }

    TEST_METHOD(ID3_V2_4_File_Has_Extended_Header_With_No_Flags_Set)
    {
        ifstream s("TestData//ID3_24_ExtendedHeaderNoFlags.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual(true, data.extendedHeader);
        Assert::AreEqual(false, data.tagIsUpdate);
        Assert::AreEqual(false, data.hasCrc);
        Assert::AreEqual(false, data.hasRestrictions);
        Assert::AreEqual(true, IMAGE_ENCODING_RESTRICTION::NO_IMAGE_ENCODING_RESTRICTION == data.imageEncodingRestriction);
        Assert::AreEqual(true, IMAGE_SIZE_RESTRICTION::NO_IMAGE_SIZE_RESTRICTION == data.imageSizeRestriction);
        Assert::AreEqual(true, TEXT_ENCODING_RESTRICTION::NO_TEXT_ENCODING_RESTRICTION == data.textEncodingRestriction);
        Assert::AreEqual(true, TEXT_FIELD_SIZE_RESTRICTION::NO_TEXT_FIELD_SIZE_RESTRICTION == data.textFieldSizeRestriction);
        Assert::AreEqual(true, TAG_SIZE_RESTRICTION::NO_TAG_SIZE_RESTRICTION == data.tagSizeRestriction);
    }

    TEST_METHOD(ID3_V2_4_File_Has_Extended_Header_With_CRC_Only)
    {
        ifstream s("TestData//ID3_24_ExtendedHeaderCRCOnly.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual(true, data.extendedHeader);
        Assert::AreEqual(false, data.tagIsUpdate);
        Assert::AreEqual(true, data.hasCrc);
        Assert::AreEqual(false, data.hasRestrictions);
        Assert::AreEqual((unsigned long)535055888L, data.crc);
    }

    TEST_METHOD(ID3_V2_4_File_Has_Extended_Header_With_Restrictions_Only)
    {
        ifstream s("TestData//ID3_24_ExtendedHeaderRestrOnly.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual(true, data.extendedHeader);
        Assert::AreEqual(false, data.tagIsUpdate);
        Assert::AreEqual(false, data.hasCrc);
        Assert::AreEqual(true, data.hasRestrictions);
        Assert::AreEqual(true, data.imageEncodingRestriction == IMAGE_ENCODING_RESTRICTION::ONLY_PNG_OR_JPG);
        Assert::AreEqual(true, data.imageSizeRestriction == IMAGE_SIZE_RESTRICTION::EXACTLY_64_X_64_UNLESS_REQUIRED_OTHERSIZE);
        Assert::AreEqual(true, data.tagSizeRestriction == TAG_SIZE_RESTRICTION::MAX_32_FRAMES_4KB_TOTAL_SIZE);
        Assert::AreEqual(true, data.textEncodingRestriction == TEXT_ENCODING_RESTRICTION::ONLY_ISO_8859_OR_UTF8);
        Assert::AreEqual(true, data.textFieldSizeRestriction == TEXT_FIELD_SIZE_RESTRICTION::MAX_30_CHARACTERS);
    }

    TEST_METHOD(ID3_V2_4_File_Has_Extended_Header_With_All_Flags_Set)
    {
        ifstream s("TestData//ID3_24_ExtendedHeaderFull.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual(true, data.extendedHeader);
        
        Assert::AreEqual(true, data.tagIsUpdate);
        Assert::AreEqual(true, data.hasCrc);
        Assert::AreEqual((unsigned long)535055888L, data.crc);

        Assert::AreEqual(true, data.hasRestrictions);
        Assert::AreEqual(true, data.imageEncodingRestriction == IMAGE_ENCODING_RESTRICTION::ONLY_PNG_OR_JPG);
        Assert::AreEqual(true, data.imageSizeRestriction == IMAGE_SIZE_RESTRICTION::EXACTLY_64_X_64_UNLESS_REQUIRED_OTHERSIZE);
        Assert::AreEqual(true, data.tagSizeRestriction == TAG_SIZE_RESTRICTION::MAX_32_FRAMES_4KB_TOTAL_SIZE);
        Assert::AreEqual(true, data.textEncodingRestriction == TEXT_ENCODING_RESTRICTION::ONLY_ISO_8859_OR_UTF8);
        Assert::AreEqual(true, data.textFieldSizeRestriction == TEXT_FIELD_SIZE_RESTRICTION::MAX_30_CHARACTERS);
    }

    TEST_METHOD(ID3_V2_4_File_Has_Frame_De_Unsynchronized_Correctly)
    {
        ifstream s("TestData//ID3_24_UnsynchFrame.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();
        
        Assert::AreEqual(string("TENC"), data.frames.find("TENC")->second.name);
        Assert::AreEqual(true, data.frames.find("TENC")->second.unsynchronized);
        //Assert::AreEqual((unsigned int)20, data.frames.find("TENC")->second.size); // size is actually 25 but there are 4 bytes to remove.

        unsigned char* d = (unsigned char*)data.frames.find("TENC")->second.frameData.c_str();

        unsigned char expected[20] {0x01, 0xFF, 0xFE, 0x00, 0x6F, 0x00, 0x6B, 0x00, 0xFF, 0xE0, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xEE, 0x00, 0xFF, 0xF0, 0x00};

        for (int i = 0; i < 16;  i++) {
            Assert::AreEqual(expected[i], d[i]);
        }
    }
};


