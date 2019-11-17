/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_LAYOUT_H_
#define LOGBOOK_LAYOUT_H_

#include <logbook/Id.h>
#include <string>

namespace logbook {

class Layout {
public:
	Layout() = default;
	~Layout() = default;

	bool getShowTimestamp() const;
	void setShowTimestamp(bool showTimestamp = true);

	bool getShowLevel() const;
	void setShowLevel(bool showLevel = true);

	bool getShowTypeName() const;
	void setShowTypeName(bool showTypeName = true);

	bool getShowAddress() const;
	void setShowAddress(bool showAddress = true);

	bool getShowFile() const;
	void setShowFile(bool showFile = true);

	bool getShowFunction() const;
	void setShowFunction(bool showFunction = true);

	bool getShowLineNo() const;
	void setShowLineNo(bool showLineNo = true);

	bool getShowThreadNo() const;
	void setShowThreadNo(bool showThreadNo = true);

	std::string makePrefix(const Id& id);

private:
	bool showTimestamp = true;
	bool showLevel = true;
	bool showTypeName = true;
	bool showThreadNo = true;
	bool showAddress = true;
	bool showFunction = false;
	bool showFile = false;
	bool showLineNo = false;
};

} /* namespace logbook */

#endif /* LOGBOOK_LAYOUT_H_ */
