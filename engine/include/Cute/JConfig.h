#pragma once
#include "Cute/Foundation.h"
#include "Cute/Variant.h"

CUTE_NS_BEGIN

class CUTE_CORE_API JConfigReader
{
public:
	typedef const char* Location;
	void parse(Variant& root, Location beg, Location end);

private:
	void readObject(Variant& object, bool startWithBrace);
	void readArray(Variant& arrays);
	void readValue(Variant& value);
	void readString(Variant& value);
	bool readNumber(Variant& value);
	bool readIP(Variant& value);
	bool readDateTime(Variant& value);
	void readDynamic(Variant& value);

	void readKey(String& key);
	char readChar();
	char readEscape(char ch);
	bool isSeparatorKey(char ch);
	bool match(const char* pattern, int length);

	void skipSpace();
	void skipComment();
	void skipSpaceAndComment();

	void error(const String& info);

private:
	Location m_beg;
	Location m_end;
	Location m_cur;
	int		 m_line;
};

/*
��������json����û����ô�ϸ�,like libconfig
1:root��Ҫ����뺬{},���Ǳ���ƥ��
2:string��Ҫ����뺬""������������ı��뺬������,���Ǵ����֣��Ƿ���,�Ҳ�����true��false����null
3:object�ָ�������ǣ�����=,������Ϊ�ָ���
4:��ǿ��Ҫ�����һ���ָ���,�ɺ����߲�������
5:����ע��:ʹ��#����//,������λ�ã�����ע��:ͬc++�﷨
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
