#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsInputControllerSchemeGrid : public Grid
	{
	public:
		SettingsInputControllerSchemeGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
