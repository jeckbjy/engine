#include "Table.h"
#include "File.h"

CU_NS_BEGIN

#define COMMENT_COUNT 3

bool ITable::load(const char* path, bool unionID /* = false */, char delimiter /* = ' */)
{
	m_unionID = unionID;
	File file;
	if (!file.open(path))
		return false;

	// ��������ע��,type,
	for (size_t i = 0; i < COMMENT_COUNT; ++i)
	{
		if (!file.skipLine())
			return true;
	}

	std::size_t count = COMMENT_COUNT;
	std::string line;
	CellVec cells;
	while (!file.eof())
	{
		count++;
		line.clear();
		file.readLine(line);
		if (line.empty() || line[0] == '#')
			continue;
		decode(cells, line, delimiter);

		// ������ͷ
		uint32_t id,id0, id1;
		if (!unionID)
		{
			cells >> id;
			id0 = id;
			id1 = 0;
		}
		else
		{
			cells >> id0;
			cells >> id1;
			id = calcID(id0, id1);
		}

		ITabRow* row = create(id);
		if (!row)
			return false;
		row->parse(cells);
		if (!row->check())
		{
			std::stringstream ss;
			ss << "check error:" << path << ":" << count << ":" << line;
			throw std::runtime_error(ss.str());
		}

		cells.reset();
	}

	return true;
}

void ITable::decode(CellVec& cells, const String& str, char delimiter)
{
	if (str.empty())
		return;

	std::stringstream cell;

	// ���Խ�β���з���
	size_t endPos = str.size() - 1;
	if (str[endPos] == '\r')
		endPos--;
	else if (str[endPos] == '\n')
	{
		endPos--;
		if (endPos == 0)
			return;
		if (str[endPos] == '\r')
			endPos--;
	}

	bool startquote = false;	// �Ƿ������ʼ����
	size_t start = 0;			// ��ʼλ��
	char ch;					// ��ǰ�ַ�
	for (size_t i = 0; i <= endPos; ++i)
	{
		ch = str[i];
		if (ch == delimiter)
		{
			if (startquote)
			{
				cell << ch;
				continue;
			}
			// �ҵ�һ��
			cells.push_back(cell.str());
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
				if (j <= endPos && str[j] == '"')
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
				// ��β�ˣ���߱�����һ���ָ�����������д���
				for (unsigned int x = i + 1; x < endPos; x++)
				{
					if (str[x] == delimiter)
					{
						i = x - 1;
						break;
					}
				}
			}
		}
		else
		{// ������ͨ�ַ�
			cell << ch;
		}
	}

	// �����ˣ��������һ��
	cells.push_back(cell.str());
}


CU_NS_END