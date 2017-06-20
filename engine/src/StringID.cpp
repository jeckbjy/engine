#include "Cute/StringID.h"
#include "Cute/Mutex.h"

CUTE_NS_BEGIN

// todo: HASH_TABLE_SIZE dynamic change?
class StringIDManager
{
public:
	static const int HASH_TABLE_SIZE = 4096;

	struct Data
	{
		char*	name;
		size_t	size;
		Data*	next1;	// for hash table
		Data*	next2;	// for chunk list
	};

	static StringIDManager& get()
	{
		static StringIDManager inst;
		return inst;
	}

	StringIDManager()
		: m_head(NULL)
	{
		memset(m_table, 0, sizeof(m_table));
	}

	~StringIDManager()
	{
		// free all
		memset(m_table, 0, sizeof(m_table));

		Data* node = m_head;
		while (node != NULL)
		{
			Data* temp = node;
			node = node->next2;

			// free node
			free(temp->name);
			delete temp;
		}
	}

	Data* create(const char* name)
	{
		return this->create(name, strlen(name));
	}

	Data* create(const char* name, size_t size)
	{
		cute_assert(size > 0);
		// calc hash
		size_t hash = 0;
		for (size_t i = 0; i < size; ++i)
			hash = hash * 101 + name[i];

		hash = hash / (HASH_TABLE_SIZE - 1);

		// find data
		Data* node = m_table[hash];
		while (node != NULL)
		{
			if (size == node->size && strcmp(name, node->name) == 0)
				return node;

			node = node->next1;
		}

		// create if not exist
		Mutex::ScopedLock guard(m_mutex);
		// todo:memory manager!!
		Data* newNode = new Data();
		newNode->name = (char*)malloc(size + 1);
		newNode->size = size;
		memcpy(newNode->name, name, size);
		newNode->name[size] = 0;

		// link node
		newNode->next1 = m_table[hash];
		m_table[hash] = newNode;

		newNode->next2 = m_head;
		m_head = newNode;

		return newNode;
	}

private:
	Data*	m_table[HASH_TABLE_SIZE];
	Data*	m_head;
	Mutex	m_mutex;
};

StringID::StringID()
	: m_data(NULL)
{
}

StringID::StringID(const char* name)
{
	m_data = StringIDManager::get().create(name);
}

StringID::StringID(const char* name, size_t size)
{
	m_data = StringIDManager::get().create(name, size);
}

StringID::StringID(const String& name)
{
	m_data = StringIDManager::get().create(name.c_str(), name.size());
}

StringID::~StringID()
{
}

const char* StringID::str() const
{
	if (m_data != NULL)
	{
		StringIDManager::Data* data = (StringIDManager::Data*)m_data;
		return data->name;
	}

	return NULL;
}

size_t StringID::size() const
{
	if (m_data != NULL)
	{
		StringIDManager::Data* data = (StringIDManager::Data*)m_data;
		return data->size;
	}

	return 0;
}

CUTE_NS_END
