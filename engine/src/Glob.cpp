// module FileSystem
#include "Cute/Glob.h"
#include "Cute/Path.h"
#include "Cute/File.h"
#include "Cute/DirectoryIterator.h"
#include "Cute/UTF8Encoding.h"
#include "Cute/TextIterator.h"
#include "Cute/Unicode.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

Glob::Glob(const String& pattern, int options)
	: m_pattern(pattern)
	, m_options(options)
{
}

Glob::~Glob()
{
}

bool Glob::match(const String& subject)
{
	UTF8Encoding utf8;
	TextIterator itp(m_pattern, utf8);
	TextIterator endp(m_pattern);
	TextIterator its(subject, utf8);
	TextIterator ends(subject);

	if ((m_options & GLOB_DOT_SPECIAL) && its != ends && *its == '.' && (*itp == '?' || *itp == '*'))
		return false;
	else
		return match(itp, endp, its, ends);
}

void Glob::glob(const String& pathPattern, std::set<String>& files, int options)
{
	glob(Path(Path::expand(pathPattern), Path::PATH_GUESS), files, options);
}

void Glob::glob(const char* pathPattern, std::set<String>& files, int options)
{
	glob(Path(Path::expand(pathPattern), Path::PATH_GUESS), files, options);
}

void Glob::glob(const Path& pathPattern, std::set<String>& files, int options)
{
	Path pattern(pathPattern);
	pattern.makeDirectory(); // to simplify pattern handling later on
	Path base(pattern);
	Path absBase(base);
	absBase.makeAbsolute();
	// In case of UNC paths we must not pop the topmost directory
	// (which must not contain wildcards), otherwise collect() will fail
	// as one cannot create a DirectoryIterator with only a node name ("\\srv\").
	int minDepth = base.getNode().empty() ? 0 : 1;
	while (base.depth() > minDepth && base[base.depth() - 1] != "..")
	{
		base.popDirectory();
		absBase.popDirectory();
	}
	if (pathPattern.isDirectory())
		options |= GLOB_DIRS_ONLY;
	collect(pattern, absBase, base, pathPattern[base.depth()], files, options);
}

void Glob::glob(const Path& pathPattern, const Path& basePath, std::set<String>& files, int options)
{
	Path pattern(pathPattern);
	pattern.makeDirectory(); // to simplify pattern handling later on
	Path absBase(basePath);
	absBase.makeAbsolute();
	if (pathPattern.isDirectory())
		options |= GLOB_DIRS_ONLY;
	collect(pattern, absBase, basePath, pathPattern[basePath.depth()], files, options);
}

bool Glob::match(TextIterator& itp, const TextIterator& endp, TextIterator& its, const TextIterator& ends)
{
	while (itp != endp)
	{
		if (its == ends)
		{
			while (itp != endp && *itp == '*') ++itp;
			break;
		}
		switch (*itp)
		{
		case '?':
			++itp; ++its;
			break;
		case '*':
			if (++itp != endp)
			{
				while (its != ends && !matchAfterAsterisk(itp, endp, its, ends)) ++its;
				return its != ends;
			}
			return true;
		case '[':
			if (++itp != endp)
			{
				bool invert = *itp == '!';
				if (invert) ++itp;
				if (itp != endp)
				{
					bool mtch = matchSet(itp, endp, *its++);
					if ((invert && mtch) || (!invert && !mtch)) return false;
					break;
				}
			}
			throw SyntaxException("bad range syntax in glob pattern");
		case '\\':
			if (++itp == endp) throw SyntaxException("backslash must be followed by character in glob pattern");
			// fallthrough
		default:
			if (m_options & GLOB_CASELESS)
			{
				if (Unicode::toLower(*itp) != Unicode::toLower(*its)) return false;
			}
			else
			{
				if (*itp != *its) return false;
			}
			++itp; ++its;
		}
	}
	return itp == endp && its == ends;
}

bool Glob::matchAfterAsterisk(TextIterator itp, const TextIterator& endp, TextIterator its, const TextIterator& ends)
{
	return match(itp, endp, its, ends);
}

bool Glob::matchSet(TextIterator& itp, const TextIterator& endp, int c)
{
	if (m_options & GLOB_CASELESS)
		c = Unicode::toLower(c);

	while (itp != endp)
	{
		switch (*itp)
		{
		case ']':
			++itp;
			return false;
		case '\\':
			if (++itp == endp) throw SyntaxException("backslash must be followed by character in glob pattern");
		}
		int first = *itp;
		int last = first;
		if (++itp != endp && *itp == '-')
		{
			if (++itp != endp)
				last = *itp++;
			else
				throw SyntaxException("bad range syntax in glob pattern");
		}
		if (m_options & GLOB_CASELESS)
		{
			first = Unicode::toLower(first);
			last = Unicode::toLower(last);
		}
		if (first <= c && c <= last)
		{
			while (itp != endp)
			{
				switch (*itp)
				{
				case ']':
					++itp;
					return true;
				case '\\':
					if (++itp == endp) break;
				default:
					++itp;
				}
			}
			throw SyntaxException("range must be terminated by closing bracket in glob pattern");
		}
	}
	return false;
}

void Glob::collect(const Path& pathPattern, const Path& base, const Path& current, const String& pattern, std::set<String>& files, int options)
{
	try
	{
		String pp = pathPattern.toString();
		String basep = base.toString();
		String curp = current.toString();
		Glob g(pattern, options);
		DirectoryIterator it(base.toString());
		DirectoryIterator end;
		Path itPath;
		while (it != end)
		{
			itPath.assign(*it);
			const String& name = itPath.getFileName();
			if (g.match(name))
			{
				Path p(current);
				if (p.depth() < pathPattern.depth() - 1)
				{
					p.pushDirectory(name);
					collect(pathPattern, itPath, p, pathPattern[p.depth()], files, options);
				}
				else
				{
					p.setFileName(name);
					if (isDirectory(p, (options & GLOB_FOLLOW_SYMLINKS) != 0))
					{
						p.makeDirectory();
						files.insert(p.toString());
					}
					else if (!(options & GLOB_DIRS_ONLY))
					{
						files.insert(p.toString());
					}
				}
			}
			++it;
		}
	}
	catch (Exception&)
	{
	}
}

bool Glob::isDirectory(const Path& path, bool followSymlink)
{
	String p = path.toString();
	bool isDir = false;
	try
	{
		isDir = File::isDirectory(p);
	}
	catch (Exception&)
	{
		return false;
	}
	if (isDir)
	{
		return true;
	}
	else if (followSymlink && File::isLink(p))
	{
		try
		{
			// Test if link resolves to a directory.
			DirectoryIterator it(p);
			return true;
		}
		catch (Exception&)
		{
		}
	}
	return false;
}

CUTE_NS_END