#include "Rendering/AppearanceGrid.hpp"

#include "Rendering/GridItemFolder.hpp"
#include "Rendering/InvisibilityGrid.hpp"
#include "Rendering/OutfitEditorGrid.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/TransformGrid.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		OutfitEditorGrid g_OutfitEditorContent{};
		InvisibilityGrid g_InvisibilityContent{};
		TransformGrid g_TransformContent{};
	}

	// Origin/spacer match every other content Grid's - see SelfGrid.cpp's
	// identical comment.
	AppearanceGrid::AppearanceGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void AppearanceGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// In real Stand's own CommandTabSelf.cpp order (of what's ported
		// so far). "Outfit Editor" here is real Stand's own Appearance >
		// Outfit subcategory - kept this project's existing, more
		// descriptive label rather than renaming it to bare "Outfit".
		// Still missing, deliberately: Animations, PTFX Trails (both
		// their own subcategories, not built yet), Walk Style, Character
		// Scale (+affects-camera/affects-speed), Visual Z Correction, No
		// Blood, Moist(+lock), Death Particle, Respawn Particle - none
		// of these have a command in this project yet.
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Transform", &g_TransformContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Outfit Editor", &g_OutfitEditorContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Invisibility", &g_InvisibilityContent));
	}
}
