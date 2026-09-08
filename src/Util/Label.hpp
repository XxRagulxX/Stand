#pragma once
#include "Util/Joaat.hpp"

#include <string>
#include <utility>

// This project has no equivalent of Stand's own translation database
// (Localization/lang_en.cpp and friends) - a real Stand Label can be
// hash-only (LOC), resolved later against that database. Without one,
// there's nothing for a hash-only Label to resolve to, so this Label
// always keeps the literal text alongside its hash: LOC(key) stores key
// itself as literal_str (displays as the raw translation key, e.g. "SLF",
// until/unless a real English text table is ported) rather than being
// unrenderable. LIT(text) is the one to use for actually-correct display
// text ported from Stand's own English lang_en.cpp values by hand.
#define LOC(key) Stand::Label(key, Stand::Label::TagLiteral{})
#define LIT(text) Stand::Label(text, Stand::Label::TagLiteral{})
#define NOLABEL Stand::Label()

namespace Stand
{
	class Label
	{
	public:
		struct TagLiteral
		{
		};

	private:
		Stand::joaat_t hash = 0;

	public:
		std::string literal_str{};

		Label() noexcept = default;

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

		Label(const Label&) noexcept = default;
		Label(Label&&) noexcept = default;
		Label& operator=(const Label&) noexcept = default;
		Label& operator=(Label&&) noexcept = default;

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

		[[nodiscard]] bool empty() const noexcept
		{
			return literal_str.empty();
		}

		void reset() noexcept
		{
			hash = 0;
			literal_str.clear();
		}

		[[nodiscard]] bool operator==(const Label& b) const noexcept
		{
			return hash == b.hash && literal_str == b.literal_str;
		}

		[[nodiscard]] bool operator!=(const Label& b) const noexcept
		{
			return !operator==(b);
		}

		[[nodiscard]] Stand::joaat_t getHash() const noexcept
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
	};
}
