#pragma once
#include "Util/Joaat.hpp"
#include "Util/atStringHash.hpp"
#include "Commands/Online/CommandName.hpp"

#include <string>
#include <utility>

#define LOC(key) Stand::Label(ATSTRINGHASH(key), Stand::Label::TagLocalised{})
#define LIT(text) Stand::Label(text, Stand::Label::TagLiteral{})
#define NOLABEL Stand::Label()

namespace Stand
{
	class Label
	{
	public:
		struct TagLiteral {};
		struct TagLocalised {};

	private:
		Stand::joaat_t hash = 0;
		hash_t loc_hash = 0;

	public:
		std::string literal_str{};

		static const Label sNoLabel;

		Label() noexcept = default;

		Label(hash_t h, TagLocalised) noexcept :
		    loc_hash(h)
		{
		}

		Label(const std::string& str, TagLiteral) noexcept :
		    hash(Stand::Joaat(str)),
		    literal_str(str)
		{
		}

		Label(std::string&& str, TagLiteral) noexcept :
		    hash(Stand::Joaat(str)),
		    literal_str(std::move(str))
		{
		}

		Label(const char* str, TagLiteral) noexcept :
		    Label(std::string(str), TagLiteral{})
		{
		}

		Label(const std::wstring& str, TagLiteral);

		Label(const Label&) noexcept = default;
		Label(Label&&) noexcept = default;
		Label& operator=(const Label&) noexcept = default;
		Label& operator=(Label&&) noexcept = default;

		[[nodiscard]] static Label combineWithSpace(const Label& primary, const Label& secondary);
		[[nodiscard]] static Label combineWithBrackets(const Label& primary, const Label& secondary);

		void setLiteral(const std::string& str) noexcept
		{
			hash = Stand::Joaat(str);
			loc_hash = 0;
			literal_str = str;
		}

		void setLiteral(std::string&& str) noexcept
		{
			hash = Stand::Joaat(str);
			loc_hash = 0;
			literal_str = std::move(str);
		}

		[[nodiscard]] bool empty() const noexcept
		{
			return literal_str.empty() && loc_hash == 0;
		}

		[[nodiscard]] bool isLiteral() const noexcept
		{
			return loc_hash == 0;
		}

		void reset() noexcept
		{
			hash = 0;
			loc_hash = 0;
			literal_str.clear();
		}

		[[nodiscard]] bool operator==(const Label& b) const noexcept
		{
			return hash == b.hash && loc_hash == b.loc_hash && literal_str == b.literal_str;
		}

		[[nodiscard]] bool operator!=(const Label& b) const noexcept
		{
			return !operator==(b);
		}

		[[nodiscard]] Stand::joaat_t getHash() const noexcept
		{
			return hash;
		}

		[[nodiscard]] hash_t getLocalisationHash() const noexcept
		{
			return loc_hash;
		}

		[[nodiscard]] std::string getLocalisedUtf8() const noexcept;
		[[nodiscard]] std::string getEnglishUtf8() const noexcept;
		[[nodiscard]] std::string getWebString() const noexcept;

		[[nodiscard]] std::wstring getLocalisedUtf16() const;

		[[nodiscard]] bool isLiteralString(const std::string& b) const noexcept;

		[[nodiscard]] std::string getLiteralUtf8() const noexcept;
		[[nodiscard]] std::wstring getLiteralUtf16() const noexcept;
		[[nodiscard]] std::wstring getEnglishUtf16() const noexcept;

		[[nodiscard]] CommandName getLiteralForCommandName() const noexcept;
		[[nodiscard]] CommandName getEnglishForCommandName() const noexcept;

		void makeLiteralLocalised() noexcept;
	};
}
