#pragma once
#include "Rendering/Grid.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Stand::Rendering
{
	class GridItemTextInput;

	// Content grid for Vehicle > Vehicle Editor: mirrors
	// VehicleEditor.cpp's BuildVehicleEditorMenu() in full - vehicle
	// name/refresh/plate, Mod Options toggles, a Mod Slots master/
	// detail/style cascade, Extras, Neon Light Options, and Color
	// Options (a category list, then either a direct palette index list
	// or a discrete R/G/B custom-color editor, mirroring
	// GridItemCommandColourCustom's own "no gradient/shader support, so every
	// channel steps discretely" precedent - just against plain
	// owned_mods state instead of a registered Command).
	//
	// owned_mods/slot_display_names/mod_display_names/front_wheel_map/
	// rear_wheel_map are all runtime-populated per-vehicle (mirrors the
	// original's own file-static maps, rebuilt by RefreshVehicle()'s
	// FiberPool job exactly like the original's own prepareVehicle()) -
	// members here instead, since this Grid isn't a singleton the way
	// the original's static locals effectively were.
	//
	// The original's every list box is a side-by-side ImGui column
	// (ImGui::SameLine()); this system's Grid is single-column, so every
	// cascade level (Slot -> Mod -> Style, Category -> Palette Family ->
	// Palette Color) stacks vertically instead - the same simplification
	// already made throughout this porting effort (e.g. Outfit Editor's
	// folder/file lists).
	//
	// A selected vehicle can change (or stop existing) with no user
	// action driving it, so - like ThreadsGrid - this needs a per-frame
	// SyncVehicle() check (draw()/drawText()/findItemAt(), before
	// delegating to Grid's own) rather than only action-triggered
	// invalidate(), mirroring the original's own per-frame
	// `currentVeh != Self::GetVehicle().GetHandle()` check.
	class VehicleEditorGrid : public Grid
	{
	public:
		VehicleEditorGrid();

		void draw() override;
		void drawText() override;
		GridItem* findItemAt(int16_t cursorX, int16_t cursorY) override;

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		void SyncVehicle();

		// Mirrors prepareVehicle() - queues a FiberPool job rebuilding
		// every map below from natives, then invalidate()s. resetSlot
		// mirrors the original's own `if (currentVeh != -1) selected_slot
		// = -1` - false for a forced same-vehicle refresh (Refresh
		// Current Vehicle/Max Vehicle/changing wheel type), true for an
		// actual vehicle change.
		void RefreshVehicle(bool resetSlot);

		void AddModOptionsRows(std::vector<std::unique_ptr<GridItem>>& items_draft);
		void AddModSlotsRows(std::vector<std::unique_ptr<GridItem>>& items_draft);
		void AddExtrasRows(std::vector<std::unique_ptr<GridItem>>& items_draft);
		void AddNeonRows(std::vector<std::unique_ptr<GridItem>>& items_draft);
		void AddColorRows(std::vector<std::unique_ptr<GridItem>>& items_draft);

		int m_CurrentVeh = 0;
		std::string m_VehName;
		bool m_PreparingVehicle = false;
		bool m_IsBennys = false;
		int m_SelectedSlot = -1;
		std::string m_Plate;
		GridItemTextInput* m_PlateInput = nullptr;

		std::map<int, std::int32_t> m_OwnedMods;
		std::map<int, std::string> m_SlotDisplayNames;
		std::map<int, std::map<int, std::string>> m_ModDisplayNames;
		std::map<std::string, std::vector<int>> m_FrontWheelMap;
		std::map<std::string, std::vector<int>> m_RearWheelMap;
		int m_FrontWheelStockMod = -1;
		int m_RearWheelStockMod = -1;

		// color_to_change/color_type in the original - kept as the same
		// two small int codes rather than named enums, matching its own
		// scheme exactly (0=Primary,1=Secondary,2=Pearlescent,3=Interior,
		// 4=Dashboard,5=Tire Smoke,6=Wheel Color,7=Headlight,8=Neon for
		// m_ColorToChange; m_ColorType selects which palette family/mode
		// applies - 9 always means "custom RGB").
		int m_ColorToChange = 0;
		int m_ColorType = 9;
	};
}
