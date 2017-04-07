#pragma once
#include "Cute/Foundation.h"
#include "Cute/Variant.h"

CUTE_NS_BEGIN

/*
规则：类似json但是没有那么严格,like libconfig
1:root不要求必须含{},但是必须匹配
2:string不要求必须含""，但是有歧义的必须含有引号,即非纯数字，非符号,且不能是true，false，和null
3:obj分割符可以是：或者=,换行作为分隔符
4:不强制要求最后一个分隔符,可含或者不含逗号
5:注释使用单行#开始
*/

/*
example:
#this is an example!!
version		= "1.1.1"
channel		= china
whitelist	= [ 192.168.1.1, 192.168.1.2 ]
game:[
  {id=1, name=asdf, addr1=192.168.1.1, port1=92 },
]

world:[
  {id=1, name=aaa,  addr1=192.168.1.1, port1=11},
]

db:[
  {id=1, addr1=192.168.1.1, port1=1},
]
*/
class CUTE_CORE_API JConfig
{
public:
	static Variant load(const String& path);
	static Variant parse(const String& data);
};


CUTE_NS_END
