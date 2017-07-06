#pragma once
#include "Cute/Foundation.h"
#include <vector>

CUTE_NS_BEGIN

/// This class represents filesystem paths in a 
/// platform-independent manner.
/// Unix, Windows and OpenVMS all use a different
/// syntax for filesystem paths.
/// This class can work with all three formats.
/// A path is made up of an optional node name
/// (only Windows and OpenVMS), an optional
/// device name (also only Windows and OpenVMS),
/// a list of directory names and an optional
/// filename.
class CUTE_CORE_API Path
{
public:
	enum Style
	{
		PATH_UNIX,            /// Unix-style path
		PATH_URI = PATH_UNIX, /// URI-style path, same as Unix-style
		PATH_WINDOWS,         /// Windows-style path
		PATH_VMS,             /// VMS-style path
		PATH_NATIVE,          /// The current platform's native style
		PATH_GUESS            /// Guess the style by examining the path
	};

	typedef std::vector<String> StringVec;

	Path();
	Path(bool absolute);
	Path(const String& path);
	Path(const String& path, Style style);
	Path(const Path& path);
	Path(const Path& parent, const String& fileName);
	Path(const Path& parent, const Path& relative);
	~Path();

	Path& operator = (const Path& path);
	Path& operator = (const String& path);

	void swap(Path& path);
	/// Swaps the path with another one.

	Path& assign(const String& path);
	/// Assigns a string containing a path in native format.

	Path& assign(const String& path, Style style);
	/// Assigns a string containing a path.

	Path& assign(const Path& path);
	/// Assigns the given path.

	Path& assign(const char* path);
	/// Assigns a string containing a path.

	String toString() const;
	/// Returns a string containing the path in native format.

	String toString(Style style) const;
	/// Returns a string containing the path in the given format.

	Path& parse(const String& path);
	/// Same as assign().

	Path& parse(const String& path, Style style);
	/// Assigns a string containing a path.

	bool tryParse(const String& path);
	/// Tries to interpret the given string as a path
	/// in native format.
	/// If the path is syntactically valid, assigns the
	/// path and returns true. Otherwise leaves the 
	/// object unchanged and returns false.

	bool tryParse(const String& path, Style style);
	/// Tries to interpret the given string as a path,
	/// according to the given style.
	/// If the path is syntactically valid, assigns the
	/// path and returns true. Otherwise leaves the
	/// object unchanged and returns false.

	Path& parseDirectory(const String& path);
	/// The resulting path always refers to a directory and
	/// the filename part is empty.

	Path& parseDirectory(const String& path, Style style);
	/// The resulting path always refers to a directory and
	/// the filename part is empty.

	Path& makeDirectory();
	/// If the path contains a filename, the filename is appended
	/// to the directory list and cleared. Thus the resulting path
	/// always refers to a directory.

	Path& makeFile();
	/// If the path contains no filename, the last directory
	/// becomes the filename.

	Path& makeParent();
	/// Makes the path refer to its parent.

	Path& makeAbsolute();
	/// Makes the path absolute if it is relative.
	/// The current working directory is taken as base directory.

	Path& makeAbsolute(const Path& base);
	/// Makes the path absolute if it is relative.
	/// The given path is taken as base. 

	Path& append(const Path& path);
	/// Appends the given path.

	Path& resolve(const Path& path);
	/// Resolves the given path against the current one.
	///
	/// If the given path is absolute, it replaces the current one.
	/// Otherwise, the relative path is appended to the current path.

	bool isAbsolute() const;
	/// Returns true iff the path is absolute.

	bool isRelative() const;
	/// Returns true iff the path is relative.

	bool isDirectory() const;
	/// Returns true iff the path references a directory
	/// (the filename part is empty).

	bool isFile() const;
	/// Returns true iff the path references a file
	/// (the filename part is not empty).

	Path& setNode(const String& node);
	/// Sets the node name.
	/// Setting a non-empty node automatically makes
	/// the path an absolute one.

	const String& getNode() const;
	/// Returns the node name.

	Path& setDevice(const String& device);
	/// Sets the device name.
	/// Setting a non-empty device automatically makes
	/// the path an absolute one.

	const String& getDevice() const;
	/// Returns the device name.

	int depth() const;
	/// Returns the number of directories in the directory list.

	const String& directory(int n) const;
	/// Returns the n'th directory in the directory list.
	/// If n == depth(), returns the filename.

	const String& operator [] (int n) const;
	/// Returns the n'th directory in the directory list.
	/// If n == depth(), returns the filename.

	Path& pushDirectory(const String& dir);
	/// Adds a directory to the directory list.

	Path& popDirectory();
	/// Removes the last directory from the directory list.

	Path& popFrontDirectory();
	/// Removes the first directory from the directory list.

	Path& setFileName(const String& name);
	/// Sets the filename.

	const String& getFileName() const;
	/// Returns the filename.

	Path& setBaseName(const String& name);
	/// Sets the basename part of the filename and
	/// does not change the extension.

	String getBaseName() const;
	/// Returns the basename (the filename sans
	/// extension) of the path.

	Path& setExtension(const String& extension);
	/// Sets the filename extension.

	String getExtension() const;
	/// Returns the filename extension.

	const String& version() const;
	/// Returns the file version. VMS only.

	Path& clear();
	/// Clears all components.

	Path parent() const;
	/// Returns a path referring to the path's
	/// directory.

	Path absolute() const;
	/// Returns an absolute variant of the path,
	/// taking the current working directory as base.

	Path absolute(const Path& base) const;
	/// Returns an absolute variant of the path,
	/// taking the given path as base.

	static Path forDirectory(const String& path);
	/// Creates a path referring to a directory.

	static Path forDirectory(const String& path, Style style);
	/// Creates a path referring to a directory.

	static char separator();
	/// Returns the platform's path name separator, which separates
	/// the components (names) in a path. 
	///
	/// On Unix systems, this is the slash '/'. On Windows systems, 
	/// this is the backslash '\'. On OpenVMS systems, this is the
	/// period '.'.

	static char pathSeparator();
	/// Returns the platform's path separator, which separates
	/// single paths in a list of paths.
	///
	/// On Unix systems, this is the colon ':'. On Windows systems,
	/// this is the semicolon ';'. On OpenVMS systems, this is the
	/// comma ','.

	static String current();
	/// Returns the current working directory.

	static String home();
	/// Returns the user's home directory.

	static String configHome();
	/// Returns the user's config directory.
	///
	/// On Unix systems, this is the '~/.config/'. On Windows systems,
	/// this is '%APPDATA%'.

	static String dataHome();
	/// Returns the user's data directory.
	///
	/// On Unix systems, this is the '~/.local/share/'. On Windows systems,
	/// this is '%APPDATA%'.

	static String tempHome();
	/// Returns the user's temp directory.
	///
	/// On Unix systems, this is the '~/.local/temp/'.

	static String cacheHome();
	/// Returns the user's cache directory.
	///
	/// On Unix systems, this is the '~/.cache/'. On Windows systems,
	/// this is '%APPDATA%'.

	static String temp();
	/// Returns the temporary directory.

	static String config();
	/// Returns the systemwide config directory.
	///
	/// On Unix systems, this is the '/etc/'.

	static String null();
	/// Returns the name of the null device.

	static String expand(const String& path);
	/// Expands all environment variables contained in the path.
	///
	/// On Unix, a tilde as first character in the path is
	/// replaced with the path to user's home directory.

	static void listRoots(std::vector<String>& roots);
	/// Fills the vector with all filesystem roots available on the
	/// system. On Unix, there is exactly one root, "/".
	/// On Windows, the roots are the drive letters.
	/// On OpenVMS, the roots are the mounted disks.

	static bool find(StringVec::const_iterator it, StringVec::const_iterator end, const String& name, Path& path);
	/// Searches the file with the given name in the locations (paths) specified
	/// by it and end. A relative path may be given in name.
	///
	/// If the file is found in one of the locations, the complete
	/// path of the file is stored in the path given as argument and true is returned. 
	/// Otherwise false is returned and the path argument remains unchanged.

	static bool find(const String& pathList, const String& name, Path& path);
	/// Searches the file with the given name in the locations (paths) specified
	/// in pathList. The paths in pathList must be delimited by the platform's
	/// path separator (see pathSeparator()). A relative path may be given in name.
	///
	/// If the file is found in one of the locations, the complete
	/// path of the file is stored in the path given as argument and true is returned. 
	/// Otherwise false is returned and the path argument remains unchanged.

	static String transcode(const String& path);
	/// On Windows, if POCO has been compiled with Windows UTF-8 support 
	/// (CUTE_WIN32_UTF8), this function converts a string (usually containing a path) 
	/// encoded in UTF-8 into a string encoded in the current Windows code page.
	/// 
	/// This function should be used for every string passed as a file name to
	/// a string stream or fopen().
	///
	/// On all other platforms, or if POCO has not been compiled with Windows UTF-8
	/// support, this function returns the string unchanged.

protected:
	void parseUnix(const String& path);
	void parseWindows(const String& path);
	void parseVMS(const String& path);
	void parseGuess(const String& path);
	String buildUnix() const;
	String buildWindows() const;
	String buildVMS() const;

private:
	String      m_node;
	String	    m_device;
	String      m_name;
	String		m_version;
	StringVec   m_dirs;
	bool        m_absolute;
};

//
// inlines
//
inline bool Path::isAbsolute() const
{
	return m_absolute;
}

inline bool Path::isRelative() const
{
	return !m_absolute;
}

inline bool Path::isDirectory() const
{
	return m_name.empty();
}

inline bool Path::isFile() const
{
	return !m_name.empty();
}

inline Path& Path::parse(const String& path)
{
	return assign(path);
}

inline Path& Path::parse(const String& path, Style style)
{
	return assign(path, style);
}

inline const String& Path::getNode() const
{
	return m_node;
}

inline const String& Path::getDevice() const
{
	return m_device;
}

inline const String& Path::getFileName() const
{
	return m_name;
}

inline int Path::depth() const
{
	return int(m_dirs.size());
}

inline const String& Path::version() const
{
	return m_version;
}

inline Path Path::forDirectory(const String& path)
{
	Path p;
	return p.parseDirectory(path);
}

inline Path Path::forDirectory(const String& path, Style style)
{
	Path p;
	return p.parseDirectory(path, style);
}

inline char Path::separator()
{
#if defined(CUTE_OS_FAMILY_VMS)
	return '.';
#elif defined(CUTE_OS_FAMILY_WINDOWS)
	return '\\';
#else
	return '/';
#endif
}

inline char Path::pathSeparator()
{
#if defined(CUTE_OS_FAMILY_VMS)
	return ',';
#elif defined(CUTE_OS_FAMILY_WINDOWS)
	return ';';
#else
	return ':';
#endif
}

inline void swap(Path& p1, Path& p2)
{
	p1.swap(p2);
}

CUTE_NS_END
