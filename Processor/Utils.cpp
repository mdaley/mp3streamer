#include "stdafx.h"
#include "Utils.h"

Utils::Utils()
{
}

Utils::~Utils()
{
}

unsigned int Utils::SyncSafeBytesToUnsignedInt(const unsigned char* bytes) {
    unsigned int result = 0;

    result = *bytes;
    for (unsigned int i = 0; i < 3; i++) {
        result = result << 7;
        bytes++;
        result = result | *bytes;
    }

    return result;
}

unsigned long Utils::SyncSafeBytesToUnsignedLong(const unsigned char* bytes, unsigned int numBytes) {
    unsigned long result = 0;
    unsigned int afterFirstByteCount = numBytes - 1;

    result = *bytes;
    for (unsigned int i = 0; i < afterFirstByteCount; i++) {
        result = result << 7;
        bytes++;
        result = result | *bytes;
    }

    return result;
}

unsigned int Utils::BytesToUnsignedInt(const unsigned char* bytes) {
    unsigned int result = 0;

    result = *bytes;
    for (unsigned int i = 0; i < 3; i++) {
        result = result << 8;
        bytes++;
        result = result | *bytes;
    }

    return result;
}

unsigned char* Utils::DeUnsynchronize(unsigned char* data, unsigned int size, unsigned int& newSize) {

    if (size == 0) {
        newSize = 0;
        return data;
    }

    unsigned char* result = new unsigned char[size];
    unsigned char* r = result;

    *r++ = *data++; 
    newSize = 1;

    for (unsigned int i = 1; i < size; i++) {
        if (*data == 0x00 && *(data - 1) == 0xFF) {
            data++;
        }
        else {
            *r++ = *data++;
            newSize++;
        }
    }

    return result;
}
