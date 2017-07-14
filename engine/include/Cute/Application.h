#pragma once
#include "Cute/Foundation.h"
#include "Cute/OptionSet.h"
#include "Cute/Logger.h"
#include "Cute/Timespan.h"
#include "Cute/Timestamp.h"
#include "Cute/JConfig.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Application
{
public:
	/// Commonly used exit status codes.
	/// Based on the definitions in the 4.3BSD <sysexits.h> header file.
	enum ExitCode
	{
		EXIT_OK             = 0,  /// successful termination
		EXIT_USAGE          = 64, /// command line usage error
		EXIT_DATAERR        = 65, /// data format error
		EXIT_NOINPUT        = 66, /// cannot open input
		EXIT_NOUSER         = 67, /// addressee unknown
		EXIT_NOHOST         = 68, /// host name unknown
		EXIT_UNAVAILABLE    = 69, /// service unavailable
		EXIT_SOFTWARE       = 70, /// internal software error
		EXIT_OSERR          = 71, /// system error (e.g., can't fork)
		EXIT_OSFILE         = 72, /// critical OS file missing
		EXIT_CANTCREAT      = 73, /// can't create (user) output file
		EXIT_IOERR          = 74, /// input/output error
		EXIT_TEMPFAIL       = 75, /// temp failure; user is invited to retry
		EXIT_PROTOCOL       = 76, /// remote error in protocol
		EXIT_NOPERM         = 77, /// permission denied
		EXIT_CONFIG         = 78  /// configuration error
	};

	static Application& instance();

	Application();
	virtual ~Application();

	int run();
	int run(int argc, char* argv[]);

//	Logger& logger() const;
//	const Timestamp& startTime() const;
//	Timespan uptime() const;

protected:
	virtual bool setup();
	virtual void quit();
	virtual void loop();

//	void getPath();

protected:
//	Logger*		m_logger;
//	OptionSet	m_options;
//	JConfig		m_config;
//	Timestamp	m_startTime;
//	String		m_path;
//	String		m_name;
//	String		m_configPath;

	static Application* s_instance;
};

//
// inlines
//

inline Application& Application::instance()
{
	return *s_instance;
}

//inline Logger& Application::logger() const
//{
//	return *m_logger;
//}
//
//inline const Timestamp& Application::startTime() const
//{
//	return m_startTime;
//}
//
//inline Timespan Application::uptime() const
//{
//	Timestamp now;
//	Timespan span = now - m_startTime;
//	return span;
//}

CUTE_NS_END
