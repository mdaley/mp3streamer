#pragma once
#include <map>

using namespace std;

enum ID3_VERSION {
    ID3V1,
    ID3V2_3,
    ID3V2_4,
    ID3V_UNKNOWN
};

enum ID3_FRAME_TYPE {
    TEXT,
    BINARY,
    UTF8,
    LATIN1,
    LATIN1FULL,
    LATIN1LIST,
    STRING,
    STRINGFULL,
    STRINGLIST,
    LANGUAGE,
    FRAMEID,
    DATE,
    INT8,
    INT16,
    INT24,
    INT32,
    INT32PLUS,
    INVALID
};

enum TAG_SIZE_RESTRICTION {
    MAX_128_FRAMES_1MB_TOTAL_SIZE,
    MAX_64_FRAMES_128KB_TOTAL_SIZE,
    MAX_32_FRAMES_40KB_TOTAL_SIZE,
    MAX_32_FRAMES_4KB_TOTAL_SIZE,
    NO_TAG_SIZE_RESTRICTION
};

enum TEXT_ENCODING_RESTRICTION {
    ONLY_ISO_8859_OR_UTF8,
    NO_TEXT_ENCODING_RESTRICTION
};

enum TEXT_FIELD_SIZE_RESTRICTION {
    MAX_1024_CHARACTERS,
    MAX_128_CHARACTERS,
    MAX_30_CHARACTERS,
    NO_TEXT_FIELD_SIZE_RESTRICTION
};

enum IMAGE_ENCODING_RESTRICTION {
    ONLY_PNG_OR_JPG,
    NO_IMAGE_ENCODING_RESTRICTION
};

enum IMAGE_SIZE_RESTRICTION {
    MAX_256_X_256,
    MAX_64_X_64,
    EXACTLY_64_X_64_UNLESS_REQUIRED_OTHERSIZE, // What the spec says! WTF?
    NO_IMAGE_SIZE_RESTRICTION
};

enum FRAME_TEXT_FORMAT {
    FRAME_TEXT_FORMAT_ISO_8859,
    FRAME_TEXT_FORMAT_UTF16,
    FRAME_TEXT_FORMAT_UTF16_BE,
    FRAME_TEXT_FORMAT_UTF8
};

struct ID3FrameType {
    string name;
    ID3_FRAME_TYPE type;
    string description;

    ID3FrameType(string name, ID3_FRAME_TYPE type, string description) {
        this->name = name;
        this->type = type;
        this->description = description;
    }
};

struct ID3Frame {
    string name;
    unsigned int size;
    bool tagAlterPreservation;
    bool fileAlterPreservation;
    bool readOnly;
    bool compressed;
    bool encrypted;
    bool grouped;
    bool unsynchronized;
    bool dataLengthIndicator;
    ID3_FRAME_TYPE type;
    string frameData;
    //FRAME_TEXT_FORMAT frameTextFormat;
    //string rawText;
};

struct ID3 {
    ID3_VERSION version;
    bool unsynchronized;
    bool extendedHeader;
    bool experimental;
    bool footerPresent;
    bool hasCrc;
    unsigned long crc;
    bool tagIsUpdate;
    bool hasRestrictions;
    TAG_SIZE_RESTRICTION tagSizeRestriction;
    TEXT_ENCODING_RESTRICTION textEncodingRestriction;
    TEXT_FIELD_SIZE_RESTRICTION textFieldSizeRestriction;
    IMAGE_ENCODING_RESTRICTION imageEncodingRestriction;
    IMAGE_SIZE_RESTRICTION imageSizeRestriction;
    unsigned int size;
    multimap<string, ID3Frame> frames;
};

static const ID3FrameType INVALID_ID3_FRAME_TYPE { "NONE", ID3_FRAME_TYPE::INVALID, "Invalid frame type" };

static const map<string, ID3FrameType> FRAME_TYPES
{
    { "TIT1", { "TIT1", TEXT, "Content group description" } },
    { "TIT2", { "TIT2", TEXT, "Title/songname/content description" } },
    { "AENC", { "AENC", TEXT, "Audio encryption" } },
    { "APIC", { "APIC", TEXT, "Attached picture" } },
    { "ASPI", { "ASPI", TEXT, "Audio seek point index" } },
    { "COMM", { "COMM", TEXT, "Comments" } },
    { "COMR", { "COMR", TEXT, "Commercial frame" } },
    { "ENCR", { "ENCR", TEXT, "Encryption method registration" } },
    { "EQU2", { "EQU2", TEXT, "Equalisation (2)" } },
    { "ETCO", { "ETCO", TEXT, "Event timing codes" } },
    { "GEOB", { "GEOB", TEXT, "General encapsulated object" } },
    { "GRID", { "GRID", TEXT, "Group identification registration" } },
    { "LINK", { "LINK", TEXT, "Linked information" } },
    { "MCDI", { "MCDI", TEXT, "Music CD identifier" } },
    { "MLLT", { "MLLT", TEXT, "MPEG location lookup table" } },
    { "OWNE", { "OWNE", TEXT, "Ownership frame" } },
    { "PRIV", { "PRIV", TEXT, "Private frame" } },
    { "PCNT", { "PCNT", TEXT, "Play counter" } },
    { "POPM", { "POPM", TEXT, "Popularimeter" } },
    { "POSS", { "POSS", TEXT, "Position synchronisation frame" } },
    { "RBUF", { "RBUF", TEXT, "Recommended buffer size" } },
    { "RVA2", { "RVA2", TEXT, "Relative volume adjustment (2)" } },
    { "RVRB", { "RVRB", TEXT, "Reverb" } },
    { "SEEK", { "SEEK", TEXT, "Seek frame" } },
    { "SIGN", { "SIGN", TEXT, "Signature frame" } },
    { "SYLT", { "SYLT", TEXT, "Synchronised lyric/text" } },
    { "SYTC", { "SYTC", TEXT, "Synchronised tempo codes" } },
    { "TALB", { "TALB", TEXT, "Album/Movie/Show title" } },
    { "TBPM", { "TBPM", TEXT, "BPM (beats per minute)" } },
    { "TCOM", { "TCOM", TEXT, "Composer" } },
    { "TCON", { "TCON", TEXT, "Content type" } },
    { "TCOP", { "TCOP", TEXT, "Copyright message" } },
    { "TDEN", { "TDEN", TEXT, "Encoding time" } },
    { "TDLY", { "TDLY", TEXT, "Playlist delay" } },
    { "TDOR", { "TDOR", TEXT, "Original release time" } },
    { "TDRC", { "TDRC", TEXT, "Recording time" } },
    { "TDRL", { "TDRL", TEXT, "Release time" } },
    { "TDTG", { "TDTG", TEXT, "Tagging time" } },
    { "TENC", { "TENC", TEXT, "Encoded by" } },
    { "TEXT", { "TEXT", TEXT, "Lyricist/Text writer" } },
    { "TFLT", { "TFLT", TEXT, "File type" } },
    { "TIPL", { "TIPL", TEXT, "Involved people list" } },
    { "TIT1", { "TIT1", TEXT, "Content group description" } },
    { "TIT2", { "TIT2", TEXT, "Title/songname/content description" } },
    { "TIT3", { "TIT3", TEXT, "Subtitle/Description refinement" } },
    { "TKEY", { "TKEY", TEXT, "Initial key" } },
    { "TLAN", { "TLAN", TEXT, "Language(s)" } },
    { "TLEN", { "TLEN", TEXT, "Length" } },
    { "TMCL", { "TMCL", TEXT, "Musician credits list" } },
    { "TMED", { "TMED", TEXT, "Media type" } },
    { "TMOO", { "TMOO", TEXT, "Mood" } },
    { "TOAL", { "TOAL", TEXT, "Original album/movie/show title" } },
    { "TOFN", { "TOFN", TEXT, "Original filename" } },
    { "TOLY", { "TOLY", TEXT, "Original lyricist(s)/text writer(s)" } },
    { "TOPE", { "TOPE", TEXT, "Original artist(s)/performer(s)" } },
    { "TOWN", { "TOWN", TEXT, "File owner/licensee" } },
    { "TPE1", { "TPE1", TEXT, "Lead performer(s)/Soloist(s)" } },
    { "TPE2", { "TPE2", TEXT, "Band/orchestra/accompaniment" } },
    { "TPE3", { "TPE3", TEXT, "Conductor/performer refinement" } },
    { "TPE4", { "TPE4", TEXT, "Interpreted, remixed, or otherwise modified by" } },
    { "TPOS", { "TPOS", TEXT, "Part of a set" } },
    { "TPRO", { "TPRO", TEXT, "Produced notice" } },
    { "TPUB", { "TPUB", TEXT, "Publisher" } },
    { "TRCK", { "TRCK", TEXT, "Track number/Position in set" } },
    { "TRSN", { "TRSN", TEXT, "Internet radio station name" } },
    { "TRSO", { "TRSO", TEXT, "Internet radio station owner" } },
    { "TSOA", { "TSOA", TEXT, "Album sort order" } },
    { "TSOP", { "TSOP", TEXT, "Performer sort order" } },
    { "TSOT", { "TSOT", TEXT, "Title sort order" } },
    { "TSRC", { "TSRC", TEXT, "ISRC (international standard recording code)" } },
    { "TSSE", { "TSSE", TEXT, "Software/Hardware and settings used for encoding" } },
    { "TSST", { "TSST", TEXT, "Set subtitle" } },
    { "TXXX", { "TXXX", TEXT, "User defined text information frame" } },
    { "TYER", { "TYER", TEXT, "Year" } },
    { "UFID", { "UFID", TEXT, "Unique file identifier" } },
    { "USER", { "USER", TEXT, "Terms of use" } },
    { "USLT", { "USLT", TEXT, "Unsynchronised lyric/text transcription" } },
    { "WCOM", { "WCOM", TEXT, "Commercial information" } },
    { "WCOP", { "WCOP", TEXT, "Copyright/Legal information" } },
    { "WOAF", { "WOAF", TEXT, "Official audio file webpage" } },
    { "WOAR", { "WOAR", TEXT, "Official artist/performer webpage" } },
    { "WOAS", { "WOAS", TEXT, "Official audio source webpage" } },
    { "WORS", { "WORS", TEXT, "Official Internet radio station homepage" } },
    { "WPAY", { "WPAY", TEXT, "Payment" } },
    { "WPUB", { "WPUB", TEXT, "Publishers official webpage" } },
    { "WXXX", { "WXXX", TEXT, "User defined URL link frame" } }
};

