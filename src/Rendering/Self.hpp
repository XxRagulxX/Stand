#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class Self : public Grid
	{
	public:
		Self();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
