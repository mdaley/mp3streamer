#include "stdafx.h"
#include "ID3Handler.h"

ID3Handler::ID3Handler()
{
}


ID3Handler::~ID3Handler()
{
}

void ID3Handler::Receive(ID3 id3) {
    this->id3Data = id3;
}

bool ID3Handler::HasReceived() {
    return this->received;
}

ID3 ID3Handler::GetID3data() {
    return id3Data;
}
