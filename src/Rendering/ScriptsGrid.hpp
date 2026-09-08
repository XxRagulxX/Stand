#pragma once
#include "Rendering/GridTabbed.hpp"

namespace Stand::Rendering
{
	// Content grid for Debug > Scripts: mirrors MenuScripts.cpp's
	// BuildScriptsMenu() TabBarItem in full - Threads/Start Script tabs
	// (ThreadsGrid/StartScriptGrid).
	class ScriptsGrid : public GridTabbed
	{
	public:
		ScriptsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
