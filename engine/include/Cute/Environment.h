#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// This class provides access to environment variables
/// and some general system information.
class CUTE_CORE_API Environment
{
public:
	typedef uint8_t NodeId[6]; /// Ethernet address.

	/// Returns the value of the environment variable
	/// with the given name. Throws a NotFoundException
	/// if the variable does not exist.
	static std::string get(const std::string& name);

	/// Returns the value of the environment variable
	/// with the given name. If the environment variable
	/// is undefined, returns defaultValue instead.
	static std::string get(const std::string& name, const std::string& defaultValue);

	/// Returns true iff an environment variable
	/// with the given name is defined.
	static bool has(const std::string& name);

	/// Sets the environment variable with the given name
	/// to the given value.
	static void set(const std::string& name, const std::string& value);

	/// Returns the operating system name.
	static std::string osName();

	/// Returns the operating system name in a
	/// "user-friendly" way.
	///
	/// Currently this is only implemented for
	/// Windows. There it will return names like
	/// "Windows XP" or "Windows 7/Server 2008 SP2".
	/// On other platforms, returns the same as
	/// osName().
	static std::string osDisplayName();

	/// Returns the operating system version.
	static std::string osVersion();

	/// Returns the operating system architecture.
	static std::string osArchitecture();

	/// Returns the node (or host) name.
	static std::string nodeName();

	/// Returns the Ethernet address of the first Ethernet
	/// adapter found on the system.
	///
	/// Throws a SystemException if no Ethernet adapter is available.
	static void nodeId(NodeId& id);
	
	/// Returns the Ethernet address (format "xx:xx:xx:xx:xx:xx")
	/// of the first Ethernet adapter found on the system.
	///
	/// Throws a SystemException if no Ethernet adapter is available.
	static std::string nodeId();

	/// Returns the number of processors installed in the system.
	///
	/// If the number of processors cannot be determined, returns 1.
	static unsigned processorCount();
};

CUTE_NS_END
