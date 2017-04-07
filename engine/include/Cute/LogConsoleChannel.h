#pragma once
#include "Cute/LogChannel.h"

CUTE_NS_BEGIN

/// A channel that writes to an ostream.
///
/// Only the message's text is written, followed
/// by a newline.
///
/// Messages can be colored depending on priority.
/// The console device must support ANSI escape codes
/// in order to display colored messages.
///
/// To enable message coloring, set the "enableColors"
/// property to true (default). Furthermore, colors can be
/// configured by setting the following properties
/// (default values are given in parenthesis):
/// 
///   * traceColor (gray)
///   * debugColor (gray)
///   * informationColor (default)
///   * noticeColor (default)
///   * warningColor (yellow)
///   * errorColor (lightRed)
///   * criticalColor (lightRed)
///   * fatalColor (lightRed)
///
/// The following color values are supported:
/// 
///   * default
///   * black
///   * red
///   * green
///   * brown
///   * blue
///   * magenta
///   * cyan
///   * gray
///   * darkgray
///   * lightRed
///   * lightGreen
///   * yellow
///   * lightBlue
///   * lightMagenta
///   * lightCyan
///   * white
///
/// Chain this channel to a FormattingChannel with an
/// appropriate Formatter to control what is contained 
/// in the text.
///
/// Similar to StreamChannel, except that a static
/// mutex is used to protect against multiple
/// console channels concurrently writing to the
/// same stream.
class CUTE_CORE_API LogConsoleChannel : public LogChannel
{
public:
	LogConsoleChannel();
	LogConsoleChannel(std::ostream& str);
	~LogConsoleChannel();

	void log(const LogMessage& msg);
	void setProperty(const String& name, const String& value);
	String getProperty(const String& name) const;

protected:
	void initColors();

private:
	std::ostream& m_str;
	bool m_enableColors;
	int  m_colors[LOG_LEVEL_MAX];
};

CUTE_NS_END