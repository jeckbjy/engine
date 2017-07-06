//! Config
#include "Cute/Table.h"
#include "Cute/Paths.h"
#include "Cute/Files.h"
#include "Cute/FileStream.h"

CUTE_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CellVector::CellVector()
	: m_index(0)
{
}

void CellVector::reset()
{
	m_cells.clear();
	m_index = 0;
}

bool CellVector::eof() const
{
	return m_index >= m_cells.size();
}

void CellVector::push(const String& cell)
{
	m_cells.push_back(cell);
}

const String& CellVector::next()
{
	if (m_index > m_cells.size())
		throw IndexOutOfBoundsException("Row Cell");

	return m_cells[m_index++];
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
ITable::ITable()
	: m_delimiter(',')
{
}

ITable::~ITable()
{
	// release
	for (size_t i = 0; i < m_rows.size(); ++i)
	{
		m_rows[i]->decRef();
	}

	m_rows.clear();
	m_maps.clear();
}

bool ITable::load(const String& path)
{
	if (!Files::exists(path))
		return false;

    m_filename = Paths::getFileName(path);
	m_path = path;

	return doload(path);
}

bool ITable::reload()
{
	return doload(m_path);
}

bool ITable::doload(const String& path)
{
	FileStream file;
	if (!file.open(path))
		return false;

	// cache and release later
	RowMap cellMap;
	std::swap(cellMap, m_maps);
	m_rows.clear();

	for (size_t i = 0; i < COMMENT_COUNT; ++i)
	{
		if (!file.skipLine())
			return true;
	}

	int64_t duplicateID = -1;

	String line;
	CellVector cells;
	while (!file.eof())
	{
		line.clear();
		file.readLine(line);
		if (line.empty() || line[0] == '#')
			continue;

		bool ret = parse(cells, line);
		if (!ret)
			continue;

		uint32_t rid;
		cells >> rid;

		if (m_maps.find(rid) != m_maps.end())
		{
			// just record the first bad id!
			if (duplicateID != -1)
				duplicateID = rid;
			continue;
		}

		IRow* row = NULL;
		if (!cellMap.empty())
		{
			RowMap::iterator itor = cellMap.find(rid);
			if (itor != cellMap.end())
			{
				row = itor->second;
				cellMap.erase(itor);
			}
		}

		if (row == NULL)
		{
			row = create(rid);
			row->setID(rid);
			row->incRef();
		}

		row->decode(cells);
		m_maps[rid] = row;
		m_rows.push_back(row);
	}

	for (RowMap::iterator itor = cellMap.begin(); itor != cellMap.end(); ++itor)
	{
		IRow* row = itor->second;
		row->decRef();
	}

	// format??
	if (duplicateID != -1)
		throw LogicException("duplicateID ", path);

	return true;
}

bool ITable::parse(CellVector& tokens, const String& line)
{
	char delimiter = m_delimiter;
	if (line.empty())
		return false;

	std::stringstream cell;

	// 忽略结尾换行符号
	size_t endPos = line.size() - 1;
	if (line[endPos] == '\r')
		endPos--;
	else if (line[endPos] == '\n')
	{
		endPos--;
		if (endPos == 0)
			return false;
		if (line[endPos] == '\r')
			endPos--;
	}

	bool startquote = false;	// 是否包含起始引号
//	size_t start = 0;			// 起始位置
	char ch;					// 当前字符
	for (size_t i = 0; i <= endPos; ++i)
	{
		ch = line[i];
		if (ch == delimiter)
		{
			if (startquote)
			{
				cell << ch;
				continue;
			}
			// 找到一个
			tokens.push(cell.str());
			cell.str("");
			continue;
		}
		else if (ch == '"')
		{
			// If the character is a quote then we need to note this and use that to ignore commas
			// added logic to ignore whitespace before and after the whitespace
			if (startquote)
			{
				size_t j = i + 1;
				if (j <= endPos && line[j] == '"')
				{
					cell << '"';
					++i;
					continue;
				}
			}

			startquote = !startquote;
			//find , and move i to it
			if (!startquote)
			{
				// 结尾了，后边必须是一个分隔符，否则就有错误
				for (size_t x = i + 1; x < endPos; x++)
				{
					if (line[x] == delimiter)
					{
						i = x - 1;
						break;
					}
				}
			}
		}
		else
		{// 输入普通字符
			cell << ch;
		}
	}

	// 结束了，加入最后一个
	tokens.push(cell.str());

	return true;
}

IRow* ITable::find(uint32_t id)
{
	RowMap::iterator itor = m_maps.find(id);
	if (itor != m_maps.end())
		return itor->second;

	return NULL;
}

CUTE_NS_END
