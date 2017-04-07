#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// A Uniform Resource Identifier, as specified in RFC 3986.
/// 
/// The URI class provides methods for building URIs from their
/// parts, as well as for splitting URIs into their parts.
/// Furthermore, the class provides methods for resolving
/// relative URIs against base URIs.
///
/// The class automatically performs a few normalizations on
/// all URIs and URI parts passed to it:
///   * scheme identifiers are converted to lower case
///   * percent-encoded characters are decoded (except for the query string)
///   * optionally, dot segments are removed from paths (see normalize())
///
/// Note that dealing with query strings requires some precautions, as, internally,
/// query strings are stored in percent-encoded form, while all other parts of the URI
/// are stored in decoded form. While parsing query strings from properly encoded URLs
/// generally works, explicitly setting query strings with setQuery() or extracting
/// query strings with getQuery() may lead to ambiguities. See the descriptions of
/// setQuery(), setRawQuery(), getQuery() and getRawQuery() for more information.
class CUTE_CORE_API URI
{
public:
	/// URI-encodes the given string by escaping reserved and non-ASCII
	/// characters. The encoded string is appended to encodedStr.
	static void encode(const String& str, const String& reserved, String& encodedStr);

	/// URI-decodes the given string by replacing percent-encoded
	/// characters with the actual character. The decoded string
	/// is appended to decodedStr.
	///
	/// When plusAsSpace is true, non-encoded plus signs in the query are decoded as spaces.
	/// (http://www.w3.org/TR/html401/interact/forms.html#h-17.13.4.1)
	static void decode(const String& str, String& decodedStr, bool plusAsSpace = false);

public:
	typedef std::vector<std::pair<String, String> > QueryParameters;

	URI();
	URI(const String& uri);
	URI(const String& scheme, const String& pathEtc);
	URI(const String& scheme, const String& authority, const String& pathEtc);
	URI(const String& scheme, const String& authority, const String& path, const String& query);
	URI(const String& scheme, const String& authority, const String& path, const String& query, const String& fragment);
	URI(const URI& uri);
	URI(const URI& baseURI, const String& relativeURI);
	~URI();

	URI& operator = (const URI& uri);
	URI& operator = (const String& uri);


	void swap(URI& uri);
	void clear();
	String toString() const;

	const String& getScheme() const;
	const String& getUserInfo() const;
	const String& getHost() const;
	const String& getPath() const;
	const String& getRawQuery() const;
	const String& getFragment() const;

	String getAuthority() const;
	String getQuery() const;
	String getPathEtc() const;
	String getPathAndQuery() const;
	uint16_t getPort() const;

	QueryParameters getQueryParameters() const;

	void getPathSegments(std::vector<String>& segments);

	void setScheme(const String& scheme);
	void setUserInfo(const String& userinfo);
	void setHost(const String& host);
	void setPort(uint16_t port);
	void setAuthority(const String& authority);
	void setPath(const String& path);
	void setQuery(const String& query);
	void addQueryParameter(const String& param, const String& val = "");
	void setRawQuery(const String& query);
	void setQueryParameters(const QueryParameters& params);
	void setFragment(const String& fragment);
	void setPathEtc(const String& pathEtc);

	void resolve(const String& relativeURI);
	void resolve(const URI& relativeURI);

	/// Normalizes the URI by removing all but leading . and .. segments from the path.
	///
	/// If the first path segment in a relative path contains a colon (:), 
	/// such as in a Windows path containing a drive letter, a dot segment (./) 
	/// is prepended in accordance with section 3.3 of RFC 3986.
	void normalize();

	bool isRelative() const;
	bool empty() const;

	bool operator == (const URI& uri) const;
	bool operator == (const String& uri) const;
	bool operator != (const URI& uri) const;
	bool operator != (const String& uri) const;

protected:
	bool equals(const URI& uri) const;
	bool isWellKnownPort() const;
	uint16_t getWellKnownPort() const;

	void parse(const String& uri);
	void parseAuthority(String::const_iterator& it, const String::const_iterator& end);
	void parseHostAndPort(String::const_iterator& it, const String::const_iterator& end);
	void parsePath(String::const_iterator& it, const String::const_iterator& end);
	void parsePathEtc(String::const_iterator& it, const String::const_iterator& end);
	void parseQuery(String::const_iterator& it, const String::const_iterator& end);
	void parseFragment(String::const_iterator& it, const String::const_iterator& end);
	
	void mergePath(const String& path);
	void removeDotSegments(bool removeLeading = true);
	void buildPath(const std::vector<String>& segments, bool leadingSlash, bool trailingSlash);

	static void getPathSegments(const String& path, std::vector<String>& segments);

	static const String RESERVED_PATH;
	static const String RESERVED_QUERY;
	static const String RESERVED_FRAGMENT;
	static const String ILLEGAL;

private:
	String		m_scheme;
	String		m_userinfo;
	String		m_host;
	uint16_t	m_port;
	String		m_path;
	String		m_query;
	String		m_fragment;
};

//
// inlines
//
inline const String& URI::getScheme() const
{
	return m_scheme;
}

inline const String& URI::getUserInfo() const
{
	return m_userinfo;
}

inline const String& URI::getHost() const
{
	return m_host;
}

inline const String& URI::getPath() const
{
	return m_path;
}

inline const String& URI::getRawQuery() const
{
	return m_query;
}

inline const String& URI::getFragment() const
{
	return m_fragment;
}

inline void swap(URI& u1, URI& u2)
{
	u1.swap(u2);
}

CUTE_NS_END
