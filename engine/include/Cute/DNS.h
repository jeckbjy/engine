#pragma once
#include "Cute/SocketDefs.h"
#include "Cute/SocketAddress.h"
#include "Cute/HostEntry.h"
#include "Cute/IPAddress.h"

CUTE_NS_BEGIN

class CUTE_CORE_API DNS
{
public:
	enum HintFlag
	{
		DNS_HINT_NONE			= 0,
		DNS_HINT_AI_PASSIVE		= AI_PASSIVE,		/// Socket address will be used in bind() call
		DNS_HINT_AI_CANONNAME	= AI_CANONNAME,		/// Return canonical name in first ai_canonname
		DNS_HINT_AI_NUMERICHOST = AI_NUMERICHOST,	/// Nodename must be a numeric address string
		DNS_HINT_AI_NUMERICSERV = AI_NUMERICSERV,	/// Servicename must be a numeric port number
		DNS_HINT_AI_ALL			= AI_ALL,			/// Query both IP6 and IP4 with AI_V4MAPPED
		DNS_HINT_AI_ADDRCONFIG	= AI_ADDRCONFIG,	/// Resolution only if global address configured
		DNS_HINT_AI_V4MAPPED	= AI_V4MAPPED,		/// On v6 failure, query v4 and convert to V4MAPPED format	
		DNS_HINT_DEFAULTS		= DNS_HINT_AI_CANONNAME | DNS_HINT_AI_ADDRCONFIG,
	};

	static HostEntry hostByName(const String& hostname, unsigned hintFlags = DNS_HINT_DEFAULTS);
	static HostEntry hostByAddress(const IPAddress& address, unsigned hintFlags = DNS_HINT_DEFAULTS);
};

CUTE_NS_END
