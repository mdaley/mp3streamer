#pragma once

#include <istream>
#include <functional>
#include "ID3.h"
#include "ID3Handler.h"

using namespace std;

class Processor
{

private:
    unsigned char* ExtractExtendedHeader(ID3* id3data, unsigned char* data, unsigned int size, unsigned int& newSize);
    unsigned char* ExtractExtendedHeader_v23(ID3* id3data, unsigned char* data, unsigned int size, unsigned int& newSize);
    unsigned char* ExtractExtendedHeader_v24(ID3* id3data, unsigned char* data, unsigned int size, unsigned int& newSize);
    void SetRestrictions(ID3* id3data, bool hasRestrictions, unsigned char* data, unsigned int restrictionsOffset);
    void SetFrameFlags(ID3* id3data, ID3Frame* frame, unsigned char* data);
    void ExtractTagFrames(ID3* id3data, unsigned char* data, unsigned int size);
    //void ExtractFrameData(ID3* id3data, ID3Frame* frame, unsigned char* frameData, unsigned int frameDataSize);
    //void ProcessTextFrameData(ID3* id3data, ID3Frame* frame, unsigned char* frameData, unsigned int frameDataSize);
public:
    Processor();
    virtual ~Processor();

    void Process(istream& stream, ID3Handler *id3Handler);

};

