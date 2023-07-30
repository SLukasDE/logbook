/*
Copyright (c) 2019-2023 Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/StreamBuffer.h>
#include <logbook/Logbook.h>

namespace logbook {
inline namespace v0_4 {

/* Defined in Logbook.cpp */
void write(const char* ptr, std::size_t size);

StreamBuffer::StreamBuffer(std::size_t aBufferSize)
: std::streambuf(),
  bufferSize(aBufferSize),
  buffer(aBufferSize+1, 0)
{
    /* Calling "setp(buffer, buffer)" set space to zero and this results in calling "int_type overflow(int_type c)" */
    setp(&buffer[0], &buffer[0]);
}

StreamBuffer::int_type StreamBuffer::overflow(int_type c) {
    if (c == traits_type::eof()) {
        return traits_type::eof();
    }

    if(epptr() == &buffer[0]) {
        setp(&buffer[0], &buffer[0] + bufferSize - 1);
    }

    *pptr() = c;
    pbump(1);

    if(sync() == -1) {
        return traits_type::eof();
    }

    return c;
}

int StreamBuffer::sync() {
    char* beginStr = pbase();
    char* endStr = pptr();

    if(beginStr == endStr) {
        return 0;
    }

    char* currentStr = beginStr;
    do {
        char currentChar = *currentStr;
        ++currentStr;

        if (currentChar == '\n') {
            std::ptrdiff_t n = currentStr - beginStr;
            logbook::write(beginStr, n);
            beginStr = currentStr;
        }
    } while(currentStr != endStr);

    if (beginStr != currentStr) {
        std::ptrdiff_t n = currentStr - beginStr;
        logbook::write(beginStr, n);
    }

    setp(&buffer[0], &buffer[0] + bufferSize - 1);
    return 0;
}

} /* inline namespace v0_4 */
} /* namespace logbook */
