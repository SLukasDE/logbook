/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/Buffer.h>

namespace logbook {

Buffer::Buffer(std::size_t bufferSize, std::function<void(const char*, std::size_t)> writeHandler)
: std::streambuf(),
  bufferSize(bufferSize),
  buffer(bufferSize+1, 0),
  writeHandler(writeHandler)
{
    /* Calling "setp(buffer, buffer)" set space to zero and this results in calling "int_type overflow(int_type c)" */
    setp(&buffer[0], &buffer[0]);
}

Buffer::int_type Buffer::overflow(int_type c) {
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

int Buffer::sync() {
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
            writeHandler(beginStr, n);
            beginStr = currentStr;
        }
    } while(currentStr != endStr);

    if (beginStr != currentStr) {
        std::ptrdiff_t n = currentStr - beginStr;
        writeHandler(beginStr, n);
    }

    setp(&buffer[0], &buffer[0] + bufferSize - 1);
    return 0;
}

} /* namespace logbook */
