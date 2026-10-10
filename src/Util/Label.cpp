#include "Util/Label.hpp"

#include "Util/StringUtils.hpp"

namespace Stand
{
	const Label Label::sNoLabel = NOLABEL;

	Label::Label(const std::wstring& str, TagLiteral)
		: Label(StringUtils::utf16_to_utf8(str), TagLiteral{})
	{
	}

	Label Label::combineWithSpace(const Label& primary, const Label& secondary)
	{
		std::string str = primary.getLocalisedUtf8();
		str.push_back(' ');
		str.append(secondary.getLocalisedUtf8());
		return LIT(std::move(str));
	}

	Label Label::combineWithBrackets(const Label& primary, const Label& secondary)
	{
		std::string str = primary.getLocalisedUtf8();
		str.append(" (");
		str.append(secondary.getLocalisedUtf8());
		str.push_back(')');
		return LIT(std::move(str));
	}

	std::wstring Label::getLocalisedUtf16() const
	{
		return StringUtils::utf8_to_utf16(literal_str);
	}

	bool Label::isLiteralString(const std::string& b) const noexcept
	{
		return literal_str == b;
	}

	std::string Label::getLiteralUtf8() const noexcept
	{
		return literal_str;
	}

	std::wstring Label::getLiteralUtf16() const noexcept
	{
		return StringUtils::utf8_to_utf16(literal_str);
	}

	std::wstring Label::getEnglishUtf16() const noexcept
	{
		return StringUtils::utf8_to_utf16(literal_str);
	}

	CommandName Label::getLiteralForCommandName() const noexcept
	{
#if COMPACT_COMMAND_NAMES
		return getLiteralUtf8();
#else
		return getLiteralUtf16();
#endif
	}

	CommandName Label::getEnglishForCommandName() const noexcept
	{
#if COMPACT_COMMAND_NAMES
		return getEnglishUtf8();
#else
		return getEnglishUtf16();
#endif
	}

	void Label::makeLiteralLocalised() noexcept
	{
	}
}
