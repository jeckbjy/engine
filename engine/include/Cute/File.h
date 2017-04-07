#pragma once
#include "Cute/Foundation.h"
#include <vector>

CUTE_NS_BEGIN

class CUTE_CORE_API File
{
public:
	static bool		readAll(const String& path, String& data, bool removeBom = true);
	static bool		exists(const String& path);
	static bool		canRead(const String& path);
	static bool		canWrite(const String& path);
	static bool		canExecute(const String& path);

	static bool		isFile(const String& path);
	static bool		isDirectory(const String& path);
	static bool		isDevice(const String& path);
	static bool		isHidden(const String& path);
	static bool		isLink(const String& path);

	static uint64	getCreationTime(const String& path);
	static uint64	getLastModified(const String& path);
	static void		setLastModified(const String& path, uint64 ts);

	static void		setWriteable(const String& path, bool flag = true);
	static void		setExecutable(const String& path, bool flag = true);
	static void		setReadOnly(const String& path, bool flag = true);
	static void		setSize(const String& path, uint64 size);
	static uint64	getSize(const String& path);
	static uint64	totalSpace(const String& path);
	static uint64	usableSpace(const String& path);
	static uint64	freeSpace(const String& path);

	static void		copy(const String& src, const String& dst);
	static void		move(const String& src, const String& dst);
	static void		rename(const String& src, const String& dst);
	static void		remove(const String& path, bool recursive = false);

	static bool		createFile(const String& path);
	static bool		createDirectory(const String& path);
	static void		createDirectories(const String& path);

	static void		list(const String& path, std::vector<String>& files);

	static void		error(const String& path);

private:
	static void		_remove(const String& path);
};

CUTE_NS_END