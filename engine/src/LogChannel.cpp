//! Logging
#include "Cute/LogChannel.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

LogChannel::LogChannel()
{

}

LogChannel::~LogChannel()
{

}

void LogChannel::open()
{

}

void LogChannel::close()
{

}

void LogChannel::setProperty(const String& name, const String& value)
{
	throw PropertyNotSupportedException(name);
}

String LogChannel::getProperty(const String& name) const
{
	throw PropertyNotSupportedException(name);
}

CUTE_NS_END