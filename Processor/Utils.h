#pragma once

class Utils
{
public:
    Utils();
    ~Utils();

    static unsigned int SyncSafeBytesToUnsignedInt(const unsigned char* bytes);
    static unsigned long SyncSafeBytesToUnsignedLong(const unsigned char* bytes, unsigned int numBytes);
    static unsigned int BytesToUnsignedInt(const unsigned char* bytes);
    static unsigned char* DeUnsynchronize(unsigned char* data, unsigned int size, unsigned int& newSize);
};

