#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class SettingsInputContextHotkeysGrid : public Grid
	{
	public:
		SettingsInputContextHotkeysGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
