#pragma once

#include "Util/hashtype.hpp"

namespace Stand
{
	struct VehicleItem
	{
		hash_t hash;

		[[nodiscard]] static const VehicleItem* fromHash(hash_t hash) noexcept;
	};
}
