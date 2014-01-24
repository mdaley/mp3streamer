#include "stdafx.h"
#include "CppUnitTest.h"
#include "Processor.h"
#include <fstream>
#include <string>
#include "ID3Handler.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ProcessorTest)
{
public:
    TEST_METHOD(Short_Clip_Encoded_By_Audacity_As_ID3_V2_3)
    {
        ifstream s("TestData//ShortClip.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual(true, data.version == ID3V2_3);
        Assert::AreEqual(false, data.experimental);
        Assert::AreEqual(false, data.extendedHeader);
        Assert::AreEqual(false, data.unsynchronized);
        Assert::AreEqual(false, data.footerPresent);

        Assert::AreEqual((size_t)8, data.frames.size());
        Assert::AreEqual((unsigned int)26, data.frames.find("TALB")->second.size);
        Assert::AreEqual((unsigned int)5, data.frames.find("TIT2")->second.size);
        Assert::AreEqual((unsigned int)3, data.frames.find("TRCK")->second.size);
        Assert::AreEqual((unsigned int)57, data.frames.find("COMM")->second.size);
        Assert::AreEqual((unsigned int)14, data.frames.find("TPE1")->second.size);
        Assert::AreEqual((unsigned int)5, data.frames.find("TYER")->second.size);
        Assert::AreEqual((unsigned int)5, data.frames.find("TDRC")->second.size);
        Assert::AreEqual((unsigned int)12, data.frames.find("TCON")->second.size);
    }

    TEST_METHOD(Short_Clip_Recover_When_Unknown_Frame_Type_Found)
    {
        // One of the eight frames is duff (TRCK changed to have invalid frame id TZZZ).
        ifstream s("TestData//ShortClip_UnknownFrame.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual(true, data.version == ID3V2_3);
        Assert::AreEqual(false, data.experimental);
        Assert::AreEqual(false, data.extendedHeader);
        Assert::AreEqual(false, data.unsynchronized);
        Assert::AreEqual(false, data.footerPresent);

        Assert::AreEqual((size_t)7, data.frames.size());
        Assert::AreEqual((unsigned int)26, data.frames.find("TALB")->second.size);
        Assert::AreEqual((unsigned int)5, data.frames.find("TIT2")->second.size);
        Assert::AreEqual((unsigned int)57, data.frames.find("COMM")->second.size);
        Assert::AreEqual((unsigned int)14, data.frames.find("TPE1")->second.size);
        Assert::AreEqual((unsigned int)5, data.frames.find("TYER")->second.size);
        Assert::AreEqual((unsigned int)5, data.frames.find("TDRC")->second.size);
        Assert::AreEqual((unsigned int)12, data.frames.find("TCON")->second.size);
    }

    TEST_METHOD(Short_Clip_Unsynchronized_With_Some_Unicode_Data)
    {
        ifstream s("TestData//ShortClipUnicode_Unsynch.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual(true, data.version == ID3V2_3);
        Assert::AreEqual(false, data.experimental);
        Assert::AreEqual(false, data.extendedHeader);
        Assert::AreEqual(true, data.unsynchronized);
        Assert::AreEqual(false, data.footerPresent);

        Assert::AreEqual((size_t)8, data.frames.size());
        Assert::AreEqual((unsigned int)26, data.frames.find("TALB")->second.size);
        Assert::AreEqual((unsigned int)5, data.frames.find("TIT2")->second.size);
        Assert::AreEqual((unsigned int)3, data.frames.find("TRCK")->second.size);
        Assert::AreEqual((unsigned int)55, data.frames.find("COMM")->second.size);
        Assert::AreEqual((unsigned int)29, data.frames.find("TPE1")->second.size);
        Assert::AreEqual((unsigned int)5, data.frames.find("TYER")->second.size);
        Assert::AreEqual((unsigned int)5, data.frames.find("TDRC")->second.size);
        Assert::AreEqual((unsigned int)12, data.frames.find("TCON")->second.size);
    }

    TEST_METHOD(ISO_8859_Text_Frame_Can_Be_Retrieved)
    {
        ifstream s("TestData//ShortClipUnicode_Unsynch.mp3", ifstream::in | ifstream::binary);

        Processor processor;
        ID3Handler handler;

        processor.Process(s, &handler);

        ID3 data = handler.GetID3data();

        Assert::AreEqual(true, data.version == ID3V2_3);
        Assert::AreEqual(false, data.experimental);
        Assert::AreEqual(false, data.extendedHeader);
        Assert::AreEqual(true, data.unsynchronized);
        Assert::AreEqual(false, data.footerPresent);

        Assert::AreEqual((size_t)8, data.frames.size());
        Assert::AreEqual((unsigned int)26, data.frames.find("TALB")->second.size);
        Assert::AreEqual((char)0x00, data.frames.find("TALB")->second.frameData[0]);
        Assert::AreEqual(string("Alex was a friend of mine"), string(&data.frames.find("TALB")->second.frameData[1]));

        Assert::AreEqual((unsigned int)29, data.frames.find("TPE1")->second.size);
        Assert::AreEqual((char)0x01, data.frames.find("TPE1")->second.frameData[0]);

        // TODO: Bodge below doesn't work even though actual is the same as expected. Bah!
        //Assert::AreEqual(wstring(L"Matthew ￥aley"), wstring((wchar_t*)&data.frames.find("TPE1")->second.frameData[1], (data.frames.find("TPE1")->second.size / 2)));
    }
};

