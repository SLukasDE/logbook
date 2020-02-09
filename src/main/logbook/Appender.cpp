/*
Copyright (c) 2019, 2020, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/Appender.h>
#include <logbook/Logbook.h>

namespace logbook {

/* Defined in Logbook.cpp
 * Functions is thread safe, quaranteed by loggerMutex */
void addAppender(Appender& appender);

/* Defined in Logbook.cpp
 * Functions is thread safe, quaranteed by loggerMutex */
void removeAppender(Appender& appender);

Appender::Appender() {
	addAppender(*this);
}

Appender::~Appender() {
	removeAppender(*this);
}

void Appender::setLayout(const Layout* aLayout) {
	layout = aLayout;
}

const Layout* Appender::getLayout() const {
	return layout;
}

Appender::RecordLevel Appender::getRecordLevel() const {
	return recordLevel;
}

void Appender::setRecordLevel(RecordLevel aRecordLevel) {
	recordLevel = aRecordLevel;
}

} /* namespace logbook */
