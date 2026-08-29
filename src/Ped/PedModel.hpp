#pragma once

#include <vector>

#include "Util/Label.hpp"
#include "Util/String2Hash.hpp"

namespace Stand
{
	struct PedModel
	{
		String2Hash hash;
		const Label menu_name;
		std::vector<CommandName> command_names;
		const hash_t category;

		constexpr PedModel(const char* model_name, Label&& menu_name, std::vector<CommandName>&& command_names, hash_t category)
			: hash(model_name), menu_name(std::move(menu_name)), command_names(std::move(command_names)), category(category)
		{
		}

		static PedModel all[1122];

		[[nodiscard]] static const PedModel* fromHash(hash_t hash) noexcept;
	};
}
