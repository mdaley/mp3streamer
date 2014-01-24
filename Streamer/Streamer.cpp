// Streamer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <bitset>
#include "Processor.h"
#include "ID3Handler.h"

using namespace std;

bool exists(const string& fileName);

int main(int argc, char* argv[])
{
    fprintf(stdout, "MP3 Stream Reader\n-----------------\n\n");

    if (argc != 2) {
        fprintf(stdout, "First arg must be mp3 file name.");
        return 1;
    }

    string mp3FileName(argv[1]);

    fprintf(stdout, "Input file name = %s\n", mp3FileName.c_str());

    if (!exists(mp3FileName)) {
        fprintf(stdout, "The input file does not exist or is not readable.");
    }

    ifstream s(mp3FileName.c_str(), ifstream::in | ifstream::binary);

    fprintf(stdout, "\n-------MP3-------\n\n");

    try {
        ID3Handler handler;
        Processor processor;
        
        processor.Process(s, &handler);
        s.close();
    } catch (...) {
        if (s) {
            s.close();
        }
    }

	return 0;
}

bool exists(const string& fileName) {
    ifstream file(fileName.c_str(), ifstream::in || ifstream::binary);
    if (file.good()) {
        file.close();
        return true;
    }
    else {
        file.close();
        return false;
    }
}