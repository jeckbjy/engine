#pragma once
#include "Platform.h"

CU_NS_BEGIN

/**
* Finds the first occurrence of needle in haystack. The algorithm is on
* average faster than O(haystack.size() * needle.size()) but not as fast
* as Boyer-Moore. On the upside, it does not do any upfront
* preprocessing and does not allocate memory.
* Ҫ��T����ʵ��size(),begin(),end(),��[]���������һ������ṩ
*/
template<class T, class Cmp = std::equal_to<typename T::value_type> >
size_t quick_find(const T& haystack, const T& needle, size_t off = 0, Cmp eq = Cmp())
{
	const size_t nsize = needle.size();
	if (haystack.size() - off < nsize)
		return std::string::npos;
	if (!nsize)
		return 0;
	size_t const nsize_1 = nsize - 1;
	auto const &lastNeedle = needle[nsize_1];
	// Boyer-Moore skip value for the last char in the needle. Zero is
	// not a valid value; skip will be computed the first time it's
	// needed.
	size_t skip = 0;
	auto i = haystack.begin() + off;
	auto iEnd = haystack.end() - nsize_1;
	while (i < iEnd)
	{
		// �ж����һ���ַ��Ƿ����
		// Boyer-Moore: match the last element in the needle
		while (!eq(i[nsize_1], lastNeedle)) {
			if (++i == iEnd) {
				// not found
				return std::string::npos;
			}
		}
		// ���һ���ַ���ȣ����ǰ���ƥ��
		for (size_t j = 0;;)
		{
			// ��ƥ����������ƫ��������ĩ�ַ���ƥ��
			if (!eq(i[j], needle[j]))
			{
				// �ӳٹ���skip
				// Not found, we can skip Compute the skip value lazily
				if (skip == 0)
				{
					skip = 1;
					while (skip <= nsize_1 && !eq(needle[nsize_1 - skip], lastNeedle))
						++skip;
				}
				i += skip;
				break;
			}
			// Check if done searching:�ж��Ƿ��ҵ���
			if (++j == nsize)
				return i - haystack.begin();
		}
	}

	return std::string::npos;
}

template<class T, class Cmp = std::equal_to<typename T::value_type> >
size_t quick_rfind(const T& haystack, const T& needle, size_t off = 0, Cmp eq = Cmp())
{
	return 0;
	//if (haystack.size() < needle.size() + off)
	//	return std::string::npos;
	//if (needle.size() == 0)
	//	return 0;

	//auto const& needChecked = needle[0];
	//size_t skip = 0;	//will be lazy init
	//auto iter = haystack.begin() + (haystack.size() - needle.size() - off);
	//while (iter != haystack.begin())
	//{
	//	// ����У��
	//	while (!eq(iter, needChecked))
	//	{
	//		if (--iter == haystack.begin())
	//			break;
	//	}
	//	// ���ƥ��
	//	for ()
	//	{
	//	}
	//}
	//const size_t nsize = needle.size();
	//if (haystack.size() < nsize)
	//	return std::string::npos;
	//if (!nsize)
	//	return 0;
	//auto const checked = needle[0];

	//size_t skip = 0;
	//auto iter = haystack.begin() + haystack.size() - needle.size();
	//while (iter != haystack.begin())

	//const size_t nsize_1 = nsize - 1;
	//const char lastNeedle = needle[nsize_1];
	//// Boyer-Moore skip value for the last char in the needle. Zero is
	//// not a valid value; skip will be computed the first time it's
	//// needed.
	//size_t skip = 0;
	//auto i = haystack.begin();
	//auto iEnd = haystack.end() - nsize_1;
	//while (i < iEnd)
	//{
	//	// �ж����һ���ַ��Ƿ����
	//	// Boyer-Moore: match the last element in the needle
	//	while (!eq(i[nsize_1], lastNeedle)) {
	//		if (++i == iEnd) {
	//			// not found
	//			return std::string::npos;
	//		}
	//	}
	//	// ���һ���ַ���ȣ����ǰ���ƥ��
	//	for (size_t j = 0;;)
	//	{
	//		// ��ƥ����������ƫ��������ĩ�ַ���ƥ��
	//		if (!eq(i[j], needle[j]))
	//		{
	//			// �ӳٹ���skip
	//			// Not found, we can skip Compute the skip value lazily
	//			if (skip == 0)
	//			{
	//				skip = 1;
	//				while (skip <= nsize_1 && !eq(needle[nsize_1 - skip], lastNeedle))
	//					++skip;
	//			}
	//			i += skip;
	//			break;
	//		}
	//		// Check if done searching:�ж��Ƿ��ҵ���
	//		if (++j == nsize)
	//			return i - haystack.begin();
	//	}
	//}

	//return std::string::npos;
}

CU_NS_END
