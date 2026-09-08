#pragma once
#include "Ped/Outfit.hpp"
#include "Rendering/Grid.hpp"

#include <string>
#include <vector>

namespace Stand::Rendering
{
	class GridItemTextInput;

	// Content grid for Self > Outfit Editor: mirrors OutfitEditor.cpp's
	// OutfitEditorMenu class - Refresh Stats/Randomize Outfit buttons,
	// per-component and per-prop drawable/texture id steppers, and the
	// folder/file browse + save/apply controls.
	//
	// Each stepper's own [lo,hi] hint text and its GridItemIntStepper's
	// clamp range are both built once, from whatever drawable_id_max/
	// texture_id_max RefreshStats() last read (0 until it's actually
	// been run, exactly like the original's own zero-initialized
	// ComponentData before its first "Refresh Stats" click) - since
	// populate() only runs once, neither the label nor the stepper's own
	// [min,max] can track a later RefreshStats() the way the original's
	// per-frame ImGui::InputInt label does. The value actually applied
	// is still always correct regardless: onChange re-clamps against
	// m_Components/m_Props's live drawable_id_max/texture_id_max (kept
	// current by every RefreshStats() call) before issuing the native
	// call, not against the stepper's own possibly-stale display range.
	//
	// Component drawable/texture ids go through
	// PED::SET_PED_COMPONENT_VARIATION; prop drawable ids go through
	// PED::CLEAR_PED_PROP (drawable_id == -1, "no prop") or
	// PED::SET_PED_PROP_INDEX, prop texture ids always through
	// PED::SET_PED_PROP_INDEX - same calls, same quirks (a drawable-id
	// change always sends texture 0, not the component/prop's current
	// texture_id - matching RenderComponents()/RenderProps() exactly,
	// not "fixed") as the original's four Render*() methods.
	//
	// Folder/file browsing and the Save section directly follow
	// SavedVehiclesGrid's own pattern, including its same deliberate
	// deviation from the original: File Name/New Folder are both always
	// shown (not New Folder only outside Root) - see that class's own
	// header comment for why. "Apply hair" is a GridItemBoundToggle over
	// m_ApplyHair instead of the original's function-local static bool,
	// reset to false after every "Apply Selected Outfit" click, same as
	// the original.
	class OutfitEditorGrid : public Grid
	{
	public:
		OutfitEditorGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		void RefreshStats();
		void RefreshList();

		void AddComponentDrawableRows(std::vector<std::unique_ptr<GridItem>>& items_draft);
		void AddComponentTextureRows(std::vector<std::unique_ptr<GridItem>>& items_draft);
		void AddPropDrawableRows(std::vector<std::unique_ptr<GridItem>>& items_draft);
		void AddPropTextureRows(std::vector<std::unique_ptr<GridItem>>& items_draft);

		Outfit::OutfitComponents m_Components{};
		Outfit::OutfitProps m_Props{};

		std::vector<std::string> m_Folders{};
		std::vector<std::string> m_Files{};
		std::string m_Folder{};
		std::string m_File{};

		GridItemTextInput* m_OutfitNameInput = nullptr;
		GridItemTextInput* m_NewFolderInput = nullptr;
		bool m_ApplyHair = false;
	};
}
