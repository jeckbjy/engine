#include "Cute/URI.h"
#include "Cute/String.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

const String URI::RESERVED_PATH = "?#";
const String URI::RESERVED_QUERY = "?#/";
const String URI::RESERVED_FRAGMENT = "";
const String URI::ILLEGAL = "%<>{}|\\\"^`";

void URI::encode(const String& str, const String& reserved, String& encodedStr)
{
	for (String::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		char c = *it;
		if ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			c == '-' || c == '_' ||
			c == '.' || c == '~')
		{
			encodedStr += c;
		}
		else if (c <= 0x20 || c >= 0x7F || ILLEGAL.find(c) != String::npos || reserved.find(c) != String::npos)
		{
			encodedStr += '%';
            encodedStr.appendHex(c, 2);
		}
		else encodedStr += c;
	}
}

void URI::decode(const String& str, String& decodedStr, bool plusAsSpace)
{
	bool inQuery = false;
	String::const_iterator it = str.begin();
	String::const_iterator end = str.end();
	while (it != end)
	{
		char c = *it++;
		if (c == '?') inQuery = true;
		// spaces may be encoded as plus signs in the query
		if (inQuery && plusAsSpace && c == '+') c = ' ';
		else if (c == '%')
		{
			if (it == end) throw URISyntaxException("URI encoding: no hex digit following percent sign", str);
			char hi = *it++;
			if (it == end) throw URISyntaxException("URI encoding: two hex digits must follow percent sign", str);
			char lo = *it++;
			if (hi >= '0' && hi <= '9')
				c = hi - '0';
			else if (hi >= 'A' && hi <= 'F')
				c = hi - 'A' + 10;
			else if (hi >= 'a' && hi <= 'f')
				c = hi - 'a' + 10;
			else throw URISyntaxException("URI encoding: not a hex digit");
			c *= 16;
			if (lo >= '0' && lo <= '9')
				c += lo - '0';
			else if (lo >= 'A' && lo <= 'F')
				c += lo - 'A' + 10;
			else if (lo >= 'a' && lo <= 'f')
				c += lo - 'a' + 10;
			else throw URISyntaxException("URI encoding: not a hex digit");
		}
		decodedStr += c;
	}
}

URI::URI() 
	: m_port(0)
{
}

URI::URI(const String& uri) 
	: m_port(0)
{
	parse(uri);
}

URI::URI(const String& scheme, const String& pathEtc) 
	: m_scheme(scheme)
	, m_port(0)
{
    m_scheme.toLower();
	m_port = getWellKnownPort();
	String::const_iterator beg = pathEtc.begin();
	String::const_iterator end = pathEtc.end();
	parsePathEtc(beg, end);
}

URI::URI(const String& scheme, const String& authority, const String& pathEtc) 
	: m_scheme(scheme)
{
    m_scheme.toLower();
	String::const_iterator beg = authority.begin();
	String::const_iterator end = authority.end();
	parseAuthority(beg, end);
	beg = pathEtc.begin();
	end = pathEtc.end();
	parsePathEtc(beg, end);
}

URI::URI(const String& scheme, const String& authority, const String& path, const String& query) 
	: m_scheme(scheme)
	, m_path(path)
	, m_query(query)
{
    m_scheme.toLower();
	String::const_iterator beg = authority.begin();
	String::const_iterator end = authority.end();
	parseAuthority(beg, end);
}

URI::URI(const String& scheme, const String& authority, const String& path, const String& query, const String& fragment) 
	: m_scheme(scheme)
	, m_path(path)
	, m_query(query)
	, m_fragment(fragment)
{
	m_scheme.toLower();
	String::const_iterator beg = authority.begin();
	String::const_iterator end = authority.end();
	parseAuthority(beg, end);
}

URI::URI(const URI& uri) 
	: m_scheme(uri.m_scheme)
	, m_userinfo(uri.m_userinfo)
	, m_host(uri.m_host)
	, m_port(uri.m_port)
	, m_path(uri.m_path)
	, m_query(uri.m_query)
	, m_fragment(uri.m_fragment)
{
}

URI::URI(const URI& baseURI, const String& relativeURI) 
	: m_scheme(baseURI.m_scheme)
	, m_userinfo(baseURI.m_userinfo)
	, m_host(baseURI.m_host)
	, m_port(baseURI.m_port)
	, m_path(baseURI.m_path)
	, m_query(baseURI.m_query)
	, m_fragment(baseURI.m_fragment)
{
	resolve(relativeURI);
}

//URI::URI(const Path& path) :
//	m_scheme("file"),
//	m_port(0)
//{
//	Path absolutePath(path);
//	absolutePath.makeAbsolute();
//	m_path = absolutePath.toString(Path::PATH_UNIX);
//}

URI::~URI()
{
}

URI& URI::operator = (const URI& uri)
{
	if (&uri != this)
	{
		m_scheme = uri.m_scheme;
		m_userinfo = uri.m_userinfo;
		m_host = uri.m_host;
		m_port = uri.m_port;
		m_path = uri.m_path;
		m_query = uri.m_query;
		m_fragment = uri.m_fragment;
	}
	return *this;
}

URI& URI::operator = (const String& uri)
{
	clear();
	parse(uri);
	return *this;
}

void URI::swap(URI& uri)
{
	std::swap(m_scheme, uri.m_scheme);
	std::swap(m_userinfo, uri.m_userinfo);
	std::swap(m_host, uri.m_host);
	std::swap(m_port, uri.m_port);
	std::swap(m_path, uri.m_path);
	std::swap(m_query, uri.m_query);
	std::swap(m_fragment, uri.m_fragment);
}

void URI::clear()
{
	m_scheme.clear();
	m_userinfo.clear();
	m_host.clear();
	m_port = 0;
	m_path.clear();
	m_query.clear();
	m_fragment.clear();
}

String URI::toString() const
{
	String uri;
	if (isRelative())
	{
		encode(m_path, RESERVED_PATH, uri);
	}
	else
	{
		uri = m_scheme;
		uri += ':';
		String auth = getAuthority();
		if (!auth.empty() || m_scheme == "file")
		{
			uri.append("//");
			uri.append(auth);
		}
		if (!m_path.empty())
		{
			if (!auth.empty() && m_path[0] != '/')
				uri += '/';
			encode(m_path, RESERVED_PATH, uri);
		}
		else if (!m_query.empty() || !m_fragment.empty())
		{
			uri += '/';
		}
	}
	if (!m_query.empty())
	{
		uri += '?';
		uri.append(m_query);
	}
	if (!m_fragment.empty())
	{
		uri += '#';
		encode(m_fragment, RESERVED_FRAGMENT, uri);
	}
	return uri;
}

void URI::setScheme(const String& scheme)
{
	m_scheme = scheme;
    m_scheme.toLower();
	if (m_port == 0)
		m_port = getWellKnownPort();
}

void URI::setUserInfo(const String& userInfo)
{
	m_userinfo.clear();
	decode(userInfo, m_userinfo);
}

void URI::setHost(const String& host)
{
	m_host = host;
}

unsigned short URI::getPort() const
{
	if (m_port == 0)
		return getWellKnownPort();
	else
		return m_port;
}

void URI::setPort(unsigned short port)
{
	m_port = port;
}

String URI::getAuthority() const
{
	String auth;
	if (!m_userinfo.empty())
	{
		auth.append(m_userinfo);
		auth += '@';
	}
	if (m_host.find(':') != String::npos)
	{
		auth += '[';
		auth += m_host;
		auth += ']';
	}
	else auth.append(m_host);
	if (m_port && !isWellKnownPort())
	{
		auth += ':';
		auth.appends(m_port);
	}
	return auth;
}

void URI::setAuthority(const String& authority)
{
	m_userinfo.clear();
	m_host.clear();
	m_port = 0;
	String::const_iterator beg = authority.begin();
	String::const_iterator end = authority.end();
	parseAuthority(beg, end);
}

void URI::setPath(const String& path)
{
	m_path.clear();
	decode(path, m_path);
}

void URI::setRawQuery(const String& query)
{
	m_query = query;
}

void URI::setQuery(const String& query)
{
	m_query.clear();
	encode(query, RESERVED_QUERY, m_query);
}

void URI::addQueryParameter(const String& param, const String& val)
{
	String reserved(RESERVED_QUERY);
	reserved += "=&";
	if (!m_query.empty()) m_query += '&';
	encode(param, reserved, m_query);
	m_query += '=';
	encode(val, reserved, m_query);
}

String URI::getQuery() const
{
	String query;
	decode(m_query, query);
	return query;
}

URI::QueryParameters URI::getQueryParameters() const
{
	QueryParameters result;
	String::const_iterator it(m_query.begin());
	String::const_iterator end(m_query.end());
	while (it != end)
	{
		String name;
		String value;
		while (it != end && *it != '=' && *it != '&')
		{
			if (*it == '+')
				name += ' ';
			else
				name += *it;
			++it;
		}
		if (it != end && *it == '=')
		{
			++it;
			while (it != end && *it != '&')
			{
				if (*it == '+')
					value += ' ';
				else
					value += *it;
				++it;
			}
		}
		String decodedName;
		String decodedValue;
		URI::decode(name, decodedName);
		URI::decode(value, decodedValue);
		result.push_back(std::make_pair(decodedName, decodedValue));
		if (it != end && *it == '&') ++it;
	}
	return result;
}

void URI::setQueryParameters(const QueryParameters& params)
{
	m_query.clear();
	for (QueryParameters::const_iterator it = params.begin(); it != params.end(); ++it)
	{
		addQueryParameter(it->first, it->second);
	}
}

void URI::setFragment(const String& fragment)
{
	m_fragment.clear();
	decode(fragment, m_fragment);
}

void URI::setPathEtc(const String& pathEtc)
{
	m_path.clear();
	m_query.clear();
	m_fragment.clear();
	String::const_iterator beg = pathEtc.begin();
	String::const_iterator end = pathEtc.end();
	parsePathEtc(beg, end);
}

String URI::getPathEtc() const
{
	String pathEtc;
	encode(m_path, RESERVED_PATH, pathEtc);
	if (!m_query.empty())
	{
		pathEtc += '?';
		pathEtc += m_query;
	}
	if (!m_fragment.empty())
	{
		pathEtc += '#';
		encode(m_fragment, RESERVED_FRAGMENT, pathEtc);
	}
	return pathEtc;
}

String URI::getPathAndQuery() const
{
	String pathAndQuery;
	encode(m_path, RESERVED_PATH, pathAndQuery);
	if (!m_query.empty())
	{
		pathAndQuery += '?';
		pathAndQuery += m_query;
	}
	return pathAndQuery;
}

void URI::resolve(const String& relativeURI)
{
	URI parsedURI(relativeURI);
	resolve(parsedURI);
}

void URI::resolve(const URI& relativeURI)
{
	if (!relativeURI.m_scheme.empty())
	{
		m_scheme = relativeURI.m_scheme;
		m_userinfo = relativeURI.m_userinfo;
		m_host = relativeURI.m_host;
		m_port = relativeURI.m_port;
		m_path = relativeURI.m_path;
		m_query = relativeURI.m_query;
		removeDotSegments();
	}
	else
	{
		if (!relativeURI.m_host.empty())
		{
			m_userinfo = relativeURI.m_userinfo;
			m_host = relativeURI.m_host;
			m_port = relativeURI.m_port;
			m_path = relativeURI.m_path;
			m_query = relativeURI.m_query;
			removeDotSegments();
		}
		else
		{
			if (relativeURI.m_path.empty())
			{
				if (!relativeURI.m_query.empty())
					m_query = relativeURI.m_query;
			}
			else
			{
				if (relativeURI.m_path[0] == '/')
				{
					m_path = relativeURI.m_path;
					removeDotSegments();
				}
				else
				{
					mergePath(relativeURI.m_path);
				}
				m_query = relativeURI.m_query;
			}
		}
	}
	m_fragment = relativeURI.m_fragment;
}

bool URI::isRelative() const
{
	return m_scheme.empty();
}

bool URI::empty() const
{
	return m_scheme.empty() && m_host.empty() && m_path.empty() && m_query.empty() && m_fragment.empty();
}

bool URI::operator == (const URI& uri) const
{
	return equals(uri);
}

bool URI::operator == (const String& uri) const
{
	URI parsedURI(uri);
	return equals(parsedURI);
}

bool URI::operator != (const URI& uri) const
{
	return !equals(uri);
}

bool URI::operator != (const String& uri) const
{
	URI parsedURI(uri);
	return !equals(parsedURI);
}

bool URI::equals(const URI& uri) const
{
	return m_scheme == uri.m_scheme
		&& m_userinfo == uri.m_userinfo
		&& m_host == uri.m_host
		&& getPort() == uri.getPort()
		&& m_path == uri.m_path
		&& m_query == uri.m_query
		&& m_fragment == uri.m_fragment;
}

void URI::normalize()
{
	removeDotSegments(!isRelative());
}

void URI::removeDotSegments(bool removeLeading)
{
	if (m_path.empty()) return;

	bool leadingSlash = *(m_path.begin()) == '/';
	bool trailingSlash = *(m_path.rbegin()) == '/';
	std::vector<String> segments;
	std::vector<String> normalizedSegments;
	getPathSegments(segments);
	for (std::vector<String>::const_iterator it = segments.begin(); it != segments.end(); ++it)
	{
		if (*it == "..")
		{
			if (!normalizedSegments.empty())
			{
				if (normalizedSegments.back() == "..")
					normalizedSegments.push_back(*it);
				else
					normalizedSegments.pop_back();
			}
			else if (!removeLeading)
			{
				normalizedSegments.push_back(*it);
			}
		}
		else if (*it != ".")
		{
			normalizedSegments.push_back(*it);
		}
	}
	buildPath(normalizedSegments, leadingSlash, trailingSlash);
}

void URI::getPathSegments(std::vector<String>& segments)
{
	getPathSegments(m_path, segments);
}

void URI::getPathSegments(const String& path, std::vector<String>& segments)
{
	String::const_iterator it = path.begin();
	String::const_iterator end = path.end();
	String seg;
	while (it != end)
	{
		if (*it == '/')
		{
			if (!seg.empty())
			{
				segments.push_back(seg);
				seg.clear();
			}
		}
		else seg += *it;
		++it;
	}
	if (!seg.empty())
		segments.push_back(seg);
}

bool URI::isWellKnownPort() const
{
	return m_port == getWellKnownPort();
}

unsigned short URI::getWellKnownPort() const
{
	if (m_scheme == "ftp")
		return 21;
	else if (m_scheme == "ssh")
		return 22;
	else if (m_scheme == "telnet")
		return 23;
	else if (m_scheme == "http" || m_scheme == "ws")
		return 80;
	else if (m_scheme == "nntp")
		return 119;
	else if (m_scheme == "ldap")
		return 389;
	else if (m_scheme == "https" || m_scheme == "wss")
		return 443;
	else if (m_scheme == "rtsp")
		return 554;
	else if (m_scheme == "sip")
		return 5060;
	else if (m_scheme == "sips")
		return 5061;
	else if (m_scheme == "xmpp")
		return 5222;
	else
		return 0;
}

void URI::parse(const String& uri)
{
	String::const_iterator it = uri.begin();
	String::const_iterator end = uri.end();
	if (it == end) return;
	if (*it != '/' && *it != '.' && *it != '?' && *it != '#')
	{
		String scheme;
		while (it != end && *it != ':' && *it != '?' && *it != '#' && *it != '/') scheme += *it++;
		if (it != end && *it == ':')
		{
			++it;
			if (it == end) throw URISyntaxException("URI scheme must be followed by authority or path", uri);
			setScheme(scheme);
			if (*it == '/')
			{
				++it;
				if (it != end && *it == '/')
				{
					++it;
					parseAuthority(it, end);
				}
				else --it;
			}
			parsePathEtc(it, end);
		}
		else
		{
			it = uri.begin();
			parsePathEtc(it, end);
		}
	}
	else parsePathEtc(it, end);
}

void URI::parseAuthority(String::const_iterator& it, const String::const_iterator& end)
{
	String userInfo;
	String part;
	while (it != end && *it != '/' && *it != '?' && *it != '#')
	{
		if (*it == '@')
		{
			userInfo = part;
			part.clear();
		}
		else part += *it;
		++it;
	}
	String::const_iterator pbeg = part.begin();
	String::const_iterator pend = part.end();
	parseHostAndPort(pbeg, pend);
	m_userinfo = userInfo;
}

void URI::parseHostAndPort(String::const_iterator& it, const String::const_iterator& end)
{
	if (it == end) return;
	String host;
	if (*it == '[')
	{
		// IPv6 address
		++it;
		while (it != end && *it != ']') host += *it++;
		if (it == end) throw URISyntaxException("unterminated IPv6 address");
		++it;
	}
	else
	{
		while (it != end && *it != ':') host += *it++;
	}
	if (it != end && *it == ':')
	{
		++it;
		String port;
		while (it != end) port += *it++;
		if (!port.empty())
		{
			int nport = 0;
			if (port.parse(nport) && nport > 0 && nport < 65536)
				m_port = (unsigned short)nport;
			else
				throw URISyntaxException("bad or invalid port number", port);
		}
		else m_port = getWellKnownPort();
	}
	else m_port = getWellKnownPort();
	m_host = host;
    m_host.toLower();
}

void URI::parsePath(String::const_iterator& it, const String::const_iterator& end)
{
	String path;
	while (it != end && *it != '?' && *it != '#') path += *it++;
	decode(path, m_path);
}

void URI::parsePathEtc(String::const_iterator& it, const String::const_iterator& end)
{
	if (it == end) return;
	if (*it != '?' && *it != '#')
		parsePath(it, end);
	if (it != end && *it == '?')
	{
		++it;
		parseQuery(it, end);
	}
	if (it != end && *it == '#')
	{
		++it;
		parseFragment(it, end);
	}
}

void URI::parseQuery(String::const_iterator& it, const String::const_iterator& end)
{
	m_query.clear();
	while (it != end && *it != '#') m_query += *it++;
}

void URI::parseFragment(String::const_iterator& it, const String::const_iterator& end)
{
	String fragment;
	while (it != end) fragment += *it++;
	decode(fragment, m_fragment);
}

void URI::mergePath(const String& path)
{
	std::vector<String> segments;
	std::vector<String> normalizedSegments;
	bool addLeadingSlash = false;
	if (!m_path.empty())
	{
		getPathSegments(segments);
		bool endsWithSlash = *(m_path.rbegin()) == '/';
		if (!endsWithSlash && !segments.empty())
			segments.pop_back();
		addLeadingSlash = m_path[0] == '/';
	}
	getPathSegments(path, segments);
	addLeadingSlash = addLeadingSlash || (!path.empty() && path[0] == '/');
	bool hasTrailingSlash = (!path.empty() && *(path.rbegin()) == '/');
	bool addTrailingSlash = false;
	for (std::vector<String>::const_iterator it = segments.begin(); it != segments.end(); ++it)
	{
		if (*it == "..")
		{
			addTrailingSlash = true;
			if (!normalizedSegments.empty())
				normalizedSegments.pop_back();
		}
		else if (*it != ".")
		{
			addTrailingSlash = false;
			normalizedSegments.push_back(*it);
		}
		else addTrailingSlash = true;
	}
	buildPath(normalizedSegments, addLeadingSlash, hasTrailingSlash || addTrailingSlash);
}

void URI::buildPath(const std::vector<String>& segments, bool leadingSlash, bool trailingSlash)
{
	m_path.clear();
	bool first = true;
	for (std::vector<String>::const_iterator it = segments.begin(); it != segments.end(); ++it)
	{
		if (first)
		{
			first = false;
			if (leadingSlash)
				m_path += '/';
			else if (m_scheme.empty() && (*it).find(':') != String::npos)
				m_path.append("./");
		}
		else m_path += '/';
		m_path.append(*it);
	}
	if (trailingSlash)
		m_path += '/';
}

CUTE_NS_END
