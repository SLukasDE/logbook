/*
Copyright (c) 2019-2023 Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/Logger.h>
#include <logbook/Logbook.h>

namespace logbook {
inline namespace v0_4 {

Logger::Logger(const char* aTypeName)
: trace(aTypeName, Level::trace),
  debug(aTypeName, Level::debug),
  info(aTypeName, Level::info),
  warn(aTypeName, Level::warn),
  error(aTypeName, Level::error),
  typeName(aTypeName)
{
}

void Logger::setLevel(Level logLevel) {
	logbook::setLevel(logLevel, typeName);
}

logbook::Logger logger("logbook");

} /* inline namespace v0_4 */
} /* namespace logbook */
