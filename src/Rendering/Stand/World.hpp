#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class World : public Grid
	{
	public:
		World();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
