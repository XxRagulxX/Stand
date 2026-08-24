#pragma once

#include <string>

#include <soup/compiletime.hpp>

#include "atStringHash.hpp"
#include "CommandName.hpp"

#define LOC(key) Label(ATSTRINGHASH(key), Label::TagLocalised{})
#define LOC_RT(...) Label(__VA_ARGS__, Label::TagLocalised{})
#define LIT(...) Label(__VA_ARGS__, Label::TagLiteral{})
#define LIT_OBF(x) Label(x, Label::TagLiteral{})
#define NOLABEL Label()
#if STAND_DEBUG
#define PHSTR(str) "[PH] " str
#define PH(str) LIT(PHSTR(str))
#endif

namespace Stand
{
#pragma pack(push, 1)
	class Label
	{
	public:
		static const Label sNoLabel;

		struct TagLocalised {};
		struct TagLiteral {};

	private:
		hash_t hash = 0;
	public:
		std::string literal_str{};

		constexpr Label() noexcept = default;

		constexpr Label(hash_t hash, TagLocalised) noexcept
			: hash(hash)
		{
		}

		Label(const std::string& hash, TagLocalised) noexcept
			: hash(rage::atStringHash(hash))
		{
		}

		constexpr Label(const std::string& str, TagLiteral) noexcept
			: literal_str(str)
		{
		}

		constexpr Label(std::string&& str, TagLiteral) noexcept
			: literal_str(std::move(str))
		{
		}

		constexpr Label(size_t n, char c, TagLiteral) noexcept
			: literal_str(std::string(n, c))
		{
		}

		Label(const std::wstring& b, TagLiteral);

		constexpr Label(Label&& b) noexcept
			: hash(b.hash), literal_str(std::move(b.literal_str))
		{
		}

		constexpr Label(const Label& b) noexcept
			: hash(b.hash), literal_str(b.literal_str)
		{
		}

		[[nodiscard]] static Label combineWithSpace(const Label& primary, const Label& secondary);
		[[nodiscard]] static Label combineWithBrackets(const Label& primary, const Label& secondary);

		void operator=(const char* b) noexcept = delete;
		void operator=(const std::string& b) noexcept = delete;
		void operator=(const Label& b) noexcept;
		void operator=(Label&& b) noexcept;

		void setLiteral(const std::string& str) noexcept;
		void setLiteral(std::string&& str) noexcept;
		void setLocalised(hash_t hash) noexcept;

		[[nodiscard]] bool isLiteral() const noexcept;
		[[nodiscard]] bool empty() const noexcept;
		void reset() noexcept;
		[[nodiscard]] bool operator==(const hash_t hash) const noexcept;
		[[nodiscard]] bool operator==(const char* b) const noexcept = delete;
		[[nodiscard]] bool operator==(const std::string& b) const noexcept = delete;
		[[nodiscard]] bool operator==(const Label& b) const noexcept;
		[[nodiscard]] bool operator!=(const char* b) const noexcept = delete;
		[[nodiscard]] bool operator!=(const std::string& b) const noexcept = delete;
		[[nodiscard]] bool operator!=(const Label& b) const noexcept;

		[[nodiscard]] bool isLiteralString(const std::string& b) const noexcept;

		[[nodiscard]] uint64_t getHash() const noexcept;
		[[nodiscard]] constexpr hash_t getLocalisationHash() const noexcept { return hash; }

		[[nodiscard]] std::string getWebString() const noexcept;

		[[nodiscard]] std::string getLiteralUtf8() const noexcept;
		[[nodiscard]] std::wstring getLiteralUtf16() const noexcept;
		[[nodiscard]] CommandName getLiteralForCommandName() const noexcept;

		[[nodiscard]] std::string getLocalisedUtf8() const noexcept;
		[[nodiscard]] std::wstring getLocalisedUtf16() const noexcept;

		[[nodiscard]] std::string getEnglishUtf8() const noexcept;
		[[nodiscard]] std::wstring getEnglishUtf16() const noexcept;
		[[nodiscard]] CommandName getEnglishForCommandName() const noexcept;

		void makeLiteralLocalised() noexcept;
	};
#pragma pack(pop)
}
