#include"stdafx.h"
#include <string>
#include"Frames.h"
#include <unordered_map>
#include "Utils.h"

using namespace std;

Frames::Frames()
{
}

Frames::~Frames()
{
}

bool Frames::IsValidFrameIdChar(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

ID3FrameType Frames::GetFrameType(const unsigned char* data) {
    
    string term = string(data, data + 4);

    map<string, ID3FrameType>::const_iterator result = FRAME_TYPES.find(term);

    if (result == FRAME_TYPES.end()) {
        return INVALID_ID3_FRAME_TYPE;
    }
    else {
        return result->second;
    }
}

unsigned int Frames::GetFrameSize(ID3_VERSION id3version, const unsigned char* sizeBytes) {
    if (id3version == ID3V2_4) {
        return Utils::SyncSafeBytesToUnsignedInt(sizeBytes);
    }
    else {
        return Utils::BytesToUnsignedInt(sizeBytes);
    }
}

