#pragma once
#include "Cute/Foundation.h"
#include "Cute/Variant.h"

CUTE_NS_BEGIN

/*
��������json����û����ô�ϸ�,like libconfig
1:root��Ҫ����뺬{},���Ǳ���ƥ��
2:string��Ҫ����뺬""������������ı��뺬������,���Ǵ����֣��Ƿ���,�Ҳ�����true��false����null
3:obj�ָ�������ǣ�����=,������Ϊ�ָ���
4:��ǿ��Ҫ�����һ���ָ���,�ɺ����߲�������
5:ע��ʹ�õ���#��ʼ
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
