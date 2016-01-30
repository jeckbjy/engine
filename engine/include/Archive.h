#pragma once
#include "API.h"
#include "Ref.h"
#include "Stream.h"

CU_NS_BEGIN

class Archive;
struct FileInfo
{
	String filename;
	String path;
	String basename;
	size_t size;
};

typedef std::vector<FileInfo> FileInfoList;

// fs or zip file or remote web
class CU_API Archive : public Ref
{
protected:
	String  m_name;
	String  m_type;
	bool	m_readonly;
public:
	Archive(const String& name, const String& type)
		:m_name(name), m_type(type), m_readonly(true)
	{}
	virtual ~Archive(){}

	const String& getName() const { return m_name; }
	const String& getType() const { return m_type; }

	virtual void load() = 0;
	virtual void unload() = 0;

	virtual bool	isCaseSensitive() const = 0;
	virtual bool	isReadOnly() const { return m_readonly; }
	virtual bool	exists(const String& filename) = 0;
	virtual time_t	getModifiedTime(const String& filename) = 0;

	virtual Stream* open(const String& filename, bool readOnly = true) = 0;
	virtual Stream* create(const String& filename) = 0;
	virtual void	remove(const String& filename) = 0;
	virtual StringList list(bool recursive = true, bool dirs = false) = 0;
	virtual StringList find(const String& pattern, bool recursive = true, bool dirs = false) = 0;
	virtual FileInfoList listFileInfo(bool recursive = true, bool dirs = false) = 0;
	virtual FileInfoList findFileInfo(const String& pattern, bool recursive = true, bool dirs = false) = 0;
};

CU_NS_END