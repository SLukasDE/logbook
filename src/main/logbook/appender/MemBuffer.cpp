/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/appender/MemBuffer.h>

namespace logbook {
namespace appender {

MemBuffer::MemBuffer(std::size_t maxRows, std::size_t maxColumns)
: Appender(),
  locations(maxRows+1),
  maxRows(maxRows+1),
  maxColumns(maxColumns)
{
	if(maxColumns == 0) {
	    std::vector<std::string> tmpLines(maxRows+1);
		lines = std::move(tmpLines);
	}
	else {
		std::vector<LineBuffer> tmpRows(maxRows+1, LineBuffer(maxColumns+1, 0));
		rows = std::move(tmpRows);
	}
}

MemBuffer::~MemBuffer() {
}

std::vector<std::tuple<Location, std::string>> MemBuffer::getBuffer() const {
	std::vector<std::tuple<Location, std::string>> rv;

	if(maxColumns == 0) {
		for(std::size_t tmpIdxCons = rowConsumer; tmpIdxCons != rowProducer; tmpIdxCons = (tmpIdxCons + 1) % maxRows) {
	    	rv.push_back(std::make_tuple(locations[tmpIdxCons], lines[tmpIdxCons]));
	    }
	}
	else {
		for(std::size_t tmpIdxCons = rowConsumer; tmpIdxCons != rowProducer; tmpIdxCons = (tmpIdxCons + 1) % maxRows) {
	    	rv.push_back(std::make_tuple(locations[tmpIdxCons], std::string(&rows[tmpIdxCons][0])));
	    }
	}

	return rv;
}

void MemBuffer::flushNewLine(const Location& location, bool enabled) {
	switch(getRecordLevel()) {
	case RecordLevel::OFF:
		return;
	case RecordLevel::ALL:
		break;
	default: /* RecordLevel::SELECTED */
		if(!enabled) {
			return;
		}
		break;
	}

	if(columnsProducer > 0) {
        rowProducer = (rowProducer + 1) % maxRows;
        if(maxColumns == 0) {
            lines[rowProducer].clear();
        }
        else {
            rows[rowProducer][0] = 0;
        }
        columnsProducer = 0;
        if(rowConsumer == rowProducer) {
            rowConsumer = (rowConsumer + 1) % maxRows;
        }
	}
}

void MemBuffer::write(const Location& location, bool enabled, const char* str, std::size_t len) {
	switch(getRecordLevel()) {
	case RecordLevel::OFF:
		return;
	case RecordLevel::ALL:
		break;
	default: /* RecordLevel::SELECTED */
		if(!enabled) {
			return;
		}
		break;
	}

    while(len > 0) {
        bool lineWritten = false;

        locations[rowProducer] = location;

        for(std::size_t i = 0; i < len; ++i) {
            if(str[i] != '\n') {
                continue;
            }

            if(maxColumns == 0) {
            	lines[rowProducer] += std::string(str, i);
            }
            else {
                std::strncat(&rows[rowProducer][columnsProducer], str, std::min(i, maxColumns - columnsProducer));
            }

            lineWritten = true;
            rowProducer = (rowProducer + 1) % maxRows;
            columnsProducer = 0;

            if(maxColumns == 0) {
            	lines[rowProducer].clear();
            }
            else {
                rows[rowProducer][0] = 0;
            }

            if(rowConsumer == rowProducer) {
                rowConsumer = (rowConsumer + 1) % maxRows;
            }

            ++i;
            str += i;
            len -= i;
            break;
        }

        if(!lineWritten) {
            if(maxColumns == 0) {
            	lines[rowProducer] += std::string(str, len);
                columnsProducer += len;
            }
            else {
                std::strncat(&rows[rowProducer][columnsProducer], str, std::min(len, maxColumns - columnsProducer));
                columnsProducer += std::min(len, maxColumns - columnsProducer);
            }

            len = 0;
        }
    }
}

} /* namespace appender */
} /* namespace logbook */
