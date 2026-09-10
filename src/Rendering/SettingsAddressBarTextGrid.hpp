#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsAddressBarTextGrid : public Grid
	{
	public:
		SettingsAddressBarTextGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
