#pragma once
#include "API.h"
#include "SmartPtr.h"
#include <vector>
#include <map>
#include <string>
#include <sstream>

CU_NS_BEGIN

// Tab中的一行
class CU_API CellVec : public std::vector<String>
{
public:
	CellVec() :m_index(0){}
	void reset() { clear(); m_index = 0; }
	void skip(size_t count = 1) { m_index += count; }
	bool eof() const { return m_index >= size(); }
	String next() { return at(m_index++); }

	CellVec& operator>>(String& cell)	{ cell = next(); return *this; }
	CellVec& operator>>(bool& cell)		{ return convert(cell); }
	CellVec& operator>>(char& cell)		{ return convert(cell); }
	CellVec& operator>>(int8_t& cell)	{ return convert(cell); }
	CellVec& operator>>(int16_t& cell)	{ return convert(cell); }
	CellVec& operator>>(int32_t& cell)	{ return convert(cell); }
	CellVec& operator>>(int64_t& cell)	{ return convert(cell); }
	CellVec& operator>>(uint8_t& cell)	{ return convert(cell); }
	CellVec& operator>>(uint16_t& cell)	{ return convert(cell); }
	CellVec& operator>>(uint32_t& cell)	{ return convert(cell); }
	CellVec& operator>>(uint64_t& cell)	{ return convert(cell); }
	CellVec& operator>>(float& cell)	{ return convert(cell); }
	CellVec& operator>>(double& cell)	{ return convert(cell); }

	// 要求外部类型都必须能够stringstream流处理
	template<typename T>
	CellVec& operator>>(T& cell)
	{
		std::stringstream ss(next());
		ss >> cell;
		return *this;
	}

	// 数组类型,每个元素对应一个cell
	template<typename T, std::size_t N>
	CellVec& operator>>(T(&data)[N])
	{
		for (std::size_t i = 0; i < N; ++i)
			*this >> data[i];

		return *this;
	}

	// 默认以','分割
	template<typename U, typename V>
	CellVec& operator>>(std::pair<U, V>& cells)
	{
		sscanf(next().c_str(), "%d , %d", &cells.first, &cells.second);
	}

	// 逗号分割的cells
	template<class T>
	CellVec& operator>>(std::vector<T>& cells)
	{
		cells.clear();
		std::vector<std::string> fields;
		Util::split(fields, next(), ",");

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
	CellVec& operator>>(std::map<U, V>& cells)
	{
		static char buff[2][20] = { 0 };

		cells.clear();

		std::vector<std::string> fields;
		Util::split(fields, next(), "|");

		U key;
		V value;
		for (auto itor = fields.begin(); itor != fields.end(); ++itor)
		{
			std::string& field = *itor;
			Util::trim(field);
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
	void convert(const char* str, bool& data) { data = atoi(str) != 0; }
	void convert(const char* str, float& data){ data = (float)atof(str); }
	void convert(const char* str, double& data) { data = atof(str); }
	template<typename T>
	void convert(const char* str, T& data) { data = (T)atoll(str); }
	template<typename T>
	void convert(const String& str, T& data) { convert(str.c_str(), data); }
	template<typename T>
	CellVec& convert(T& data) 
	{ 
		convert(next(), data);  
		return *this; 
	}

private:
	size_t m_index;
};

// 表格中一行，ID最多支持两列
class CU_API ITabRow : public Ref
{
	static const uint32_t LOW_MASK = 0xffff;
public:
	ITabRow():m_id(0){}
	virtual ~ITabRow(){}
	virtual void parse(CellVec& cells) = 0;
	virtual bool check() { return true; }

	void setID(uint32_t id) { m_id = id; }
	void setID(uint32_t id0, uint32_t id1) { m_id = (id0 << 16) + id1; }
	uint32_t getID() const { return m_id; }
	uint32_t getHighID() const { return (m_id >> 16) & LOW_MASK; }
	uint32_t getLowID() const { return m_id & LOW_MASK; }

protected:
	uint32_t m_id;
};

/*
二维表格，最多支持两列索引,支持\t和csv格式解析
有三行注释：type，name，description
#开始作为行注释
最多支持两列ID
*/
class CU_API ITable
{
public:
	virtual ~ITable(){}
	bool load(const char* path, bool unionID = false, char delimiter = ',');

protected:
	void decode(CellVec& cells, const String& str, char delimiter);
	virtual ITabRow* create(uint32_t id) = 0;

protected:
	static uint32_t calcID(uint32_t id0, uint32_t id1) { return (id0 << 16) + id1; }
	bool m_unionID;		// 是否是联合ID，高16位标示第一ID，低16位标示第二ID
};

// 表格
template<typename T>
class Table : public ITable
{
public:
	typedef typename SharedPtr<T> Row;
	typedef std::map<uint32_t, Row> RowMap;
	typedef typename RowMap::const_iterator iterator;
	typedef std::pair<iterator, iterator>	itor_pair;
	
	Row& find(uint32_t id)
	{
		RowMap::iterator itor = m_rows.find(id);
		if (itor != m_rows.end())
			return itor->second;
		return NULL;
	}

	Row& find(uint32_t id0, uint32_t id1)
	{
		return find(id0, id1);
	}

	const T* findCfg(uint32_t id) const
	{
		RowMap::const_iterator itor = m_rows.find(id);
		if (itor != m_rows.end())
			return itor->second.get();
		return 0;
	}

	const T* findCfg(uint32_t id0, uint32_t id1) const
	{
		return findCfg(calcID(id0, id1));
	}

	/*
	用法:
	通过第一ID查找集合
	----id1---id2----
	0-----1----1------
	1-----1----2------
	2-----2----1------
	3-----2----2------
	findRange(1)-->返回0,1
	findRange(2)-->返回2,3
	itor_pair range_itor;
	for(auto itor = range_itor.first; itor != range_itor.end; ++itor){}
	*/
	itor_pair findRange(uint16_t firstID)
	{
		uint32_t startID = firstID << 16;
		uint32_t endID = startID & 0xffff;
		itor_pair itor;
		itor.first = m_rows.lower_bound(startID);
		itor.second = m_rows.upper_bound(endID);
		return itor;
	}

	itor_pair findRange(uint32_t startID, uint32_t endID)
	{
		itor_pair itor;
		itor.first = m_rows.lower_bound(startID);
		itor.second = m_rows.upper_bound(endID);
		return itor;
	}

	iterator begin() const { return m_rows.cbegin(); }
	iterator end() const { return m_rows.cend(); }

	ITabRow* create(uint32_t id)
	{
		RowMap::iterator itor = m_rows.find(id);
		if (itor != m_rows.end())
			return itor->second.get();
		ITabRow* row = new T();
		row->setID(id);
		m_rows[id] = row;
		return row;
	}
private:
	RowMap m_rows;
};

// 固定名字 XxxCfg->XxxTab XxxRow
#define AUTO_DEF_TAB(Name)\
	typedef Table<Name##Cfg>	Name##Tab;	\
	typedef Name##Tab::Row		Name##Row;

CU_NS_END
