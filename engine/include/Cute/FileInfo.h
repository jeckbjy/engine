#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API FileInfo
{
public:
#ifdef CUTE_OS_FAMILY_WINDOWS
	typedef DWORD attr_t;
#else
	typedef unsigned short attr_t;
#endif
	FileInfo();
	FileInfo(const String& path);
	~FileInfo();

	void refresh();

	bool exists() const;
	bool canRead() const;
	bool canWrite() const;
	bool canExecute() const;

	bool isFile() const;
	bool isLink() const;
	bool isDirectory() const;
	bool isDevice() const;
	bool isHidden() const;

	uint64 getCreationTime() const;
	uint64 getLastModified() const;
	void   setLastModified(uint64 time);

	uint64 getSize() const;
	void setSize(uint64 size);
	void setWriteable(bool flag = true);
	void setExecutable(bool flag = true);
	void setReadOnly(bool flag = true);

	void handleLastError();

	attr_t getAttributes() const;

private:
#ifdef CUTE_OS_FAMILY_WINDOWS
	typedef WIN32_FILE_ATTRIBUTE_DATA FileData;
#else
	typedef struct stat FileData;
#endif
	TString	 m_path;
	FileData m_data;
	error_t	 m_error;
};

CUTE_NS_END
