#pragma once
#include "Cute/Foundation.h"
#include "Cute/LogMessage.h"
#include "Cute/Ref.h"

CUTE_NS_BEGIN

class CUTE_CORE_API LogChannel : public Ref<LogChannel>
{
public:
	virtual ~LogChannel();

	virtual void open();
	virtual void close();
	virtual void log(const LogMessage& msg) = 0;
	virtual void setProperty(const String& name, const String& value);
	virtual String getProperty(const String& name) const;

protected:
	LogChannel();
};

CUTE_NS_END
