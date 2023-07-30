/*
Copyright (c) 2019-2023 Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_LOGBOOK_H_
#define LOGBOOK_LOGBOOK_H_

#include <logbook/Level.h>
#include <logbook/Writer.h>
#include <ostream>
#include <string>
#include <memory>
#include <thread>

namespace logbook {
inline namespace v0_4 {

// NOT thread save - call it at the beginning if needed. Default is already "true"
// unblocked behavior makes other threads not waiting on logging, while current thread is writing to logger already.
// If logger is used already by current thread, other threads will write to a temporary buffer.
// - Temporary buffer is flushed to real logger, if other thread is done using the logger.
// - If logger is still used by current thread, buffer is queued.
// - If current thread is done using the logger, it flushes queued buffers.
void setUnblocked(bool isUnblocked);

// thread safe, quaranteed by configMutex
void setLevel(Level logLevel, const std::string& typeName);

unsigned int getThreadNo(std::thread::id threadId);

bool isLoggingEnabled(const char* typeName, Level level);


/* ********************************************* *
 * following functions are for internal use only *
 * ********************************************* */

class Location;
std::unique_ptr<Writer> createWriter(const Location& location);

} /* inline namespace v0_4 */
} /* namespace logbook */

#endif /* LOGBOOK_LOGBOOK_H_ */
