#pragma once
#include "Cute/Foundation.h"
#include "Cute/StringTokenizer.h"
#include "Cute/String.h"
#include "Cute/Ref.h"
#include "Cute/RefPtr.h"
#include <vector>
#include <map>

CUTE_NS_BEGIN

class CUTE_CORE_API CellVector
{
public:
	CellVector();

	void reset();
	void skip(size_t count = 1);
	bool eof() const;
	void push(const String& cell);
	const String& next();

public:
	CellVector& operator>>(String& cell)	{ cell = next(); return *this; }
	CellVector& operator>>(bool& cell)		{ return convert(next(), cell); }
	CellVector& operator>>(char& cell)		{ return convert(next(), cell); }
	CellVector& operator>>(int8_t& cell)	{ return convert(next(), cell); }
	CellVector& operator>>(int16_t& cell)	{ return convert(next(), cell); }
	CellVector& operator>>(int32_t& cell)	{ return convert(next(), cell); }
	CellVector& operator>>(int64_t& cell)	{ return convert(next(), cell); }
	CellVector& operator>>(uint8_t& cell)	{ return convert(next(), cell); }
	CellVector& operator>>(uint16_t& cell)	{ return convert(next(), cell); }
	CellVector& operator>>(uint32_t& cell)	{ return convert(next(), cell); }
	CellVector& operator>>(uint64_t& cell)	{ return convert(next(), cell); }
	CellVector& operator>>(float& cell)		{ return convert(next(), cell); }
	CellVector& operator>>(double& cell)	{ return convert(next(), cell); }
	// 要求外部类型都必须能够stringstream流处理
	template<typename T>
	CellVector& operator>>(T& cell)
	{
		std::stringstream ss(next());
		ss >> cell;
		return *this;
	}

	// 数组类型,每个元素对应一个cell
	template<typename T, std::size_t N>
	CellVector& operator>>(T(&data)[N])
	{
		for (std::size_t i = 0; i < N; ++i)
			*this >> data[i];

		return *this;
	}

	// 默认以','分割
	template<typename U, typename V>
	CellVector& operator>>(std::pair<U, V>& cells)
	{
		sscanf(next().c_str(), "%d , %d", &cells.first, &cells.second);
	}

	// 逗号分割的cells
	template<class T>
	CellVector& operator>>(std::vector<T>& cells)
	{
		cells.clear();

		StringTokenizer fields(next(), ",");

		T value;
		for (auto itor = fields.begin(); itor != fields.end(); ++itor)
		{
			convert(*itor, value);
			cells.push_back(value);
		}

		return *this;
	}

	// map类型
	template<class U, class V>
	CellVector& operator>>(std::map<U, V>& cells)
	{
		static char buff[2][20] = { 0 };

		cells.clear();

		StringTokenizer fields(next(), "|");

		U key;
		V value;
		for (auto itor = fields.begin(); itor != fields.end(); ++itor)
		{
			std::string& field = *itor;
			trim(field);
			if (field.empty())
				continue;

			sscanf(field.c_str(), "%[^,] ,%s", buff[0], buff[1]);
			convert(buff[0], key);
			convert(buff[1], value);
			cells[key] = value;
		}
		return *this;
	}

private:
	template<typename T>
	CellVector& convert(const String& str, T& data)
	{
		std::stringstream ss(str);
		ss >> data;

		return *this;
	}

private:
	typedef std::vector<String> Container;
	Container m_cells;
	size_t	  m_index;
};

struct CUTE_CORE_API IRow : public Ref<IRow>
{
public:
	IRow() :m_tabID(0){}
	virtual ~IRow(){}
	virtual void decode(CellVector& cells) = 0;

	void setID(uint32_t id) { m_tabID = id; }
	uint32_t getID() const { return m_tabID; }

	bool expired() const { return m_expired; }

protected:
	friend class ITable;
	uint32_t m_tabID;
	bool	 m_expired;	// 数据是否已经过期
};

/************************************************************************/
/*
二维表格，最多支持两列索引,支持\t和','(csv)格式解析
要求第一列必须是唯一ID并且前三行是注释上，分别是类型，名字，注释
#开始表示注释行
支持读取bom标识,并自动过滤
支持重复加载，相同id的指针不会发生变化
外边使用弱引用防止数据过期
*/
/************************************************************************/
class CUTE_CORE_API ITable
{
	enum
	{
		COMMENT_COUNT = 3
	};
public:
	ITable();
	virtual ~ITable();

	bool load(const String& path);
	bool reload();

	size_t size() const { return m_rows.size(); }
	const String& name() const { return m_filename; }
	void setDelimiter(char ch) { m_delimiter = ch; }

protected:
	virtual bool doload(const String& path);
	virtual IRow* create(uint32_t id) = 0;
	IRow* find(uint32_t id);

	bool parse(CellVector& cells, const String& line);

protected:
	typedef std::vector<IRow*>			RowVec;
	typedef std::map<uint32_t, IRow*>	RowMap;
	typedef RowMap::const_iterator		RowIterator;
	RowVec	m_rows;
	RowMap	m_maps;
	String	m_filename;
	String	m_path;
	char	m_delimiter;
};

// 基于WeakPtr的指针检测
template<typename T>
class Table : public ITable
{
public:
	typedef RefPtr<T> Ptr;

	class Iterator
	{
	public:
		Iterator(const RowIterator& itor)
			: m_itor(itor)
		{
		}

		uint32_t id()
		{
			return m_itor->first;
		}

		const T* operator -> ()
		{
			return (T*)(m_itor->second);
		}

		const T& operator * ()
		{
			return *(T*)(m_itor->second);
		}

		// prefix
		inline Iterator& operator ++ ()
		{
			++m_itor;
			return *this;
		}

		// postfix
		inline Iterator operator++(int)
		{
			Iterator other(m_itor);
			++m_itor;
			return other();
		}

		inline bool operator == (const Iterator& rhs) const
		{
			return m_itor == rhs.m_itor;
		}

		inline bool operator != (const Iterator& rhs) const
		{
			return m_itor != rhs.m_itor;
		}

	private:
		RowIterator m_itor;
	};

	Ptr get(uint32_t id)
	{
		return Ptr((T*)find(id));
	}

	const T* get(uint32_t id) const
	{
		return (T*)find(id);
	}

	const T* at(uint32_t index) const
	{
		return (T*)m_rows[index];
	}

	const T* operator [] (int index) const
	{
		return (T*)m_rows[index];
	}

	Iterator begin() const { return Iterator(m_maps.begin()); }
	Iterator end() const { return Iterator(m_maps.end()); }

protected:
	IRow* create(uint32_t rid)
	{
		return new T();
	}
};

/*
example:
struct ItemCfg : IRow
{
	int type;
};

typedef Table<ItemCfg> ItemTab;
or
class ItemTab : public Table<ItemCfg>
{
};
*/
// 固定名字 XxxCfg->XxxTab
#define AUTO_DEF_TAB(Name)			\
	typedef Table<Name##Cfg>	Name##Tab;

CUTE_NS_END