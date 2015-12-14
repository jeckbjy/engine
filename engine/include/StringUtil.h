#pragma once
#include "StringPiece.h"

CU_NS_BEGIN

CU_API void trim(String& str);
CU_API void trimLeft(String& str);
CU_API void trimRight(String& str);
CU_API void toUpper(String& str);
CU_API void toLower(String& str);
CU_API void startWith(const StringPiece& str, const StringPiece& pattern, bool care = true);
CU_API void endWith(const StringPiece& str, const StringPiece& pattern, bool care = true);
CU_API void replace(String& str, const StringPiece& old_word, const StringPiece& new_word);
CU_API void split(StringList& tokens, const StringPiece& str, const StringPiece& delimiter = "|", bool compress = true);
CU_API void escape(String& out, const StringPiece& str);
CU_API void unescape(String& out, const StringPiece& str, bool strict = true);
CU_API StringPiece getExtension(const StringPiece& str);
CU_API WString toWString(const StringPiece& str);

template<typename T>
inline T toInteger(StringPiece str) { return (T)atoi(str.data()); }
template<typename T>
inline T toFloat(StringPiece str) { return (T)atof(str.data()); }

CU_NS_END
