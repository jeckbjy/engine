#include "Cute/Object.h"

CUTE_NS_BEGIN

Object::Object()
{
}

Object::~Object()
{
}

String Object::toString() const
{
	return typeid(Object).name();
}

CUTE_NS_END