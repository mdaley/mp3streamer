mp3streamer
===========

A C++ library allowing mp3 streams to be decoded.

NOT READY FOR USE - WORK IN PROGRESS.

So, far this library can read ID3 v2.3 and v2.4 data from an mp3 stream. 

There are unit tests showing reading from a variety of test mp3 files. Of
course there need to be more tests to cover the range of ID3 data created
by various different encoders. Things remaining to be done in this area are:

- write interpreter classes that can read the raw ID3 frame data.
- deal with ID3 v1 and perhaps with ID3 v2.2.
- obtain test mp3 files with unsynchronized ID3 data generated from real encoders.
- write more tests.
- write the code that inteprets mp3 frames and transforms them into raw PCM.

One of my aims is to be able to read mp3 streams and transform them into PCM
data on the fly so that audio processing effects can be added. I want to be able
to do this rather than to have to download a complete mp3 file before processing
it as most other mp3 libraries that I've found seem to do.

Also, this is a C++ learning exercise for me; if I have commited faux pas, please
tell me how to improve the code...