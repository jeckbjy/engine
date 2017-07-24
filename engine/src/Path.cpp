// module FileSystem
#include "Cute/Path.h"
#include "Cute/Files.h"
#include "Cute/Exception.h"
#include "Cute/String.h"
#include "Cute/Environment.h"
#include <algorithm>

#if defined(CUTE_OS_FAMILY_POSIX)
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
//#if !defined(POCO_VXWORKS)
#include <pwd.h>
//#endif
#endif

CUTE_NS_BEGIN

#ifdef CUTE_OS_FAMILY_WINDOWS
String getSystemPath()
{
	char buffer[MAX_PATH];
	DWORD n = GetSystemDirectoryA(buffer, sizeof(buffer));
	if (n > 0 && n < sizeof(buffer))
	{
		String result(buffer, n);
		if (result[n - 1] != '\\')
			result.append("\\");
		return result;
	}
	else throw SystemException("Cannot get system directory");
}
#endif

Path::Path()
	: m_absolute(false)
{
}

Path::Path(bool absolutePath)
	: m_absolute(absolutePath)
{
}

Path::Path(const String& path)
{
	assign(path);
}

Path::Path(const String& path, Style style)
{
	assign(path, style);
}

Path::Path(const Path& path)
	: m_node(path.m_node)
	, m_device(path.m_device)
	, m_name(path.m_name)
	, m_version(path.m_version)
	, m_dirs(path.m_dirs)
	, m_absolute(path.m_absolute)
{
}

Path::Path(const Path& rParent, const String& fileName) 
	: m_node(rParent.m_node)
	, m_device(rParent.m_device)
	, m_name(rParent.m_name)
	, m_version(rParent.m_version)
	, m_dirs(rParent.m_dirs)
	, m_absolute(rParent.m_absolute)
{
	makeDirectory();
	m_name = fileName;
}

Path::Path(const Path& parent, const Path& relative)
	: m_node(parent.m_node)
	, m_device(parent.m_device)
	, m_name(parent.m_name)
	, m_version(parent.m_version)
	, m_dirs(parent.m_dirs)
	, m_absolute(parent.m_absolute)
{
	resolve(relative);
}

Path::~Path()
{
}

Path& Path::operator = (const Path& path)
{
	return assign(path);
}

Path& Path::operator = (const String& path)
{
	return assign(path);
}

void Path::swap(Path& path)
{
	std::swap(m_node, path.m_node);
	std::swap(m_device, path.m_device);
	std::swap(m_name, path.m_name);
	std::swap(m_version, path.m_version);
	std::swap(m_dirs, path.m_dirs);
	std::swap(m_absolute, path.m_absolute);
}

Path& Path::assign(const Path& path)
{
	if (&path != this)
	{
		m_node = path.m_node;
		m_device = path.m_device;
		m_name = path.m_name;
		m_version = path.m_version;
		m_dirs = path.m_dirs;
		m_absolute = path.m_absolute;
	}
	return *this;
}

Path& Path::assign(const String& path)
{
#if defined(CUTE_OS_FAMILY_VMS)
	parseVMS(path);
#elif defined(CUTE_OS_FAMILY_WINDOWS)
	parseWindows(path);
#else
	parseUnix(path);
#endif
	return *this;
}

Path& Path::assign(const String& path, Style style)
{
	switch (style)
	{
	case PATH_UNIX:
		parseUnix(path);
		break;
	case PATH_WINDOWS:
		parseWindows(path);
		break;
	case PATH_VMS:
		parseVMS(path);
		break;
	case PATH_NATIVE:
		assign(path);
		break;
	case PATH_GUESS:
		parseGuess(path);
		break;
	default:
		cute_bugcheck();
	}
	return *this;
}

Path& Path::assign(const char* path)
{
	return assign(String(path));
}

String Path::toString() const
{
#if defined(CUTE_OS_FAMILY_UNIX)
	return buildUnix();
#elif defined(CUTE_OS_FAMILY_WINDOWS)
	return buildWindows();
#else
	return buildVMS();
#endif
}

String Path::toString(Style style) const
{
	switch (style)
	{
	case PATH_UNIX:
		return buildUnix();
	case PATH_WINDOWS:
		return buildWindows();
	case PATH_VMS:
		return buildVMS();
	case PATH_NATIVE:
	case PATH_GUESS:
		return toString();
	default:
		cute_bugcheck();
	}
	return String();
}

bool Path::tryParse(const String& path)
{
	try
	{
		Path p;
		p.parse(path);
		assign(p);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Path::tryParse(const String& path, Style style)
{
	try
	{
		Path p;
		p.parse(path, style);
		assign(p);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

Path& Path::parseDirectory(const String& path)
{
	assign(path);
	return makeDirectory();
}

Path& Path::parseDirectory(const String& path, Style style)
{
	assign(path, style);
	return makeDirectory();
}


Path& Path::makeDirectory()
{
#if defined(CUTE_OS_FAMILY_VMS)
	pushDirectory(getBaseName());
#else
	pushDirectory(m_name);
#endif
	m_name.clear();
	m_version.clear();
	return *this;
}

Path& Path::makeFile()
{
	if (!m_dirs.empty() && m_name.empty())
	{
		m_name = m_dirs.back();
		m_dirs.pop_back();
#if defined(CUTE_OS_FAMILY_VMS)
		setExtension("DIR");
#endif
	}
	return *this;
}

Path& Path::makeAbsolute()
{
	return makeAbsolute(current());
}

Path& Path::makeAbsolute(const Path& base)
{
	if (!m_absolute)
	{
		Path tmp = base;
		tmp.makeDirectory();
		for (StringVec::const_iterator it = m_dirs.begin(); it != m_dirs.end(); ++it)
		{
			tmp.pushDirectory(*it);
		}
		m_node = tmp.m_node;
		m_device = tmp.m_device;
		m_dirs = tmp.m_dirs;
		m_absolute = base.m_absolute;
	}
	return *this;
}

Path Path::absolute() const
{
	Path result(*this);
	if (!result.m_absolute)
	{
		result.makeAbsolute();
	}
	return result;
}

Path Path::absolute(const Path& base) const
{
	Path result(*this);
	if (!result.m_absolute)
	{
		result.makeAbsolute(base);
	}
	return result;
}

Path Path::parent() const
{
	Path p(*this);
	return p.makeParent();
}

Path& Path::makeParent()
{
	if (m_name.empty())
	{
		if (m_dirs.empty())
		{
			if (!m_absolute)
				m_dirs.push_back("..");
		}
		else
		{
			if (m_dirs.back() == "..")
				m_dirs.push_back("..");
			else
				m_dirs.pop_back();
		}
	}
	else
	{
		m_name.clear();
		m_version.clear();
	}
	return *this;
}

Path& Path::append(const Path& path)
{
	makeDirectory();
	m_dirs.insert(m_dirs.end(), path.m_dirs.begin(), path.m_dirs.end());
	m_name = path.m_name;
	m_version = path.m_version;
	return *this;
}

Path& Path::resolve(const Path& path)
{
	if (path.isAbsolute())
	{
		assign(path);
	}
	else
	{
		for (int i = 0; i < path.depth(); ++i)
			pushDirectory(path[i]);
		m_name = path.m_name;
	}
	return *this;
}

Path& Path::setNode(const String& node)
{
	m_node = node;
	m_absolute = m_absolute || !node.empty();
	return *this;
}

Path& Path::setDevice(const String& device)
{
	m_device = device;
	m_absolute = m_absolute || !device.empty();
	return *this;
}

const String& Path::directory(int n) const
{
	cute_assert(0 <= n && n <= m_dirs.size());

	if (n < m_dirs.size())
		return m_dirs[n];
	else
		return m_name;
}

const String& Path::operator [] (int n) const
{
	cute_assert(0 <= n && n <= m_dirs.size());

	if (n < m_dirs.size())
		return m_dirs[n];
	else
		return m_name;
}

Path& Path::pushDirectory(const String& dir)
{
	if (!dir.empty() && dir != ".")
	{
#if defined(CUTE_OS_FAMILY_VMS)
		if (dir == ".." || dir == "-")
		{
			if (!m_dirs.empty() && m_dirs.back() != ".." && m_dirs.back() != "-")
				m_dirs.pop_back();
			else if (!m_absolute)
				m_dirs.push_back(dir);
		}
		else m_dirs.push_back(dir);
#else
		if (dir == "..")
		{
			if (!m_dirs.empty() && m_dirs.back() != "..")
				m_dirs.pop_back();
			else if (!m_absolute)
				m_dirs.push_back(dir);
		}
		else m_dirs.push_back(dir);
#endif
	}
	return *this;
}

Path& Path::popDirectory()
{
	cute_assert(!m_dirs.empty());

	m_dirs.pop_back();
	return *this;
}

Path& Path::popFrontDirectory()
{
	cute_assert(!m_dirs.empty());

	StringVec::iterator it = m_dirs.begin();
	m_dirs.erase(it);
	return *this;
}

Path& Path::setFileName(const String& name)
{
	m_name = name;
	return *this;
}

Path& Path::setBaseName(const String& name)
{
	String ext = getExtension();
	m_name = name;
	if (!ext.empty())
	{
		m_name.append(".");
		m_name.append(ext);
	}
	return *this;
}

String Path::getBaseName() const
{
	String::size_type pos = m_name.rfind('.');
	if (pos != String::npos)
		return m_name.substr(0, pos);
	else
		return m_name;
}

Path& Path::setExtension(const String& extension)
{
	m_name = getBaseName();
	if (!extension.empty())
	{
		m_name.append(".");
		m_name.append(extension);
	}
	return *this;
}

String Path::getExtension() const
{
	String::size_type pos = m_name.rfind('.');
	if (pos != String::npos)
		return m_name.substr(pos + 1);
	else
		return String();
}

Path& Path::clear()
{
	m_node.clear();
	m_device.clear();
	m_name.clear();
	m_dirs.clear();
	m_version.clear();
	m_absolute = false;
	return *this;
}

String Path::current()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	char buffer[MAX_PATH];
	DWORD n = GetCurrentDirectoryA(sizeof(buffer), buffer);
	if (n > 0 && n < sizeof(buffer))
	{
		String result(buffer, n);
		if (result[n - 1] != '\\')
			result.append("\\");
		return result;
	}
	else throw SystemException("Cannot get current directory");
	// convert
#else
	String path;
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)))
		path = cwd;
	else
		throw SystemException("cannot get current directory");
	String::size_type n = path.size();
	if (n > 0 && path[n - 1] != '/') path.append("/");
	return path;
#endif
}

String Path::home()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	String result;
	if (Environment::has("USERPROFILE"))
	{
		result = Environment::get("USERPROFILE");
	}
	else if (Environment::has("HOMEDRIVE") && Environment::has("HOMEPATH"))
	{
		result = Environment::get("HOMEDRIVE");
		result.append(Environment::get("HOMEPATH"));
	}
	else
	{
		result = getSystemPath();
	}

	String::size_type n = result.size();
	if (n > 0 && result[n - 1] != '\\')
		result.append("\\");
	return result;
#elif defined(CUTE_VXWORKS)
	if (Environment::has("HOME"))
		return Environment::get("HOME");
	else
		return "/";
#else
	String path;
	struct passwd* pwd = getpwuid(getuid());
	if (pwd)
		path = pwd->pw_dir;
	else
	{
		pwd = getpwuid(geteuid());
		if (pwd)
			path = pwd->pw_dir;
		else
			if (Environment::has("HOME"))
				path = Environment::get("HOME");
	}
	String::size_type n = path.size();
	if (n > 0 && path[n - 1] != '/') path.append("/");
	return path;
#endif
}

String Path::configHome()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	String result;

	// if APPDATA environment variable not exist, return home directory instead
	try
	{
		result = Environment::get("APPDATA");
	}
	catch (NotFoundException&)
	{
		result = home();
	}

	String::size_type n = result.size();
	if (n > 0 && result[n - 1] != '\\')
		result.append("\\");
	return result;

#elif defined(CUTE_OS_VXWORKS)
	return home();

#else

	String path = home();
	String::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
#if defined(CUTE_OS_MAC)
		path.append("Library/Preferences/");
#else
		path.append(".config/");
#endif

	return path;
#endif
}

String Path::dataHome()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	String result;

	// if LOCALAPPDATA environment variable not exist, return config home instead
	try
	{
		result = Environment::get("LOCALAPPDATA");
	}
	catch (NotFoundException&)
	{
		result = configHome();
	}

	String::size_type n = result.size();
	if (n > 0 && result[n - 1] != '\\')
		result.append("\\");
	return result;

#elif defined(CUTE_OS_VXWORKS)
	return home();
#else
	String path = home();
	String::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
#if defined(CUTE_OS_MAC)
		path.append("Library/Application Support/");
#else
		path.append(".local/share/");
#endif

	return path;
#endif
}

String Path::tempHome()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	return temp();

#elif defined(CUTE_OS_VXWORKS)
	return temp();

#else
	String path = home();
	String::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
#if defined(CUTE_OS_MAC)
		path.append("Library/Caches/");
#else
		path.append(".local/tmp/");
#endif

	return path;

#endif
}

String Path::cacheHome()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	return temp();

#elif defined(CUTE_VXWORKS)
	return temp();

#else
	String path = home();
	String::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
#if  defined(CUTE_OS_MAC)
		path.append("Library/Caches/");
#else
		path.append(".cache/");
#endif

	return path;
#endif
}

String Path::temp()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	char buffer[MAX_PATH];
	DWORD n = GetTempPathA(sizeof(buffer), buffer);
	if (n > 0 && n < sizeof(buffer))
	{
		n = GetLongPathNameA(buffer, buffer, static_cast<DWORD>(sizeof buffer));
		if (n <= 0) throw SystemException("Cannot get temporary directory long path name");
		String result(buffer, n);
		if (result[n - 1] != '\\')
			result.append("\\");
		return result;
	}
	else throw SystemException("Cannot get temporary directory");

#else

	String path;
	char* tmp = getenv("TMPDIR");
	if (tmp)
	{
		path = tmp;
		String::size_type n = path.size();
		if (n > 0 && path[n - 1] != '/') path.append("/");
	}
	else
	{
		path = "/tmp/";
	}

	return path;

#endif
}

String Path::config()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	String result;

	// if PROGRAMDATA environment variable not exist, return system directory instead
	try
	{
		result = Environment::get("PROGRAMDATA");
	}
	catch (NotFoundException&)
	{
		result = getSystemPath();
	}

	String::size_type n = result.size();
	if (n > 0 && result[n - 1] != '\\')
		result.append("\\");
	return result;

#else

	String path;

#if defined(CUTE_OS_MAC)
	path = "/Library/Preferences/";
#else
	path = "/etc/";
#endif
	return path;

#endif
}

String Path::null()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	return "NUL:";
#elif defined(CUTE_VXWORKS)
	return "/null";
#else
	return "/dev/null";
#endif
}

String Path::expand(const String& path)
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	char buffer[MAX_PATH];
	DWORD n = ExpandEnvironmentStringsA(path.c_str(), buffer, sizeof(buffer));
	if (n > 0 && n < sizeof(buffer))
		return String(buffer, n - 1);
	else
		return path;

#else
	String result;
	String::const_iterator it = path.begin();
	String::const_iterator end = path.end();
	if (it != end && *it == '~')
	{
		++it;
		if (it != end && *it == '/')
		{
			const char* homeEnv = getenv("HOME");
			if (homeEnv)
			{
				result += homeEnv;
				String::size_type resultSize = result.size();
				if (resultSize > 0 && result[resultSize - 1] != '/')
					result.append("/");
			}
			else
			{
				result += home();
			}
			++it;
		}
		else result += '~';
	}
	while (it != end)
	{
		if (*it == '$')
		{
			String var;
			++it;
			if (it != end && *it == '{')
			{
				++it;
				while (it != end && *it != '}') var += *it++;
				if (it != end) ++it;
			}
			else
			{
				while (it != end && (Ascii::isAlphaNumeric(*it) || *it == '_')) var += *it++;
			}
			char* val = getenv(var.c_str());
			if (val) result += val;
		}
		else result += *it++;
	}
	return result;
#endif
}

void Path::listRoots(std::vector<String>& roots)
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	roots.clear();
	char buffer[128];
	DWORD n = GetLogicalDriveStringsA(sizeof(buffer) - 1, buffer);
	char* it = buffer;
	char* end = buffer + (n > sizeof(buffer) ? sizeof(buffer) : n);
	while (it < end)
	{
		String dev;
		while (it < end && *it) dev += *it++;
		roots.push_back(dev);
		++it;
	}
#else
	roots.clear();
	roots.push_back("/");
#endif
}

bool Path::find(StringVec::const_iterator it, StringVec::const_iterator end, const String& name, Path& path)
{
	while (it != end)
	{
#if defined(WIN32)
		String cleanPath(*it);
		if (cleanPath.size() > 1 && cleanPath[0] == '"' && cleanPath[cleanPath.size() - 1] == '"')
		{
			cleanPath = cleanPath.substr(1, cleanPath.size() - 2);
		}
		Path p(cleanPath);
#else
		Path p(*it);
#endif
		p.makeDirectory();
		p.resolve(Path(name));
		if (Files::exists(p.toString()))
		{
			path = p;
			return true;
		}
		++it;
	}
	return false;
}

bool Path::find(const String& pathList, const String& name, Path& path)
{
    StringArray st = Strings::split(pathList, pathSeparator(), String::TOKEN_ALL);
    return find(st.begin(), st.end(), name, path);
}

void Path::parseUnix(const String& path)
{
	clear();

	String::const_iterator it = path.begin();
	String::const_iterator end = path.end();

	if (it != end)
	{
		if (*it == '/')
		{
			m_absolute = true; ++it;
		}
		else if (*it == '~')
		{
			++it;
			if (it == end || *it == '/')
			{
				Path cwd(home());
				m_dirs = cwd.m_dirs;
				m_absolute = true;
			}
			else --it;
		}

		while (it != end)
		{
			String name;
			while (it != end && *it != '/') name += *it++;
			if (it != end)
			{
				if (m_dirs.empty())
				{
					if (!name.empty() && *(name.rbegin()) == ':')
					{
						m_absolute = true;
						m_device.assign(name, 0, name.length() - 1);
					}
					else
					{
						pushDirectory(name);
					}
				}
				else pushDirectory(name);
			}
			else m_name = name;
			if (it != end) ++it;
		}
	}
}

void Path::parseWindows(const String& path)
{
	clear();

	String::const_iterator it = path.begin();
	String::const_iterator end = path.end();

	if (it != end)
	{
		if (*it == '\\' || *it == '/') { m_absolute = true; ++it; }
		if (m_absolute && it != end && (*it == '\\' || *it == '/')) // UNC
		{
			++it;
			while (it != end && *it != '\\' && *it != '/') m_node += *it++;
			if (it != end) ++it;
		}
		else if (it != end)
		{
			char d = *it++;
			if (it != end && *it == ':') // drive letter
			{
				if (m_absolute || !((d >= 'a' && d <= 'z') || (d >= 'A' && d <= 'Z'))) throw PathSyntaxException(path);
				m_absolute = true;
				m_device += d;
				++it;
				if (it == end || (*it != '\\' && *it != '/')) throw PathSyntaxException(path);
				++it;
			}
			else --it;
		}
		while (it != end)
		{
			String name;
			while (it != end && *it != '\\' && *it != '/') name += *it++;
			if (it != end)
				pushDirectory(name);
			else
				m_name = name;
			if (it != end) ++it;
		}
	}
	if (!m_node.empty() && m_dirs.empty() && !m_name.empty())
		makeDirectory();
}

void Path::parseVMS(const String& path)
{
	clear();

	String::const_iterator it = path.begin();
	String::const_iterator end = path.end();

	if (it != end)
	{
		String name;
		while (it != end && *it != ':' && *it != '[' && *it != ';') name += *it++;
		if (it != end)
		{
			if (*it == ':')
			{
				++it;
				if (it != end && *it == ':')
				{
					m_node = name;
					++it;
				}
				else m_device = name;
				m_absolute = true;
				name.clear();
			}
			if (it != end)
			{
				if (m_device.empty() && *it != '[')
				{
					while (it != end && *it != ':' && *it != ';') name += *it++;
					if (it != end)
					{
						if (*it == ':')
						{
							m_device = name;
							m_absolute = true;
							name.clear();
							++it;
						}
					}
				}
			}
			if (name.empty())
			{
				if (it != end && *it == '[')
				{
					++it;
					if (it != end)
					{
						m_absolute = true;
						if (*it == '.')
						{
							m_absolute = false; ++it;
						}
						else if (*it == ']' || *it == '-')
							m_absolute = false;
						while (it != end && *it != ']')
						{
							name.clear();
							if (*it == '-')
								name = "-";
							else
								while (it != end && *it != '.' && *it != ']') name += *it++;
							if (!name.empty())
							{
								if (name == "-")
								{
									if (m_dirs.empty() || m_dirs.back() == "..")
										m_dirs.push_back("..");
									else
										m_dirs.pop_back();
								}
								else m_dirs.push_back(name);
							}
							if (it != end && *it != ']') ++it;
						}
						if (it == end) throw PathSyntaxException(path);
						++it;
						if (it != end && *it == '[')
						{
							if (!m_absolute) throw PathSyntaxException(path);
							++it;
							if (it != end && *it == '.') throw PathSyntaxException(path);
							int d = int(m_dirs.size());
							while (it != end && *it != ']')
							{
								name.clear();
								if (*it == '-')
									name = "-";
								else
									while (it != end && *it != '.' && *it != ']') name += *it++;
								if (!name.empty())
								{
									if (name == "-")
									{
										if (m_dirs.size() > d)
											m_dirs.pop_back();
									}
									else m_dirs.push_back(name);
								}
								if (it != end && *it != ']') ++it;
							}
							if (it == end) throw PathSyntaxException(path);
							++it;
						}
					}
					m_name.clear();
				}
				while (it != end && *it != ';') m_name += *it++;
			}
			else m_name = name;
			if (it != end && *it == ';')
			{
				++it;
				while (it != end) m_version += *it++;
			}
		}
		else m_name = name;
	}
}

void Path::parseGuess(const String& path)
{
	bool hasBackslash = false;
	bool hasSlash = false;
	bool hasOpenBracket = false;
	bool hasClosBracket = false;
	bool isWindows = path.length() > 2 && path[1] == ':' && (path[2] == '/' || path[2] == '\\');
	String::const_iterator end = path.end();
	String::const_iterator semiIt = end;
	if (!isWindows)
	{
		for (String::const_iterator it = path.begin(); it != end; ++it)
		{
			switch (*it)
			{
			case '\\': hasBackslash = true; break;
			case '/':  hasSlash = true; break;
			case '[':  hasOpenBracket = true;
			case ']':  hasClosBracket = hasOpenBracket;
			case ';':  semiIt = it; break;
			}
		}
	}
	if (hasBackslash || isWindows)
	{
		parseWindows(path);
	}
	else if (hasSlash)
	{
		parseUnix(path);
	}
	else
	{
		bool isVMS = hasClosBracket;
		if (!isVMS && semiIt != end)
		{
			isVMS = true;
			++semiIt;
			while (semiIt != end)
			{
				if (*semiIt < '0' || *semiIt > '9')
				{
					isVMS = false; break;
				}
				++semiIt;
			}
		}
		if (isVMS)
			parseVMS(path);
		else
			parseUnix(path);
	}
}

String Path::buildUnix() const
{
	String result;
	if (!m_device.empty())
	{
		result.append("/");
		result.append(m_device);
		result.append(":/");
	}
	else if (m_absolute)
	{
		result.append("/");
	}
	for (StringVec::const_iterator it = m_dirs.begin(); it != m_dirs.end(); ++it)
	{
		result.append(*it);
		result.append("/");
	}
	result.append(m_name);
	return result;
}

String Path::buildWindows() const
{
	String result;
	if (!m_node.empty())
	{
		result.append("\\\\");
		result.append(m_node);
		result.append("\\");
	}
	else if (!m_device.empty())
	{
		result.append(m_device);
		result.append(":\\");
	}
	else if (m_absolute)
	{
		result.append("\\");
	}
	for (StringVec::const_iterator it = m_dirs.begin(); it != m_dirs.end(); ++it)
	{
		result.append(*it);
		result.append("\\");
	}
	result.append(m_name);
	return result;
}

String Path::buildVMS() const
{
	String result;
	if (!m_node.empty())
	{
		result.append(m_node);
		result.append("::");
	}
	if (!m_device.empty())
	{
		result.append(m_device);
		result.append(":");
	}
	if (!m_dirs.empty())
	{
		result.append("[");
		if (!m_absolute && m_dirs[0] != "..")
			result.append(".");
		for (StringVec::const_iterator it = m_dirs.begin(); it != m_dirs.end(); ++it)
		{
			if (it != m_dirs.begin() && *it != "..")
				result.append(".");
			if (*it == "..")
				result.append("-");
			else
				result.append(*it);
		}
		result.append("]");
	}
	result.append(m_name);
	if (!m_version.empty())
	{
		result.append(";");
		result.append(m_version);
	}
	return result;
}

String Path::transcode(const String& path)
{
#if defined(_WIN32) && defined(CUTE_WIN32_UTF8)
	std::wstring uniPath;
	Strings::toUTF16(path, uniPath);
	DWORD len = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, uniPath.c_str(), static_cast<int>(uniPath.length()), NULL, 0, NULL, NULL);
	if (len > 0)
	{
		String result;
		result.resize(len);
		DWORD rc = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, uniPath.c_str(), static_cast<int>(uniPath.length()), &result[0], static_cast<int>(result.size()), NULL, NULL);
		if (rc)
		{
			return result;
		}
	}
#endif
	return path;
}

CUTE_NS_END
