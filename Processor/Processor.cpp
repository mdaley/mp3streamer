#include "stdafx.h"
#include "Processor.h"
#include <bitset>
#include <string>
#include "Frames.h"
#include "Utils.h"
#include "ID3Handler.h"

using namespace std;

enum PROCESSING_STATE {
    NOTHING_FOUND,
    ID3V2_FOUND,
    ID3V2_BODY_PROCESSING,
    COMPLETED
};

enum ID3_FLAG_BIT_POSITION {
    UNSYNCHRONIZATION = 7,
    EXTENDED_HEADER = 6,
    EXPERIMENTAL = 5,
    FOOTER_PRESENT = 4 // ID3 v2.4.0 only
};

enum ID3_FRAME_STATUS_BIT_POSITION {
    TAG_ALTER_PRESERVATION = 7,
    FILE_ALTER_PRESERVATION = 6,
    READ_ONLY = 5,
};

enum ID3_FRAME_FORMAT_BIT_POSITION_V2_3 {
    COMPRESSED_V2_3 = 7,
    ENCRYPTED_V2_3 = 6,
    GROUPED_V2_3 = 5
};

enum ID3_FRAME_FORMAT_BIT_POSITION_V2_4 {
    GROUPED_V2_4 = 6,
    COMPRESSED_V2_4 = 3,
    ENCRYPTED_V2_4 = 2,
    UNSYNCHRONIZED_V2_4 = 1,
    DATA_LENGTH_INDICATOR_PRESENT_V2_4 = 0
};

enum ID3_EXTENDED_HEADER_BIT_POSITION {
    CRC_DATA_PRESENT_V2_3 = 7,
    CRC_DATA_PRESENT_V2_4 = 5,
    TAG_IS_AN_UPDATE = 6,
    TAG_RESTRICTIONS_PRESENT = 4
};

Processor::Processor()
{
}


Processor::~Processor()
{
}

void Processor::Process(istream& stream, ID3Handler *id3Handler) {

    std::function<void(ID3)> callback = std::bind(&ID3Handler::Receive, id3Handler, std::placeholders::_1);

    int id3 = 0;
    unsigned char* data = NULL;
    unsigned int dataSize = 0;
    unsigned int dataCurrent = 0;
    unsigned char lookback[11] {'\0'}; // 11 chars used (10 in the past + 1 current).

    PROCESSING_STATE state = NOTHING_FOUND;

    ID3 id3data;

    while (stream.good()) {
        int c = stream.get();

        for (int j = 1; j < 11; j++) {
            lookback[j - 1] = lookback[j];
        }
        lookback[10] = c;
        
        if (state == ID3V2_FOUND) {
            int minor = lookback[3];
            id3data.version = (minor == 3) ? ID3V2_3 : ID3V2_4;

            bitset<8> bits(lookback[5]);
            id3data.unsynchronized = bits[ID3_FLAG_BIT_POSITION::UNSYNCHRONIZATION];
            id3data.extendedHeader = bits[ID3_FLAG_BIT_POSITION::EXTENDED_HEADER];
            id3data.experimental = bits[ID3_FLAG_BIT_POSITION::EXPERIMENTAL];
            id3data.footerPresent = bits[ID3_FLAG_BIT_POSITION::FOOTER_PRESENT];

            unsigned int size = Utils::SyncSafeBytesToUnsignedInt(&lookback[6]);
            size = id3data.footerPresent ? (size + 10L) : size;
            id3data.size = size;

            data = new unsigned char[size + 1];
            dataCurrent = 0;
            dataSize = size;

            state = ID3V2_BODY_PROCESSING;
        }

        if (state == ID3V2_BODY_PROCESSING && dataCurrent == dataSize) {

            ExtractTagFrames(&id3data, data, dataSize);
            delete[] data;
            callback(id3data);

            state = COMPLETED;
        }

        if (state == ID3V2_BODY_PROCESSING) {
            data[dataCurrent++] = c;
        }

        if (state == NOTHING_FOUND) {
            if (c == 'I' || c == 'D' && id3 == 1 || c == '3' && id3 == 2 || c < 0xFF && (id3 == 3 || id3 == 4) || (c | 0xF0) == 0xF0 && id3 == 5 || c < 0x80 && (id3 >= 6 && id3 <= 9)) {
                id3++;
            }
            else {
                id3 = 0;
            }

            if (id3 == 10) {
                state = ID3V2_FOUND;
            }
        }
    }
}

unsigned char* Processor::ExtractExtendedHeader(ID3* id3data, unsigned char* data, unsigned int size, unsigned int& newSize) {
    if (id3data->extendedHeader) {
        if (id3data->version == ID3V2_3) {
            return ExtractExtendedHeader_v23(id3data, data, size, newSize);
        }
        else { //ID3V2_4
            return ExtractExtendedHeader_v24(id3data, data, size, newSize);
        }
    }
    else {
        newSize = size;
        return data;
    }
}

unsigned char* Processor::ExtractExtendedHeader_v23(ID3* id3data, unsigned char* data, unsigned int size, unsigned int& newSize) {
    if (size >= 6) {
        unsigned int headerSize = Utils::BytesToUnsignedInt(data);

        if (headerSize <= size) {
            bitset<8> bits(*(data + 4));
            bool hasCrc = bits[ID3_EXTENDED_HEADER_BIT_POSITION::CRC_DATA_PRESENT_V2_3];
            id3data->hasCrc = hasCrc;

            if (hasCrc) {
                id3data->crc = Utils::BytesToUnsignedInt(data + 6);
            }

            newSize = size - headerSize;
            return (data + headerSize);
        }
    }

    newSize = size;
    return data;
}

unsigned char* Processor::ExtractExtendedHeader_v24(ID3* id3data, unsigned char* data, unsigned int size, unsigned int& newSize) {
    if (size >= 6) {
        unsigned int headerSize = Utils::BytesToUnsignedInt(data);

        if (headerSize <= size) {
            bitset<8> bits(*(data + 5));
            bool hasCrc = bits[ID3_EXTENDED_HEADER_BIT_POSITION::CRC_DATA_PRESENT_V2_4];
            bool tagIsUpdate = bits[ID3_EXTENDED_HEADER_BIT_POSITION::TAG_IS_AN_UPDATE];
            bool hasRestrictions = bits[ID3_EXTENDED_HEADER_BIT_POSITION::TAG_RESTRICTIONS_PRESENT];
            id3data->hasCrc = hasCrc;
            id3data->tagIsUpdate = tagIsUpdate;
            id3data->hasRestrictions = hasRestrictions;

            unsigned int crcOffset = 7 + (tagIsUpdate ? 1 : 0);
            unsigned int restrictionsOffset = 7 + (tagIsUpdate ? 1 : 0) + (hasCrc ? 6 : 0);

            if (hasCrc) {
                id3data->crc = Utils::SyncSafeBytesToUnsignedLong(data + crcOffset, 5);
            }

            SetRestrictions(id3data, hasRestrictions, data, restrictionsOffset);

            newSize = size - headerSize;
            return (data + headerSize);
        }
    }

    newSize = size;
    return data;
}

void Processor::SetRestrictions(ID3* id3data, bool hasRestrictions, unsigned char* data, unsigned int restrictionsOffset) {
    if (hasRestrictions) {
        bitset<8> rbits(*(data + restrictionsOffset));
        unsigned int tagSizeRestrictionValue = (rbits[7] << 1) + rbits[6];
        unsigned int textSizeRestrictionValue = (rbits[4] << 1) + rbits[3];
        unsigned int imageSizeRestrictionValue = (rbits[1] << 1) + rbits[0];

        switch (tagSizeRestrictionValue) {
        case 0:
            id3data->tagSizeRestriction = TAG_SIZE_RESTRICTION::MAX_128_FRAMES_1MB_TOTAL_SIZE;
            break;
        case 1:
            id3data->tagSizeRestriction = TAG_SIZE_RESTRICTION::MAX_64_FRAMES_128KB_TOTAL_SIZE;
            break;
        case 2:
            id3data->tagSizeRestriction = TAG_SIZE_RESTRICTION::MAX_32_FRAMES_40KB_TOTAL_SIZE;
            break;
        default:
            id3data->tagSizeRestriction = TAG_SIZE_RESTRICTION::MAX_32_FRAMES_4KB_TOTAL_SIZE;
        }

        id3data->textEncodingRestriction = rbits[5] ? TEXT_ENCODING_RESTRICTION::ONLY_ISO_8859_OR_UTF8 : TEXT_ENCODING_RESTRICTION::NO_TEXT_ENCODING_RESTRICTION;

        switch (textSizeRestrictionValue) {
        case 0:
            id3data->textFieldSizeRestriction = TEXT_FIELD_SIZE_RESTRICTION::NO_TEXT_FIELD_SIZE_RESTRICTION;
            break;
        case 1:
            id3data->textFieldSizeRestriction = TEXT_FIELD_SIZE_RESTRICTION::MAX_1024_CHARACTERS;
            break;
        case 2:
            id3data->textFieldSizeRestriction = TEXT_FIELD_SIZE_RESTRICTION::MAX_128_CHARACTERS;
            break;
        default:
            id3data->textFieldSizeRestriction = TEXT_FIELD_SIZE_RESTRICTION::MAX_30_CHARACTERS;
        }

        id3data->imageEncodingRestriction = rbits[2] ? IMAGE_ENCODING_RESTRICTION::ONLY_PNG_OR_JPG : IMAGE_ENCODING_RESTRICTION::NO_IMAGE_ENCODING_RESTRICTION;

        switch (imageSizeRestrictionValue) {
        case 0:
            id3data->imageSizeRestriction = IMAGE_SIZE_RESTRICTION::NO_IMAGE_SIZE_RESTRICTION;
            break;
        case 1:
            id3data->imageSizeRestriction = IMAGE_SIZE_RESTRICTION::MAX_256_X_256;
            break;
        case 2:
            id3data->imageSizeRestriction = IMAGE_SIZE_RESTRICTION::MAX_64_X_64;
            break;
        default:
            id3data->imageSizeRestriction = IMAGE_SIZE_RESTRICTION::EXACTLY_64_X_64_UNLESS_REQUIRED_OTHERSIZE;
        }

    }
    else {
        id3data->imageEncodingRestriction = IMAGE_ENCODING_RESTRICTION::NO_IMAGE_ENCODING_RESTRICTION;
        id3data->imageSizeRestriction = IMAGE_SIZE_RESTRICTION::NO_IMAGE_SIZE_RESTRICTION;
        id3data->tagSizeRestriction = TAG_SIZE_RESTRICTION::NO_TAG_SIZE_RESTRICTION;
        id3data->textEncodingRestriction = TEXT_ENCODING_RESTRICTION::NO_TEXT_ENCODING_RESTRICTION;
        id3data->textFieldSizeRestriction = TEXT_FIELD_SIZE_RESTRICTION::NO_TEXT_FIELD_SIZE_RESTRICTION;
    }
}

enum FRAME_PROCESSING_STATE {
    NO_FRAME,
    FRAME_FOUND,
    FRAME_PROCESSING
};

enum FRAME_FLAGS {
    FRAME_STATUS_FLAG_MASK = 0xE0,
    FRAME_FORMAT_FLAG_MASK = 0xEF
};

void Processor::ExtractTagFrames(ID3* id3data, unsigned char* incomingData, unsigned int incomingSize) {

    if (incomingData == 0) {
        return;
    }

    // De-unsynchronize if necessary
    unsigned char* dataAfterDeUnsynchronization = nullptr;
    unsigned int sizeAfterDeunsynchronization = 0;

    // TODO: Replace this try catch with use of unique_ptr.
    try {
        if (id3data->unsynchronized) {
            dataAfterDeUnsynchronization = Utils::DeUnsynchronize(incomingData, incomingSize, sizeAfterDeunsynchronization);
        }
        else {
            dataAfterDeUnsynchronization = incomingData;
            sizeAfterDeunsynchronization = incomingSize;
        }

        // Extract extended header
        unsigned int size;
        unsigned char* data = ExtractExtendedHeader(id3data, dataAfterDeUnsynchronization, sizeAfterDeunsynchronization, size);

        unsigned int nextFrameStart = 0;
        while (nextFrameStart < size) {
            ID3FrameType frameType = Frames::GetFrameType(data);
            data += 4;
            unsigned int frameSize = Frames::GetFrameSize(id3data->version, data);
            data += 4;
            unsigned char* flags = data;
            data += 2;
            unsigned char* frameData = data;
            data += frameSize;
            nextFrameStart += frameSize + 10;
            fprintf(stdout, "Frame %s, size %u\n", frameType.name.c_str(), frameSize);

            if (frameType.name != "NONE") {
                ID3Frame frame;
                frame.name = frameType.name;
                frame.type = frameType.type;
                frame.size = frameSize;
                SetFrameFlags(id3data, &frame, flags);

                if (frame.unsynchronized) {
                    unsigned int deunsynchedFrameSize;
                    unsigned char* deunsynchedFrameData = Utils::DeUnsynchronize(frameData, frameSize, deunsynchedFrameSize);
                    frame.frameData.assign((char*)deunsynchedFrameData, deunsynchedFrameSize);
                    frame.size = deunsynchedFrameSize;
                }
                else {
                    frame.frameData.assign((char*)frameData, frameSize);
                }
                id3data->frames.emplace(frameType.name, frame);
            }
        }
    }
    catch (...) {
        if (id3data->unsynchronized && dataAfterDeUnsynchronization != nullptr) {
            delete[] dataAfterDeUnsynchronization;
        }
    }

    if (id3data->unsynchronized) {
        delete[] dataAfterDeUnsynchronization;
    }

}

void Processor::SetFrameFlags(ID3* id3data, ID3Frame* frame, unsigned char* data) {
    bitset<8> statusBits(*data);
    frame->tagAlterPreservation = statusBits[ID3_FRAME_STATUS_BIT_POSITION::TAG_ALTER_PRESERVATION];
    frame->fileAlterPreservation = statusBits[ID3_FRAME_STATUS_BIT_POSITION::FILE_ALTER_PRESERVATION];
    frame->readOnly = statusBits[ID3_FRAME_STATUS_BIT_POSITION::READ_ONLY];

    bitset<8> formatBits(*(data + 1));
    if (id3data->version == ID3V2_3) {
        frame->compressed = formatBits[ID3_FRAME_FORMAT_BIT_POSITION_V2_3::COMPRESSED_V2_3];
        frame->encrypted = formatBits[ID3_FRAME_FORMAT_BIT_POSITION_V2_3::ENCRYPTED_V2_3];
        frame->grouped = formatBits[ID3_FRAME_FORMAT_BIT_POSITION_V2_3::GROUPED_V2_3];
    }
    else { // ID3V2_4
        frame->compressed = formatBits[ID3_FRAME_FORMAT_BIT_POSITION_V2_4::COMPRESSED_V2_4];
        frame->encrypted = formatBits[ID3_FRAME_FORMAT_BIT_POSITION_V2_4::ENCRYPTED_V2_4];
        frame->grouped = formatBits[ID3_FRAME_FORMAT_BIT_POSITION_V2_4::GROUPED_V2_4];
        frame->unsynchronized = formatBits[ID3_FRAME_FORMAT_BIT_POSITION_V2_4::UNSYNCHRONIZED_V2_4];
        frame->dataLengthIndicator = formatBits[ID3_FRAME_FORMAT_BIT_POSITION_V2_4::DATA_LENGTH_INDICATOR_PRESENT_V2_4];
    }
}

