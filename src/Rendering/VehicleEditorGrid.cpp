#include "Rendering/VehicleEditorGrid.hpp"

#include "Rendering/GridItemBoundToggle.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemIntStepper.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemTextInput.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Vehicle/ModNames.hpp"
#include "Vehicle/VehicleModel.hpp"
#include "Vehicle/VehicleValues.hpp"
#include "World/Self.hpp"

#include <format>
#include <functional>
#include <optional>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	// Origin (1438, 587) matches every other content Grid's. Spacer is
	// 0, not 3 - confirmed against real Stand's own source (origin/
	// stand-reference) that individual list rows have zero gap between
	// them; the 3-unit spacer real Stand does use is only ever between
	// distinct chrome pieces (addressbar/tabs/list), never between rows -
	// see the comment in MenuGrid.cpp's anonymous namespace for why (no
	// shared header for these yet). Each item below specifies its own
	// width (Theme::kContentWidth) rather than the Grid itself, matching
	// Stand's real Grid - see Grid.hpp's class comment.
	VehicleEditorGrid::VehicleEditorGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void VehicleEditorGrid::SyncVehicle()
	{
		if (!Self::GetVehicle())
		{
			if (m_CurrentVeh != 0)
			{
				m_CurrentVeh = 0;
				invalidate();
			}
			return;
		}

		if (m_PreparingVehicle)
			return;

		if (m_CurrentVeh != Self::GetVehicle().GetHandle())
			RefreshVehicle(true);
	}

	void VehicleEditorGrid::draw()
	{
		SyncVehicle();
		Grid::draw();
	}

	void VehicleEditorGrid::drawText()
	{
		SyncVehicle();
		Grid::drawText();
	}

	GridItem* VehicleEditorGrid::findItemAt(int16_t cursorX, int16_t cursorY)
	{
		SyncVehicle();
		return Grid::findItemAt(cursorX, cursorY);
	}

	// Mirrors VehicleEditor.cpp's own prepareVehicle() exactly.
	void VehicleEditorGrid::RefreshVehicle(bool resetSlot)
	{
		if (resetSlot)
			m_SelectedSlot = -1;

		m_PreparingVehicle = true;

		FiberPool::queueJob([this] {
			m_CurrentVeh = Self::GetVehicle().GetHandle();
			auto model = Self::GetVehicle().GetModel();
			m_FrontWheelStockMod = -1;
			m_RearWheelStockMod = -1;
			m_VehName = Self::GetVehicle().GetFullName();
			m_OwnedMods = Self::GetVehicle().GetOwnedMods();
			m_IsBennys = VehicleModel::IsBennys(m_CurrentVeh);
			m_Plate = Self::GetVehicle().GetPlateText();

			if (!HUD::HAS_THIS_ADDITIONAL_TEXT_LOADED("MOD_MNU", 10))
			{
				HUD::CLEAR_ADDITIONAL_TEXT(10, TRUE);
				HUD::REQUEST_ADDITIONAL_TEXT("MOD_MNU", 10);
				Script::current()->yield();
			}

			VEHICLE::SET_VEHICLE_MOD_KIT(m_CurrentVeh, 0);

			std::map<int, std::string> slotNames;
			std::map<int, std::map<int, std::string>> modNames;
			std::map<std::string, std::vector<int>> frontWheelMap;
			std::map<std::string, std::vector<int>> rearWheelMap;

			slotNames[(int)CustomVehicleModType::MOD_PLATE_STYLE] = "Plate Style";
			slotNames[(int)CustomVehicleModType::MOD_WINDOW_TINT] = "Window Tint";
			slotNames[(int)CustomVehicleModType::MOD_WHEEL_TYPE] = "Wheel Type";

			modNames[(int)CustomVehicleModType::MOD_PLATE_STYLE].insert(lscPlateStyles.begin(), lscPlateStyles.end());
			modNames[(int)CustomVehicleModType::MOD_WINDOW_TINT].insert(lscWindowTintTypes.begin(), lscWindowTintTypes.end());
			modNames[(int)CustomVehicleModType::MOD_WHEEL_TYPE].insert(lscWheelStyles.begin(), lscWheelStyles.end());

			for (int slot = (int)VehicleModType::MOD_SPOILERS; slot <= (int)VehicleModType::MOD_LIGHTBAR; slot++)
			{
				int count = VEHICLE::GET_NUM_VEHICLE_MODS(m_CurrentVeh, slot);
				if (count <= 0)
					continue;

				int owner_mod = m_OwnedMods[slot];

				std::string slot_name = VehicleModel::GetModSlotName(model, m_CurrentVeh, slot);
				if (slot_name.empty())
					continue;

				// No "##<slot>" suffix (unlike the original) - that was
				// purely an ImGui widget-id disambiguator; this system's
				// GridItemSelectList selects by index, not by label text,
				// so it isn't needed.
				slotNames[slot] = slot_name;

				std::map<int, std::string> mods;
				for (int mod = -1; mod < count; mod++)
				{
					if (VehicleModel::CheckModBlacklist(model, slot, mod))
						continue;

					bool is_repeated = false;
					std::string mod_name = VehicleModel::GetModName(model, m_CurrentVeh, slot, mod, count);
					if (mod_name.empty() || mod_name == "NULL")
						continue;

					if (slot == (int)VehicleModType::MOD_FRONTWHEEL)
					{
						if (m_IsBennys && mod_name.rfind("Chrome ", 0) == 0)
						{
							std::string new_mod_name = mod_name.substr(7);
							if (!frontWheelMap[new_mod_name].empty())
								mod_name = new_mod_name;
						}

						frontWheelMap[mod_name].push_back(mod);
						if (mod == owner_mod)
							m_FrontWheelStockMod = frontWheelMap[mod_name][0];
						if (frontWheelMap[mod_name].size() > 1)
							is_repeated = true;
					}
					else if (slot == (int)VehicleModType::MOD_REARWHEEL)
					{
						if (m_IsBennys && mod_name.rfind("Chrome ", 0) == 0)
						{
							std::string new_mod_name = mod_name.substr(7);
							if (!rearWheelMap[new_mod_name].empty())
								mod_name = new_mod_name;
						}

						rearWheelMap[mod_name].push_back(mod);
						if (mod == owner_mod)
							m_RearWheelStockMod = rearWheelMap[mod_name][0];
						if (rearWheelMap[mod_name].size() > 1)
							is_repeated = true;
					}

					if (!is_repeated)
						mods[mod] = mod_name;
				}

				modNames[slot] = mods;
			}

			if (modNames.count((int)VehicleModType::MOD_HORNS) > 0)
				modNames[(int)VehicleModType::MOD_HORNS].insert(lscMissingHorns.begin(), lscMissingHorns.end());

			m_SlotDisplayNames = slotNames;
			m_ModDisplayNames = modNames;
			m_FrontWheelMap = frontWheelMap;
			m_RearWheelMap = rearWheelMap;

			m_PreparingVehicle = false;
			invalidate();
		});
	}

	void VehicleEditorGrid::AddModOptionsRows(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Mod Options", Theme::kText));

		items_draft.push_back(std::make_unique<GridItemBoundToggle>(
		    Theme::kContentWidth,
		    kItemH,
		    "Burstible Tires",
		    [this] {
			    return m_OwnedMods[(int)CustomVehicleModType::MOD_TIRE_CAN_BURST] != 0;
		    },
		    [this](bool value) {
			    m_OwnedMods[(int)CustomVehicleModType::MOD_TIRE_CAN_BURST] = value;
			    const auto veh = m_CurrentVeh;
			    FiberPool::queueJob([veh, value] {
				    VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, value);
			    });
		    }));

		items_draft.push_back(std::make_unique<GridItemBoundToggle>(
		    Theme::kContentWidth,
		    kItemH,
		    "Low Grip Tires",
		    [this] {
			    return m_OwnedMods[(int)CustomVehicleModType::MOD_DRIFT_TIRE] != 0;
		    },
		    [this](bool value) {
			    m_OwnedMods[(int)CustomVehicleModType::MOD_DRIFT_TIRE] = value;
			    const auto veh = m_CurrentVeh;
			    FiberPool::queueJob([veh, value] {
				    VEHICLE::SET_DRIFT_TYRES(veh, value);
			    });
		    }));

		items_draft.push_back(std::make_unique<GridItemBoundToggle>(
		    Theme::kContentWidth,
		    kItemH,
		    "Turbo",
		    [this] {
			    return m_OwnedMods[(int)VehicleModType::MOD_TURBO] != 0;
		    },
		    [this](bool value) {
			    m_OwnedMods[(int)VehicleModType::MOD_TURBO] = value;
			    const auto veh = m_CurrentVeh;
			    FiberPool::queueJob([veh, value] {
				    VEHICLE::TOGGLE_VEHICLE_MOD(veh, (int)VehicleModType::MOD_TURBO, value);
			    });
		    }));

		// Tire Smoke also gates whether the "Tire Smoke" Color Options
		// category shows at all (see AddColorRows()) - invalidate() so
		// toggling it live adds/removes that row.
		items_draft.push_back(std::make_unique<GridItemBoundToggle>(
		    Theme::kContentWidth,
		    kItemH,
		    "Tire Smoke",
		    [this] {
			    return m_OwnedMods[(int)VehicleModType::MOD_TYRE_SMOKE] != 0;
		    },
		    [this](bool value) {
			    m_OwnedMods[(int)VehicleModType::MOD_TYRE_SMOKE] = value;
			    const auto veh = m_CurrentVeh;
			    FiberPool::queueJob([veh, value] {
				    VEHICLE::TOGGLE_VEHICLE_MOD(veh, (int)VehicleModType::MOD_TYRE_SMOKE, value);
			    });
			    invalidate();
		    }));
	}

	void VehicleEditorGrid::AddModSlotsRows(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Mod Slots", Theme::kText));

		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [this] {
			    std::vector<std::string> rows;
			    rows.reserve(m_SlotDisplayNames.size());
			    for (auto& [slot, name] : m_SlotDisplayNames)
				    rows.push_back(name);
			    return rows;
		    },
		    [this](size_t index, const std::string&, bool, bool, bool) {
			    size_t i = 0;
			    for (auto& [slot, name] : m_SlotDisplayNames)
			    {
				    if (i == index)
				    {
					    m_SelectedSlot = slot;
					    invalidate();
					    return;
				    }
				    i++;
			    }
		    }));

		if (m_SelectedSlot == -1)
			return;

		const int selectedSlot = m_SelectedSlot;
		const bool isRearWheel = selectedSlot == (int)VehicleModType::MOD_REARWHEEL;
		const bool isWheelMod = isRearWheel || selectedSlot == (int)VehicleModType::MOD_FRONTWHEEL;
		int* wheelStockMod = isRearWheel ? &m_RearWheelStockMod : &m_FrontWheelStockMod;

		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [this, selectedSlot] {
			    std::vector<std::string> rows;
			    if (auto it = m_ModDisplayNames.find(selectedSlot); it != m_ModDisplayNames.end())
				    for (auto& [mod, name] : it->second)
					    rows.push_back(name);
			    return rows;
		    },
		    [this, selectedSlot, isWheelMod, isRearWheel](size_t index, const std::string&, bool, bool, bool) {
			    auto it = m_ModDisplayNames.find(selectedSlot);
			    if (it == m_ModDisplayNames.end())
				    return;

			    size_t i = 0;
			    for (auto& [mod, name] : it->second)
			    {
				    if (i != index)
				    {
					    i++;
					    continue;
				    }

				    const int modId = mod;
				    FiberPool::queueJob([this, selectedSlot, modId, isWheelMod, isRearWheel] {
					    if (selectedSlot >= 0)
					    {
						    VEHICLE::SET_VEHICLE_MOD(m_CurrentVeh, selectedSlot, modId, 0);
						    m_OwnedMods[selectedSlot] = modId;

						    if (isWheelMod)
						    {
							    (isRearWheel ? m_RearWheelStockMod : m_FrontWheelStockMod) = modId;
							    m_OwnedMods[isRearWheel ? (int)CustomVehicleModType::MOD_REARWHEEL_VAR : (int)CustomVehicleModType::MOD_FRONTWHEEL_VAR] = 0;
						    }

						    invalidate();
					    }
					    else if (selectedSlot == (int)CustomVehicleModType::MOD_WINDOW_TINT)
					    {
						    VEHICLE::SET_VEHICLE_WINDOW_TINT(m_CurrentVeh, modId);
						    m_OwnedMods[selectedSlot] = modId;
						    invalidate();
					    }
					    else if (selectedSlot == (int)CustomVehicleModType::MOD_WHEEL_TYPE)
					    {
						    VEHICLE::SET_VEHICLE_WHEEL_TYPE(m_CurrentVeh, modId);
						    VEHICLE::SET_VEHICLE_MOD(m_CurrentVeh, (int)VehicleModType::MOD_FRONTWHEEL, 0, 0);
						    VEHICLE::SET_VEHICLE_MOD(m_CurrentVeh, (int)VehicleModType::MOD_REARWHEEL, 0, 0);
						    RefreshVehicle(false); // original forces a full refresh here (currentVeh = -1)
					    }
					    else if (selectedSlot == (int)CustomVehicleModType::MOD_PLATE_STYLE)
					    {
						    VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(m_CurrentVeh, modId);
						    m_OwnedMods[selectedSlot] = modId;
						    invalidate();
					    }
				    });
				    return;
			    }
		    }));

		if (!isWheelMod || *wheelStockMod == -1)
			return;

		auto& wheelMap = isRearWheel ? m_RearWheelMap : m_FrontWheelMap;
		auto namesIt = m_ModDisplayNames.find(selectedSlot);
		std::string modName;
		if (namesIt != m_ModDisplayNames.end())
			if (auto nameIt = namesIt->second.find(*wheelStockMod); nameIt != namesIt->second.end())
				modName = nameIt->second;

		auto mapIt = wheelMap.find(modName);
		if (mapIt == wheelMap.end())
			return;

		// Mirrors the original's own "Stock"/"Style N" row construction
		// exactly, including a non-Bennys vehicle getting should_custom=1
		// for every "Style N" entry (not just the ones after "Stock") -
		// built once here so rows()/onSelect share the identical list.
		struct StyleEntry
		{
			std::string label;
			int mod;
			int shouldCustom;
		};
		std::vector<StyleEntry> styleEntries;
		const auto& wheelMods = mapIt->second;
		for (size_t i = 0; i < wheelMods.size(); i++)
		{
			const int mod = wheelMods[i];
			if (i == 0 && !m_IsBennys)
				styleEntries.push_back({"Stock", mod, 0});
			styleEntries.push_back({std::format("Style {}", mod), mod, m_IsBennys ? 0 : 1});
		}

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Style", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [styleEntries] {
			    std::vector<std::string> rows;
			    for (auto& e : styleEntries)
				    rows.push_back(e.label);
			    return rows;
		    },
		    [this, styleEntries, selectedSlot](size_t index, const std::string&, bool, bool, bool) {
			    if (index >= styleEntries.size())
				    return;

			    const int mod = styleEntries[index].mod;
			    const int shouldCustom = styleEntries[index].shouldCustom;
			    FiberPool::queueJob([this, selectedSlot, mod, shouldCustom] {
				    VEHICLE::SET_VEHICLE_MOD(m_CurrentVeh, selectedSlot, mod, shouldCustom);
				    RefreshVehicle(false);
			    });
		    }));
	}

	void VehicleEditorGrid::AddExtrasRows(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Extras", Theme::kText));

		for (int extra = (int)CustomVehicleModType::MOD_EXTRA_1; extra >= (int)CustomVehicleModType::MOD_EXTRA_14; extra--)
		{
			if (!m_OwnedMods.count(extra))
				continue;

			const int id = (extra - (int)CustomVehicleModType::MOD_EXTRA_1) * -1;
			items_draft.push_back(std::make_unique<GridItemBoundToggle>(
			    Theme::kContentWidth,
			    kItemH,
			    std::format("Extra {}", id),
			    [this, extra] {
				    return m_OwnedMods[extra] == 1;
			    },
			    [this, extra, id](bool value) {
				    m_OwnedMods[extra] = value;
				    const auto veh = m_CurrentVeh;
				    FiberPool::queueJob([veh, id, value] {
					    VEHICLE::SET_VEHICLE_EXTRA(veh, id, !value);
				    });
			    }));
		}
	}

	void VehicleEditorGrid::AddNeonRows(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Neon Light Options", Theme::kText));

		// Also gates the "Headlight" Color Options category - see
		// AddColorRows().
		items_draft.push_back(std::make_unique<GridItemBoundToggle>(
		    Theme::kContentWidth,
		    kItemH,
		    "Headlight",
		    [this] {
			    return m_OwnedMods[(int)VehicleModType::MOD_XENON_LIGHTS] != 0;
		    },
		    [this](bool value) {
			    m_OwnedMods[(int)VehicleModType::MOD_XENON_LIGHTS] = value;
			    const auto veh = m_CurrentVeh;
			    FiberPool::queueJob([veh, value] {
				    VEHICLE::TOGGLE_VEHICLE_MOD(veh, (int)VehicleModType::MOD_XENON_LIGHTS, value);
			    });
			    invalidate();
		    }));

		auto addNeon = [&](const char* label, CustomVehicleModType modType, NeonLightLocations location) {
			items_draft.push_back(std::make_unique<GridItemBoundToggle>(
			    Theme::kContentWidth,
			    kItemH,
			    label,
			    [this, modType] {
				    return m_OwnedMods[(int)modType] != 0;
			    },
			    [this, modType, location](bool value) {
				    m_OwnedMods[(int)modType] = value;
				    const auto veh = m_CurrentVeh;
				    FiberPool::queueJob([veh, location, value] {
					    VEHICLE::SET_VEHICLE_NEON_ENABLED(veh, (int)location, value);
				    });
			    }));
		};

		addNeon("Left", CustomVehicleModType::MOD_NEON_LEFT_ON, NeonLightLocations::NEON_LEFT);
		addNeon("Right", CustomVehicleModType::MOD_NEON_RIGHT_ON, NeonLightLocations::NEON_RIGHT);
		addNeon("Front", CustomVehicleModType::MOD_NEON_FRONT_ON, NeonLightLocations::NEON_FRONT);
		addNeon("Back", CustomVehicleModType::MOD_NEON_BACK_ON, NeonLightLocations::NEON_BACK);
	}

	void VehicleEditorGrid::AddColorRows(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Color Options", Theme::kText));

		// Same "snap back to Primary if the category currently selected
		// just became unavailable" guard as the original's own per-frame
		// check.
		if ((m_ColorToChange == 7 && !m_OwnedMods[(int)VehicleModType::MOD_XENON_LIGHTS]) || (m_ColorToChange == 5 && !m_OwnedMods[(int)VehicleModType::MOD_TYRE_SMOKE]))
		{
			m_ColorToChange = 0;
			m_ColorType = 9;
		}

		struct CategoryEntry
		{
			std::string label;
			int colorToChange;
			std::optional<int> colorType; // nullopt = leave m_ColorType as-is (Primary/Secondary)
		};
		std::vector<CategoryEntry> categories = {
		    {"Primary", 0, std::nullopt},
		    {"Secondary", 1, std::nullopt},
		    {"Pearlescent", 2, 7},
		    {"Interior", 3, 10},
		    {"Dashboard", 4, 11},
		};
		if (m_OwnedMods[(int)VehicleModType::MOD_TYRE_SMOKE])
			categories.push_back({"Tire Smoke", 5, 9});
		categories.push_back({"Wheel Color", 6, 8});
		if (m_OwnedMods[(int)VehicleModType::MOD_XENON_LIGHTS])
			categories.push_back({"Headlight", 7, 12});
		categories.push_back({"Neon", 8, 9});

		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [categories] {
			    std::vector<std::string> rows;
			    for (auto& c : categories)
				    rows.push_back(c.label);
			    return rows;
		    },
		    [this, categories](size_t index, const std::string&, bool, bool, bool) {
			    if (index >= categories.size())
				    return;

			    m_ColorToChange = categories[index].colorToChange;
			    if (categories[index].colorType)
				    m_ColorType = *categories[index].colorType;
			    if ((m_ColorToChange == 0 || m_ColorToChange == 1) && m_ColorType > 6)
				    m_ColorType = 9;

			    invalidate();
		    }));

		// Primary/Secondary get an extra "palette family" picker in
		// front of the actual color list - Custom (RGB)/Remove Custom/
		// one of six discrete palettes.
		if (m_ColorToChange == 0 || m_ColorToChange == 1)
		{
			items_draft.push_back(std::make_unique<GridItemSelectList>(
			    Theme::kContentWidth,
			    "Search",
			    [] {
				    return std::vector<std::string>{"Custom", "Remove Custom", "Chrome", "Classic", "Matte", "Metals", "Util", "Worn", "Chameleon"};
			    },
			    [this](size_t index, const std::string&, bool, bool, bool) {
				    if (index == 1)
				    {
					    const int colorToChange = m_ColorToChange;
					    FiberPool::queueJob([this, colorToChange] {
						    if (colorToChange == 0)
							    VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(m_CurrentVeh);
						    else
							    VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(m_CurrentVeh);
						    VEHICLE::SET_VEHICLE_COLOURS(m_CurrentVeh, m_OwnedMods[(int)CustomVehicleModType::MOD_PRIMARY_COL], m_OwnedMods[(int)CustomVehicleModType::MOD_SECONDARY_COL]);
					    });
					    return;
				    }

				    m_ColorType = (index == 0) ? 9 : (static_cast<int>(index) - 2);
				    invalidate();
			    }));
		}

		if (m_ColorType == 9)
		{
			// Custom RGB - discrete steppers, same "no gradient/shader
			// support" tradeoff as GridItemCommandColourCustom's own class
			// comment. Resolves which owned_mods R/G/B keys this edit
			// targets - fixes a bug in the original (its own equivalent
			// switch used color_to_change values 2/3, which its category
			// list never actually produces when color_type==9 - only
			// 0/1/5/8 are reachable here - so its Tire Smoke/Neon custom
			// editor silently read/wrote the Primary color slots
			// instead; this resolves 5/8 to their own real
			// MOD_TIRESMOKE_COL_*/MOD_NEON_COL_* storage).
			int rKey = (int)CustomVehicleModType::MOD_PRIMARY_COL_R;
			int gKey = (int)CustomVehicleModType::MOD_PRIMARY_COL_G;
			int bKey = (int)CustomVehicleModType::MOD_PRIMARY_COL_B;
			if (m_ColorToChange == 1)
			{
				rKey = (int)CustomVehicleModType::MOD_SECONDARY_COL_R;
				gKey = (int)CustomVehicleModType::MOD_SECONDARY_COL_G;
				bKey = (int)CustomVehicleModType::MOD_SECONDARY_COL_B;
			}
			else if (m_ColorToChange == 5)
			{
				rKey = (int)CustomVehicleModType::MOD_TIRESMOKE_COL_R;
				gKey = (int)CustomVehicleModType::MOD_TIRESMOKE_COL_G;
				bKey = (int)CustomVehicleModType::MOD_TIRESMOKE_COL_B;
			}
			else if (m_ColorToChange == 8)
			{
				rKey = (int)CustomVehicleModType::MOD_NEON_COL_R;
				gKey = (int)CustomVehicleModType::MOD_NEON_COL_G;
				bKey = (int)CustomVehicleModType::MOD_NEON_COL_B;
			}

			if (m_ColorToChange == 5 || m_ColorToChange == 8)
			{
				const bool useNeonPresets = (m_ColorToChange == 8);
				items_draft.push_back(std::make_unique<GridItemSelectList>(
				    Theme::kContentWidth,
				    "Search",
				    [useNeonPresets] {
					    std::vector<std::string> rows;
					    for (auto& [name, rgb] : (useNeonPresets ? lscNeonRgb : lscTireSmokeRgb))
						    rows.push_back(name);
					    return rows;
				    },
				    [this, useNeonPresets, rKey, gKey, bKey, colorToChange = m_ColorToChange](size_t index, const std::string&, bool, bool, bool) {
					    auto& presets = useNeonPresets ? lscNeonRgb : lscTireSmokeRgb;
					    size_t i = 0;
					    for (auto& [name, rgb] : presets)
					    {
						    if (i != index)
						    {
							    i++;
							    continue;
						    }

						    m_OwnedMods[rKey] = rgb[0];
						    m_OwnedMods[gKey] = rgb[1];
						    m_OwnedMods[bKey] = rgb[2];
						    const auto veh = m_CurrentVeh;
						    FiberPool::queueJob([veh, rgb, colorToChange] {
							    if (colorToChange == 5)
								    VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, rgb[0], rgb[1], rgb[2]);
							    else
								    VEHICLE::SET_VEHICLE_NEON_COLOUR(veh, rgb[0], rgb[1], rgb[2]);
						    });
						    invalidate();
						    return;
					    }
				    }));
			}

			auto applyCustomColor = [this, rKey, gKey, bKey, colorToChange = m_ColorToChange] {
				const int r = m_OwnedMods[rKey];
				const int g = m_OwnedMods[gKey];
				const int b = m_OwnedMods[bKey];
				const auto veh = m_CurrentVeh;
				FiberPool::queueJob([veh, r, g, b, colorToChange] {
					switch (colorToChange)
					{
					case 0: VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, r, g, b); break;
					case 1: VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, r, g, b); break;
					case 5: VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, r, g, b); break;
					case 8: VEHICLE::SET_VEHICLE_NEON_COLOUR(veh, r, g, b); break;
					}
				});
			};

			items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth, kItemH, "Red", m_OwnedMods[rKey], 0, 255, [this, rKey, applyCustomColor](int value) {
				m_OwnedMods[rKey] = value;
				applyCustomColor();
			}));
			items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth, kItemH, "Green", m_OwnedMods[gKey], 0, 255, [this, gKey, applyCustomColor](int value) {
				m_OwnedMods[gKey] = value;
				applyCustomColor();
			}));
			items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth, kItemH, "Blue", m_OwnedMods[bKey], 0, 255, [this, bKey, applyCustomColor](int value) {
				m_OwnedMods[bKey] = value;
				applyCustomColor();
			}));
			return;
		}

		// Everything else is a direct palette-index list - a small
		// local helper (rather than one per case) since only the native
		// dispatch + owned_mods key(s) actually differ between them.
		auto addPaletteRows = [&](const std::map<int, const char*>& palette, std::function<void(int)> onPick) {
			items_draft.push_back(std::make_unique<GridItemSelectList>(
			    Theme::kContentWidth,
			    "Search",
			    [&palette] {
				    std::vector<std::string> rows;
				    for (auto& [color, name] : palette)
					    rows.push_back(name);
				    return rows;
			    },
			    [&palette, onPick](size_t index, const std::string&, bool, bool, bool) {
				    size_t i = 0;
				    for (auto& [color, name] : palette)
				    {
					    if (i == index)
					    {
						    onPick(color);
						    return;
					    }
					    i++;
				    }
			    }));
		};

		if (m_ColorType == 0)
		{
			// Chrome - a single fixed value, no real list needed.
			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Chrome", [this] {
				const int colorToChange = m_ColorToChange;
				FiberPool::queueJob([this, colorToChange] {
					if (colorToChange == 0)
						m_OwnedMods[(int)CustomVehicleModType::MOD_PRIMARY_COL] = (int)VehicleColorsChrome::COLOR_CHROME;
					else
						m_OwnedMods[(int)CustomVehicleModType::MOD_SECONDARY_COL] = (int)VehicleColorsChrome::COLOR_CHROME;
					VEHICLE::SET_VEHICLE_COLOURS(m_CurrentVeh, m_OwnedMods[(int)CustomVehicleModType::MOD_PRIMARY_COL], m_OwnedMods[(int)CustomVehicleModType::MOD_SECONDARY_COL]);
				});
			}));
		}
		else if (m_ColorType >= 1 && m_ColorType <= 6)
		{
			// Classic/Matte/Metals/Util/Worn/Chameleon - both Primary and
			// Secondary write through SET_VEHICLE_COLOURS with both
			// channels at once, same as the original.
			static const std::map<int, const char*>* const kPalettes[] = {nullptr, &lscClassicColors, &lscMatteColors, &lscMetalColors, &lscUtilColors, &lscWornColors, &lscChameleonColors};
			addPaletteRows(*kPalettes[m_ColorType], [this](int color) {
				const int colorToChange = m_ColorToChange;
				FiberPool::queueJob([this, colorToChange, color] {
					if (colorToChange == 0)
						m_OwnedMods[(int)CustomVehicleModType::MOD_PRIMARY_COL] = color;
					else
						m_OwnedMods[(int)CustomVehicleModType::MOD_SECONDARY_COL] = color;
					VEHICLE::SET_VEHICLE_COLOURS(m_CurrentVeh, m_OwnedMods[(int)CustomVehicleModType::MOD_PRIMARY_COL], m_OwnedMods[(int)CustomVehicleModType::MOD_SECONDARY_COL]);
				});
			});
		}
		else if (m_ColorType == 7)
		{
			// Pearlescent
			addPaletteRows(lscClassicColors, [this](int color) {
				FiberPool::queueJob([this, color] {
					m_OwnedMods[(int)CustomVehicleModType::MOD_PEARLESCENT_COL] = color;
					VEHICLE::SET_VEHICLE_EXTRA_COLOURS(m_CurrentVeh, m_OwnedMods[(int)CustomVehicleModType::MOD_PEARLESCENT_COL], m_OwnedMods[(int)CustomVehicleModType::MOD_WHEEL_COL]);
				});
			});
		}
		else if (m_ColorType == 8)
		{
			// Wheel Color - Alloy plus Classic plus Chameleon, all in
			// one list, same as the original.
			constexpr int kAlloyColor = 156;
			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Alloy", [this] {
				FiberPool::queueJob([this] {
					m_OwnedMods[(int)CustomVehicleModType::MOD_WHEEL_COL] = kAlloyColor;
					VEHICLE::SET_VEHICLE_EXTRA_COLOURS(m_CurrentVeh, m_OwnedMods[(int)CustomVehicleModType::MOD_PEARLESCENT_COL], m_OwnedMods[(int)CustomVehicleModType::MOD_WHEEL_COL]);
				});
			}));

			auto onPickWheelColor = [this](int color) {
				FiberPool::queueJob([this, color] {
					m_OwnedMods[(int)CustomVehicleModType::MOD_WHEEL_COL] = color;
					VEHICLE::SET_VEHICLE_EXTRA_COLOURS(m_CurrentVeh, m_OwnedMods[(int)CustomVehicleModType::MOD_PEARLESCENT_COL], m_OwnedMods[(int)CustomVehicleModType::MOD_WHEEL_COL]);
				});
			};
			addPaletteRows(lscClassicColors, onPickWheelColor);
			addPaletteRows(lscChameleonColors, onPickWheelColor);
		}
		else if (m_ColorType == 10)
		{
			// Interior
			addPaletteRows(lscClassicColors, [this](int color) {
				FiberPool::queueJob([this, color] {
					m_OwnedMods[(int)CustomVehicleModType::MOD_INTERIOR_COL] = color;
					VEHICLE::SET_VEHICLE_EXTRA_COLOUR_5(m_CurrentVeh, m_OwnedMods[(int)CustomVehicleModType::MOD_INTERIOR_COL]);
				});
			});
		}
		else if (m_ColorType == 11)
		{
			// Dashboard
			addPaletteRows(lscClassicColors, [this](int color) {
				FiberPool::queueJob([this, color] {
					m_OwnedMods[(int)CustomVehicleModType::MOD_DASHBOARD_COL] = color;
					VEHICLE::SET_VEHICLE_EXTRA_COLOUR_6(m_CurrentVeh, m_OwnedMods[(int)CustomVehicleModType::MOD_DASHBOARD_COL]);
				});
			});
		}
		else if (m_ColorType == 12)
		{
			// Headlight
			addPaletteRows(lscHeadlightColors, [this](int color) {
				FiberPool::queueJob([this, color] {
					m_OwnedMods[(int)CustomVehicleModType::MOD_XENON_COL] = color;
					VEHICLE::SET_VEHICLE_XENON_LIGHT_COLOR_INDEX(m_CurrentVeh, m_OwnedMods[(int)CustomVehicleModType::MOD_XENON_COL]);
				});
			});
		}
	}

	void VehicleEditorGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		if (!Self::GetVehicle())
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Please enter a vehicle.", Theme::kError));
			return;
		}

		if (m_PreparingVehicle)
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Loading vehicle...", Theme::kText));
			return;
		}

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, m_VehName, Theme::kText));
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Refresh Current Vehicle", [this] {
			RefreshVehicle(false);
		}));

		auto plateInput = std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "Plate", m_Plate, nullptr);
		m_PlateInput = plateInput.get();
		items_draft.push_back(std::move(plateInput));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Change Plate", [this] {
			if (!m_PlateInput)
				return;

			const auto plate = m_PlateInput->GetValue();
			FiberPool::queueJob([plate] {
				Self::GetVehicle().SetPlateText(plate);
			});
		}));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Max Vehicle", [this] {
			FiberPool::queueJob([this] {
				Self::GetVehicle().Upgrade();
				RefreshVehicle(false);
			});
		}));

		AddModOptionsRows(items_draft);
		AddModSlotsRows(items_draft);
		AddExtrasRows(items_draft);
		AddNeonRows(items_draft);
		AddColorRows(items_draft);
	}
}
