#pragma once
#include "ID3.h"

class ID3Handler
{
    bool received = false;
    ID3 id3Data;
public:
    ID3Handler();
    ~ID3Handler();

    void Receive(ID3 id3);

    bool HasReceived();

    ID3 GetID3data();
};

