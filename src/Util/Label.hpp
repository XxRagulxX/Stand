#pragma once

#include "Util/Joaat.hpp"

#include <string>
#include <utility>

#include "Util/hashtype.hpp"
#include "Commands/Online/CommandName.hpp"

#define LOC(key) Stand::Label(key, Stand::Label::TagLiteral{})
#define LIT(text) Stand::Label(text, Stand::Label::TagLiteral{})
#define NOLABEL Stand::Label()

namespace Stand
{
#pragma pack(push, 1)
	class Label
	{
	public:
		static const Label sNoLabel;

		struct TagLiteral
		{
		};

	private:
		Stand::joaat_t hash = 0;

	public:
		std::string literal_str{};

		Label() noexcept = default;

		Label(const std::string& str, TagLiteral) noexcept
			: hash(Stand::Joaat(str)), literal_str(str)
		{
		}

		Label(std::string&& str, TagLiteral) noexcept
			: hash(Stand::Joaat(str)), literal_str(std::move(str))
		{
		}

		Label(const char* str, TagLiteral) noexcept
			: Label(std::string(str), TagLiteral{})
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
			literal_str = str;
		}

		void setLiteral(std::string&& str) noexcept
		{
			hash = Stand::Joaat(str);
			literal_str = std::move(str);
		}

		void setLocalised(hash_t hash) noexcept;

		[[nodiscard]] bool empty() const noexcept
		{
			return literal_str.empty();
		}

		void reset() noexcept
		{
			hash = 0;
			literal_str.clear();
		}

		[[nodiscard]] bool operator==(const hash_t hash) const noexcept;
		[[nodiscard]] bool operator==(const Label& b) const noexcept
		{
			return hash == b.hash && literal_str == b.literal_str;
		}

		[[nodiscard]] bool operator!=(const Label& b) const noexcept
		{
			return !operator==(b);
		}

		[[nodiscard]] bool isLiteralString(const std::string& b) const noexcept;

		[[nodiscard]] Stand::joaat_t getHash() const noexcept
		{
			return hash;
		}

		[[nodiscard]] Stand::joaat_t getLocalisationHash() const noexcept
		{
			return hash;
		}

		[[nodiscard]] const std::string& getLocalisedUtf8() const noexcept
		{
			return literal_str;
		}

		[[nodiscard]] const std::string& getEnglishUtf8() const noexcept
		{
			return literal_str;
		}

		[[nodiscard]] const std::string& getWebString() const noexcept
		{
			return literal_str;
		}

		[[nodiscard]] std::wstring getLocalisedUtf16() const;

		[[nodiscard]] bool isLiteral() const noexcept
		{
			return true;
		}

		[[nodiscard]] std::string getLiteralUtf8() const noexcept;
		[[nodiscard]] std::wstring getLiteralUtf16() const noexcept;
		[[nodiscard]] CommandName getLiteralForCommandName() const noexcept;

		[[nodiscard]] std::wstring getEnglishUtf16() const noexcept;
		[[nodiscard]] CommandName getEnglishForCommandName() const noexcept;

		void makeLiteralLocalised() noexcept;
	};
#pragma pack(pop)
}
