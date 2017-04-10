#include "Test.h"
#include "Cute/Ref.h"
#include "Cute/RefPtr.h"
#include "Cute/DoubleArrayTrie.h"
#include "Cute/Object.h"

using namespace Cute;

void Test::run()
{
	//testRefPtr();
	testDoubleArrayTrie();
}

void Test::testRefPtr()
{
	class Foo : public Ref<Foo>
	{

	};

	typedef RefPtr<Foo> Ptr;
	Foo* foo = new Foo();
	Ptr ptr(foo);
	//if (ptr == foo)
	//{

	//}
}

void Test::testDoubleArrayTrie()
{
	DoubleArrayTrie dat;
	const char* words[] = 
	{
		"�й�����",
		"�й���",
		"�й�",
		"���в���",
		"����",
		"asdf",
	};

	dat.build(CUTE_ARRAY_SIZE(words), words);

	int result = dat.match(DoubleArrayTrie::MATCH_PREFIX_MAX, "asdf");
}