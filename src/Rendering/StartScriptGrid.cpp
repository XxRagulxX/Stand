#include "Rendering/StartScriptGrid.hpp"

#include "Core/Pointers.hpp"
#include "Core/StackSizes.hpp"
#include "Rendering/GridItemBoundToggle.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemIntStepper.hpp"
#include "Rendering/GridItemLocalList.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Scripting/ScriptNames.hpp"
#include "Scripting/Scripts.hpp"
#include "Scripting/scrThread.hpp"
#include "Util/Joaat.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <limits>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Duplicated from MenuScripts.cpp's own file-static stackSizes[]
		// (internal linkage there, so it isn't reachable from here) -
		// same "mirror the original's logic, don't reuse it" approach as
		// every other port in this series (e.g. StatEditorGrid.cpp's own
		// note on StatEditor.cpp's file-static helpers).
		constexpr std::pair<const char*, int> kStackSizes[] = {
		    {"MICRO", static_cast<int>(eStackSizes::MICRO)},
		    {"MINI", static_cast<int>(eStackSizes::MINI)},
		    {"DEFAULT", static_cast<int>(eStackSizes::DEFAULT)},
		    {"SPECIAL_ABILITY", static_cast<int>(eStackSizes::SPECIAL_ABILITY)},
		    {"FRIEND", static_cast<int>(eStackSizes::FRIEND)},
		    {"SHOP", static_cast<int>(eStackSizes::SHOP)},
		    {"CELLPHONE", static_cast<int>(eStackSizes::CELLPHONE)},
		    {"VEHICLE_SPAWN", static_cast<int>(eStackSizes::VEHICLE_SPAWN)},
		    {"CAR_MOD_SHOP", static_cast<int>(eStackSizes::CAR_MOD_SHOP)},
		    {"PAUSE_MENU_SCRIPT", static_cast<int>(eStackSizes::PAUSE_MENU_SCRIPT)},
		    {"APP_INTERNET", static_cast<int>(eStackSizes::APP_INTERNET)},
		    {"MULTIPLAYER_MISSION", static_cast<int>(eStackSizes::MULTIPLAYER_MISSION)},
		    {"CONTACTS_APP", static_cast<int>(eStackSizes::CONTACTS_APP)},
		    {"INTERACTION_MENU", static_cast<int>(eStackSizes::INTERACTION_MENU)},
		    {"SCRIPT_XML", static_cast<int>(eStackSizes::SCRIPT_XML)},
		    {"PROPERTY_INT", static_cast<int>(eStackSizes::PROPERTY_INT)},
		    {"ACTIVITY_CREATOR_INT", static_cast<int>(eStackSizes::ACTIVITY_CREATOR_INT)},
		    {"SMPL_INTERIOR", static_cast<int>(eStackSizes::SMPL_INTERIOR)},
		    {"WAREHOUSE", static_cast<int>(eStackSizes::WAREHOUSE)},
		    {"IE_DELIVERY", static_cast<int>(eStackSizes::IE_DELIVERY)},
		    {"SHOP_CONTROLLER", static_cast<int>(eStackSizes::SHOP_CONTROLLER)},
		    {"AM_MP_YACHT", static_cast<int>(eStackSizes::AM_MP_YACHT)},
		    {"INGAMEHUD", static_cast<int>(eStackSizes::INGAMEHUD)},
		    {"TRANSITION", static_cast<int>(eStackSizes::TRANSITION)},
		    {"FMMC_LAUNCHER", static_cast<int>(eStackSizes::FMMC_LAUNCHER)},
		    {"MULTIPLAYER_FREEMODE", static_cast<int>(eStackSizes::MULTIPLAYER_FREEMODE)},
		    {"MISSION", static_cast<int>(eStackSizes::MISSION)},
		    {"MP_LAUNCH_SCRIPT", static_cast<int>(eStackSizes::MP_LAUNCH_SCRIPT)}};

		std::vector<std::string> StackSizeNames()
		{
			std::vector<std::string> names;
			names.reserve(std::size(kStackSizes));
			for (auto& entry : kStackSizes)
				names.push_back(std::format("{} ({})", entry.first, entry.second));
			return names;
		}

		int FindStackSizeIndex(int stackSize)
		{
			for (size_t i = 0; i < std::size(kStackSizes); i++)
				if (kStackSizes[i].second == stackSize)
					return static_cast<int>(i);

			return 0;
		}
	}

	// Origin (1438, 622) - offset further down than every other content
	// Grid's usual (1438, 587) origin to clear ScriptsGrid's own tab
	// strip, same as every other GridTabbed target does (see
	// GridTabbed.hpp's own class comment and VehicleSpawnNewGrid's
	// identical offset).
	StartScriptGrid::StartScriptGrid() :
	    Grid(Theme::GetTabbedContentOrigin(), 0),
	    m_StackSize(kStackSizes[0].second)
	{
	}

	void StartScriptGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Start Script", Theme::kText));

		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [] {
			    std::vector<std::string> rows;
			    rows.reserve(scriptNames.size());
			    for (auto name : scriptNames)
				    rows.push_back(name);
			    return rows;
		    },
		    [this](size_t, const std::string& value, bool, bool, bool) {
			    m_SelectedScript = value;
			    m_LauncherIndex = Scripts::GetLauncherIndexFromScript(Joaat(value));
			    invalidate();
		    }));

		if (!m_SelectedScript.empty())
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Selected: {}", m_SelectedScript), Theme::kText));

		items_draft.push_back(std::make_unique<GridItemLocalList>(Theme::kContentWidth,
		    kItemH,
		    "Stack Size",
		    StackSizeNames(),
		    FindStackSizeIndex(m_StackSize),
		    [this](int index) {
			    if (index >= 0 && static_cast<size_t>(index) < std::size(kStackSizes))
				    m_StackSize = kStackSizes[index].second;
		    }));

		// Arg Count - changing it changes how many Arg[i] rows exist
		// below, so it invalidates same as every other shape-changing
		// action in this porting effort; m_Args.resize() preserves
		// existing values and zero-fills new ones, the same behaviour
		// the original's own manual new[]/memcpy/delete[] resize had.
		items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
		    kItemH,
		    "Arg Count",
		    m_ArgCount,
		    0,
		    32,
		    [this](int value) {
			    m_ArgCount = value;
			    m_Args.resize(static_cast<size_t>(m_ArgCount), 0);
			    invalidate();
		    }));

		for (int i = 0; i < m_ArgCount; i++)
		{
			// No 64-bit stepper widget exists yet - same documented
			// narrowing tradeoff as SavedVariableGridRows.cpp's own
			// INT64 case.
			const auto clamped = static_cast<int>(std::clamp<std::int64_t>(m_Args[i], std::numeric_limits<int>::min(), std::numeric_limits<int>::max()));
			items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
			    kItemH,
			    std::format("Arg[{}]", i),
			    clamped,
			    std::numeric_limits<int>::min(),
			    std::numeric_limits<int>::max(),
			    [this, i](int value) {
				    m_Args[i] = static_cast<std::int64_t>(value);
			    }));
		}

		items_draft.push_back(std::make_unique<GridItemBoundToggle>(
		    Theme::kContentWidth,
		    kItemH,
		    "Pause After Starting",
		    [this] {
			    return m_PauseAfterStarting;
		    },
		    [this](bool value) {
			    m_PauseAfterStarting = value;
		    }));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Start Script", [this] {
			const auto scriptName = m_SelectedScript;
			const auto stackSize = m_StackSize;
			auto args = m_Args;
			const auto pauseAfterStarting = m_PauseAfterStarting;

			FiberPool::queueJob([scriptName, stackSize, args, pauseAfterStarting]() mutable {
				auto hash = Joaat(scriptName);

				if (!SCRIPT::DOES_SCRIPT_WITH_NAME_HASH_EXIST(hash))
				{
					Notifications::Show("Start Script", "Script does not exist.", NotificationType::Error);
					return;
				}

				if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(hash) > 0)
				{
					Notifications::Show("Start Script", "Script is already running.", NotificationType::Error);
					return;
				}

				if (MISC::GET_NUMBER_OF_FREE_STACKS_OF_THIS_SIZE(stackSize) == 0)
				{
					Notifications::Show("Start Script", "No free stack of this size.", NotificationType::Error);
					return;
				}

				while (!SCRIPT::HAS_SCRIPT_WITH_NAME_HASH_LOADED(hash))
				{
					SCRIPT::REQUEST_SCRIPT_WITH_NAME_HASH(hash);
					Script::current()->yield();
				}

				int id = 0;
				if (!args.empty())
				{
					id = BUILTIN::START_NEW_SCRIPT_WITH_NAME_HASH_AND_ARGS(hash, args.data(), static_cast<int>(args.size()), stackSize);
				}
				else
				{
					id = BUILTIN::START_NEW_SCRIPT_WITH_NAME_HASH(hash, stackSize);
				}

				if (pauseAfterStarting)
				{
					if (auto* thread = Scripts::FindScriptThreadByID(id))
						thread->m_Context.m_State = rage::scrThread::State::PAUSED;
				}

				SCRIPT::SET_SCRIPT_WITH_NAME_HASH_AS_NO_LONGER_NEEDED(hash);
				Notifications::Show("Start Script", std::format("Started script with ID {}.", id), NotificationType::Success);
			});
		}));

		if (m_LauncherIndex && *Pointers.IsSessionStarted)
		{
			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Start Session Script", [this] {
				const auto scriptName = m_SelectedScript;
				FiberPool::queueJob([scriptName] {
					Scripts::StartLauncherScript(Joaat(scriptName));
				});
			}));
			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Start Script With Event", [this] {
				const auto scriptName = m_SelectedScript;
				FiberPool::queueJob([scriptName] {
					Scripts::ForceScriptOnPlayer(Joaat(scriptName), -1);
				});
			}));
		}
	}
}
