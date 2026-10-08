#pragma once

#include "Util/Label.hpp"
#include "Util/hashtype.hpp"

namespace Stand
{
	struct PedModel
	{
		hash_t hash;
		const Label menu_name;
		const hash_t category;

		PedModel(hash_t hash, Label menu_name, hash_t category)
			: hash(hash), menu_name(std::move(menu_name)), category(category)
		{
		}

		[[nodiscard]] static const PedModel* fromHash(hash_t hash) noexcept;
	};
}
