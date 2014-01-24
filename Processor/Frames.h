#pragma once

#include "ID3.h"

class Frames
{
    
public:
    Frames();
    ~Frames();

    static bool IsValidFrameIdChar(char c);
    static ID3FrameType GetFrameType(const unsigned char* data);
    static unsigned int GetFrameSize(ID3_VERSION id3version, const unsigned char* sizeBytes);
};

