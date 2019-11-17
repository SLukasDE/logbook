/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_BUFFER_H_
#define LOGBOOK_BUFFER_H_

#include <streambuf>
#include <vector>
#include <functional>

namespace logbook {

class Buffer : public std::streambuf {
public:
	Buffer(std::size_t bufferSize, std::function<void(const char*, std::size_t)> writeHandler);

private:
    int_type overflow(int_type ch) override;
    int sync() override;

    std::size_t bufferSize;
    std::vector<char> buffer;
    std::function<void(const char*, std::size_t)> writeHandler;
};

} /* namespace logbook */

#endif /* LOGBOOK_BUFFER_H_ */
