#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsHeaderGrid : public Grid
	{
	public:
		SettingsHeaderGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
