/*
Copyright (c) 2019, 2020, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/appender/MemBuffer.h>

namespace logbook {
namespace appender {

MemBuffer::MemBuffer(std::size_t maxRows, std::size_t maxColumns)
: Appender(),
  entries(maxRows+1, (maxColumns > 0 ? maxColumns : 0)),
  maxRows(maxRows+1),
  maxColumns(maxColumns)
{
}

MemBuffer::~MemBuffer() {
}

std::vector<std::tuple<Location, std::string>> MemBuffer::getBuffer() const {
	std::vector<std::tuple<Location, std::string>> rv;

	if(maxColumns > 0) {
		for(std::size_t tmpIdxCons = rowConsumer; tmpIdxCons != rowProducer; tmpIdxCons = (tmpIdxCons + 1) % maxRows) {
	    	rv.push_back(std::make_tuple(entries[tmpIdxCons].location, std::string(&entries[tmpIdxCons].lineStaticSize[0])));
		}
	}
	else {
		for(std::size_t tmpIdxCons = rowConsumer; tmpIdxCons != rowProducer; tmpIdxCons = (tmpIdxCons + 1) % maxRows) {
	    	rv.push_back(std::make_tuple(entries[tmpIdxCons].location, entries[tmpIdxCons].lineDynamicSize));
		}
	}

	return rv;
}

void MemBuffer::flush() {
}

void MemBuffer::write(const Location& location, const char* str, std::size_t len) {
	switch(getRecordLevel()) {
	case RecordLevel::OFF:
		return;
	case RecordLevel::ALL:
		break;
	default: /* RecordLevel::SELECTED */
		if(!location.enabled) {
			return;
		}
		break;
	}

	if(entries[rowProducer].location != location) {
		if(columnsProducer > 0) {
			newline();
		}
    	entries[rowProducer].location = location;
	}

	const char* begin = str;

	for(auto iter = str; iter != &str[len]; ++iter) {
		if(*iter == '\n') {
            write(begin, iter-begin);
            newline();
        	entries[rowProducer].location = location;
			begin = iter+1;
		}
	}
}

void MemBuffer::write(const char* ptr, std::size_t size) {
    if(maxColumns == 0) {
    	entries[rowProducer].lineDynamicSize += std::string(ptr, size);
        columnsProducer += size;
    }
    else {
        std::strncat(&entries[rowProducer].lineStaticSize[columnsProducer], ptr, std::min(size, maxColumns - columnsProducer));
        columnsProducer += std::min(size, maxColumns - columnsProducer);
    }
}

void MemBuffer::newline() {
    rowProducer = (rowProducer + 1) % maxRows;
    if(rowConsumer == rowProducer) {
        rowConsumer = (rowConsumer + 1) % maxRows;
    }

    if(maxColumns == 0) {
    	entries[rowProducer].lineDynamicSize.clear();
    }
    else {
    	entries[rowProducer].lineStaticSize[0] = 0;
    }
    columnsProducer = 0;
}

} /* namespace appender */
} /* namespace logbook */
