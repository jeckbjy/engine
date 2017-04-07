#pragma once
#include "Cute/Foundation.h"
#include <set>

CUTE_NS_BEGIN

class Path;
class TextIterator;

/// This class implements glob-style pattern matching
/// as known from Unix shells.
///
/// In the pattern string, '*' matches any sequence of characters,
/// '?' matches any single character, [SET] matches any single character 
/// in the specified set, [!SET] matches any character not in the  
/// specified set.
///
/// A set is composed of characters or ranges; a range looks like
/// character hyphen character (as in 0-9 or A-Z).
/// [0-9a-zA-Z_] is the set of characters allowed in C identifiers.
/// Any other character in the pattern must be matched exactly.
///
/// To suppress the special syntactic significance of any of '[]*?!-\',
/// and match the character exactly, precede it with a backslash.
///
/// All strings are assumed to be UTF-8 encoded.
class CUTE_CORE_API Glob
{
public:

	/// Flags that modify the matching behavior.
	enum Options
	{
		GLOB_DEFAULT		 = 0x00, /// default behavior
		GLOB_DOT_SPECIAL	 = 0x01, /// '*' and '?' do not match '.' at beginning of subject
		GLOB_FOLLOW_SYMLINKS = 0x02, /// follow symbolic links
		GLOB_CASELESS		 = 0x04, /// ignore case when comparing characters
		GLOB_DIRS_ONLY		 = 0x80  /// only glob for directories (for internal use only)
	};

	/// Creates the Glob, using the given pattern. The pattern
	/// must not be an empty string.
	///
	/// If the GLOB_DOT_SPECIAL option is specified, '*' and '?' do 
	/// not match '.' at the beginning of a matched subject. This is useful for
	/// making dot-files invisible in good old Unix-style.
	Glob(const String& pattern, int options = 0);
	~Glob();

	/// Matches the given subject against the glob pattern.
	/// Returns true if the subject matches the pattern, false
	/// otherwise.
	bool match(const String& subject);

	/// Creates a set of files that match the given pathPattern.
	///
	/// The path may be give in either Unix, Windows or VMS syntax and
	/// is automatically expanded by calling Path::expand().
	///
	/// The pattern may contain wildcard expressions even in intermediate
	/// directory names (e.g. /usr/include/*/*.h).
	///
	/// Note that, for obvious reasons, escaping characters in a pattern
	/// with a backslash does not work in Windows-style paths.
	///
	/// Directories that for whatever reason cannot be traversed are
	/// ignored.
	static void glob(const String& pathPattern, std::set<String>& files, int options = 0);

	/// Creates a set of files that match the given pathPattern.
	///
	/// The path may be give in either Unix, Windows or VMS syntax and
	/// is automatically expanded by calling Path::expand().
	///
	/// The pattern may contain wildcard expressions even in intermediate
	/// directory names (e.g. /usr/include/*/*.h).
	///
	/// Note that, for obvious reasons, escaping characters in a pattern
	/// with a backslash does not work in Windows-style paths.
	///
	/// Directories that for whatever reason cannot be traversed are
	/// ignored.
	static void glob(const char* pathPattern, std::set<String>& files, int options = 0);

	/// Creates a set of files that match the given pathPattern.
	///
	/// The pattern may contain wildcard expressions even in intermediate
	/// directory names (e.g. /usr/include/*/*.h).
	///
	/// Note that, for obvious reasons, escaping characters in a pattern
	/// with a backslash does not work in Windows-style paths.
	///
	/// Directories that for whatever reason cannot be traversed are
	/// ignored.
	static void glob(const Path& pathPattern, std::set<String>& files, int options = 0);
	
	/// Creates a set of files that match the given pathPattern, starting from basePath.
	///
	/// The pattern may contain wildcard expressions even in intermediate
	/// directory names (e.g. /usr/include/*/*.h).
	///
	/// Note that, for obvious reasons, escaping characters in a pattern
	/// with a backslash does not work in Windows-style paths.
	///
	/// Directories that for whatever reason cannot be traversed are
	/// ignored.
	static void glob(const Path& pathPattern, const Path& basePath, std::set<String>& files, int options = 0);

protected:
	bool match(TextIterator& itp, const TextIterator& endp, TextIterator& its, const TextIterator& ends);
	bool matchAfterAsterisk(TextIterator itp, const TextIterator& endp, TextIterator its, const TextIterator& ends);
	bool matchSet(TextIterator& itp, const TextIterator& endp, int c);
	static void collect(const Path& pathPattern, const Path& base, const Path& current, const String& pattern, std::set<String>& files, int options);
	static bool isDirectory(const Path& path, bool followSymlink);

private:
	Glob();
	Glob(const Glob&);
	Glob& operator = (const Glob&);

private:
	String m_pattern;
	int    m_options;
};

CUTE_NS_END