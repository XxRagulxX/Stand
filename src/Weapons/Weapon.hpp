#pragma once
#include "Core/types.hpp"
#include <vector>

namespace Stand
{
	struct Weapon
	{
		enum Category : uint8_t
		{
			MELEE = 0,
			THROWABLE,
			PISTOL,
			SMG,
			RIFLE,
			SHOTGUN,
			SNIPER,
			HEAVY,
		};

		const Hash hash;
		const char* const name;
		const char* const cmd_suffix;
		const Category category;

		constexpr Weapon(Hash hash, const char* name, const char* cmd_suffix, Category category) noexcept
			: hash(hash), name(name), cmd_suffix(cmd_suffix), category(category)
		{}

		static const std::vector<Weapon> weapons;

		[[nodiscard]] static std::vector<Hash> getAllHashes();
		[[nodiscard]] static std::vector<Hash> getCategoryHashes(Category cat);

		static const char* getCategoryName(Category cat) noexcept;
		static const char* getCategoryGetCmd(Category cat) noexcept;
		static const char* getCategoryRemCmd(Category cat) noexcept;
	};
}
