#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

// Regex:RegularExpression


/// This class contains enumerations and static
/// utility functions for dealing with Unicode characters
/// and their properties.
///
/// For more information on Unicode, see <http://www.unicode.org>.
///
/// The implementation is based on the Unicode support
/// functions in PCRE.
class CUTE_CORE_API Unicode
{
public:
	/// Unicode 5.0 character categories.
	enum CharacterCategory
	{
		UCP_OTHER,
		UCP_LETTER,
		UCP_MARK,
		UCP_NUMBER,
		UCP_PUNCTUATION,
		UCP_SYMBOL,
		UCP_SEPARATOR
	};

	/// Unicode 5.0 character types.
	enum CharacterType
	{
		UCP_CONTROL,
		UCP_FORMAT,
		UCP_UNASSIGNED,
		UCP_PRIVATE_USE,
		UCP_SURROGATE,
		UCP_LOWER_CASE_LETTER,
		UCP_MODIFIER_LETTER,
		UCP_OTHER_LETTER,
		UCP_TITLE_CASE_LETTER,
		UCP_UPPER_CASE_LETTER,
		UCP_SPACING_MARK,
		UCP_ENCLOSING_MARK,
		UCP_NON_SPACING_MARK,
		UCP_DECIMAL_NUMBER,
		UCP_LETTER_NUMBER,
		UCP_OTHER_NUMBER,
		UCP_CONNECTOR_PUNCTUATION,
		UCP_DASH_PUNCTUATION,
		UCP_CLOSE_PUNCTUATION,
		UCP_FINAL_PUNCTUATION,
		UCP_INITIAL_PUNCTUATION,
		UCP_OTHER_PUNCTUATION,
		UCP_OPEN_PUNCTUATION,
		UCP_CURRENCY_SYMBOL,
		UCP_MODIFIER_SYMBOL,
		UCP_MATHEMATICAL_SYMBOL,
		UCP_OTHER_SYMBOL,
		UCP_LINE_SEPARATOR,
		UCP_PARAGRAPH_SEPARATOR,
		UCP_SPACE_SEPARATOR
	};

	/// Unicode 5.0 scripts.
	enum Script
	{
		UCP_ARABIC,
		UCP_ARMENIAN,
		UCP_BENGALI,
		UCP_BOPOMOFO,
		UCP_BRAILLE,
		UCP_BUGINESE,
		UCP_BUHID,
		UCP_CANADIAN_ABORIGINAL,
		UCP_CHEROKEE,
		UCP_COMMON,
		UCP_COPTIC,
		UCP_CYPRIOT,
		UCP_CYRILLIC,
		UCP_DESERET,
		UCP_DEVANAGARI,
		UCP_ETHIOPIC,
		UCP_GEORGIAN,
		UCP_GLAGOLITIC,
		UCP_GOTHIC,
		UCP_GREEK,
		UCP_GUJARATI,
		UCP_GURMUKHI,
		UCP_HAN,
		UCP_HANGUL,
		UCP_HANUNOO,
		UCP_HEBREW,
		UCP_HIRAGANA,
		UCP_INHERITED,
		UCP_KANNADA,
		UCP_KATAKANA,
		UCP_KHAROSHTHI,
		UCP_KHMER,
		UCP_LAO,
		UCP_LATIN,
		UCP_LIMBU,
		UCP_LINEAR_B,
		UCP_MALAYALAM,
		UCP_MONGOLIAN,
		UCP_MYANMAR,
		UCP_NEW_TAI_LUE,
		UCP_OGHAM,
		UCP_OLD_ITALIC,
		UCP_OLD_PERSIAN,
		UCP_ORIYA,
		UCP_OSMANYA,
		UCP_RUNIC,
		UCP_SHAVIAN,
		UCP_SINHALA,
		UCP_SYLOTI_NAGRI,
		UCP_SYRIAC,
		UCP_TAGALOG,
		UCP_TAGBANWA,
		UCP_TAI_LE,
		UCP_TAMIL,
		UCP_TELUGU,
		UCP_THAANA,
		UCP_THAI,
		UCP_TIBETAN,
		UCP_TIFINAGH,
		UCP_UGARITIC,
		UCP_YI,
		UCP_BALINESE,
		UCP_CUNEIFORM,
		UCP_NKO,
		UCP_PHAGS_PA,
		UCP_PHOENICIAN,
		UCP_CARIAN,
		UCP_CHAM,
		UCP_KAYAH_LI,
		UCP_LEPCHA,
		UCP_LYCIAN,
		UCP_LYDIAN,
		UCP_OL_CHIKI,
		UCP_REJANG,
		UCP_SAURASHTRA,
		UCP_SUNDANESE,
		UCP_VAI
	};

	enum
	{
		UCP_MAX_CODEPOINT = 0x10FFFF
	};

	/// This structure holds the character properties
	/// of an Unicode character.
	struct CharacterProperties
	{
		CharacterCategory category;
		CharacterType     type;
		Script            script;
	};

	/// Return the Unicode character properties for the
	/// character with the given Unicode value.
	static void properties(int ch, CharacterProperties& props);

	static bool isSpace(int ch);
	static bool isDigit(int ch);
	static bool isPunct(int ch);
	static bool isAlpha(int ch);
	static bool isLower(int ch);
	static bool isUpper(int ch);

	static int toLower(int ch);
	static int toUpper(int ch);
};

//
// inlines
//
inline bool Unicode::isSpace(int ch)
{
	CharacterProperties props;
	properties(ch, props);
	return props.category == UCP_SEPARATOR;
}

inline bool Unicode::isDigit(int ch)
{
	CharacterProperties props;
	properties(ch, props);
	return props.category == UCP_NUMBER;
}

inline bool Unicode::isPunct(int ch)
{
	CharacterProperties props;
	properties(ch, props);
	return props.category == UCP_PUNCTUATION;
}

inline bool Unicode::isAlpha(int ch)
{
	CharacterProperties props;
	properties(ch, props);
	return props.category == UCP_LETTER;
}

inline bool Unicode::isLower(int ch)
{
	CharacterProperties props;
	properties(ch, props);
	return props.category == UCP_LETTER && props.type == UCP_LOWER_CASE_LETTER;
}

inline bool Unicode::isUpper(int ch)
{
	CharacterProperties props;
	properties(ch, props);
	return props.category == UCP_LETTER && props.type == UCP_UPPER_CASE_LETTER;
}

CUTE_NS_END
