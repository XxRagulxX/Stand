#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Self > Appearance - real Stand's own category
	// (CommandTabSelf.cpp). See its own populate() for exactly what's
	// here vs. still missing.
	class AppearanceGrid : public Grid
	{
	public:
		AppearanceGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
