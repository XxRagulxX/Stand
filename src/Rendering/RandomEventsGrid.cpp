#include "Rendering/RandomEventsGrid.hpp"

#include "Network/CNetGamePlayer.hpp"
#include "Network/GPBD_FM_2.hpp"
#include "Network/GSBD_RandomEvents.hpp"
#include "Network/ScriptEvent.hpp"
#include "Network/Tunables.hpp"
#include "Rendering/GridItemBoundToggle.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemIntStepper.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/CGameScriptHandlerNetComponent.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/GtaThread.hpp"
#include "Scripting/Script.hpp"
#include "Scripting/ScriptFunction.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Scripting/Scripts.hpp"
#include "Util/Joaat.hpp"
#include "World/FMRandomEvents.hpp"
#include "World/RandomEvents.hpp"
#include "World/Self.hpp"

#include <format>
#include <limits>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Armoured Truck (index 16 in randomEventNames/randomEventScripts)
		// doesn't have real tunables - mirrors ResetEventTunables()'s own
		// special case.
		constexpr int kArmouredTruckIndex = 16;
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
	RandomEventsGrid::RandomEventsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void RandomEventsGrid::EnsureCooldownPatchesEnabled()
	{
		if (m_PatchesEnabled)
			return;

		if (m_TSECooldownPatches.empty())
		{
			for (int event = 0; event < static_cast<int>(randomEventScripts.size()); event++)
			{
				// TODO: this can crash the game
				m_TSECooldownPatches.push_back(ScriptPatches::AddPatch(randomEventScripts[event], ScriptPointer("SendUpdateRECoordsTSECooldownPatch", "43 88 13 2E 00 01"), {0x71, 0x00, 0x00}));
			}
		}

		for (auto& patch : m_TSECooldownPatches)
			patch->Enable();

		m_PatchesEnabled = true;
	}

	std::string RandomEventsGrid::GetEventStateString(GSBD_RandomEvents* gsbd, RANDOM_EVENTS_FREEMODE_DATA* fm, int event) const
	{
		switch (gsbd->EventData[event].State)
		{
		case eRandomEventState::INACTIVE:
			return "Inactive - launching in " + gsbd->EventData[event].TimerState.GetRemainingTimeStr(fm->EventData[event].InactiveTime);
		case eRandomEventState::AVAILABLE:
			return "Available - deactivating in " + gsbd->EventData[event].TimerState.GetRemainingTimeStr(fm->EventData[event].AvailableTime);
		case eRandomEventState::ACTIVE:
			return "Active";
		case eRandomEventState::CLEANUP:
			return "Cleanup";
		}

		return "N/A";
	}

	int RandomEventsGrid::GetNumLocallyActiveEvents(GPBD_FM_2* gpbd) const
	{
		int numEvents = 0;

		for (int event = 0; event < static_cast<int>(randomEventScripts.size()); event++)
			if (gpbd->Entries[Self::GetPlayer().GetId()].RandomEventsClientData.EventData[event].State != eRandomEventClientState::INACTIVE)
				numEvents++;

		return numEvents;
	}

	void RandomEventsGrid::ResetEventTunables(int event)
	{
		if (event == kArmouredTruckIndex)
		{
			m_SetCooldown = *ScriptGlobal(262145).At(33808).As<int*>();
			m_SetAvailability = *ScriptGlobal(262145).At(33809).As<int*>();
		}
		else
		{
			// Phantom Car's cooldown is actually 2147483647ms if
			// STANDARDTARGETTINGTIME is not enabled.
			if (auto tunable = Tunables::GetTunable(randomEventCooldowns[event]))
				m_SetCooldown = *tunable->As<int*>();
			if (auto tunable = Tunables::GetTunable(randomEventAvailabilities[event]))
				m_SetAvailability = *tunable->As<int*>();
		}
	}

	// Called from inside the same FiberPool job that just assigned
	// m_SelectedEvent (see populate()'s own Select Event row) - not
	// wrapped in its own queueJob, matching the original's own
	// OnComboChange() being called directly inside that same job.
	void RandomEventsGrid::OnEventChanged()
	{
		static ScriptFunction getNumFMMCVariations("freemode"_J, ScriptPointer("GetNumFMMCVariations", "5D ? ? ? 01 72 02 39 04").Add(1).Rip());

		// Re-resolved fresh here rather than reusing whatever populate()
		// last saw - this runs on a later fiber tick, by which point the
		// freemode thread/stack could theoretically have gone away (the
		// original has no such guard, since OnComboChange() only ever
		// runs while BuildRandomEventsMenu()'s own gating checks already
		// guaranteed FMRandomEvents was valid moments earlier in the same
		// frame - not a guarantee this deferred call still has).
		if (auto* freemodeThread = Scripts::FindScriptThread("freemode"_J))
		{
			if (auto* fmRandomEvents = RANDOM_EVENTS_FREEMODE_DATA::Get(freemodeThread))
				m_NumSubvariations = getNumFMMCVariations.Call<int>(fmRandomEvents->MissionData.FMMCData[m_SelectedEvent].FMMCType, 0) - 1;
		}

		m_SelectedSubvariation = 0;
		ResetEventTunables(m_SelectedEvent);
		invalidate();
	}

	void RandomEventsGrid::KillActiveEvent()
	{
		if (auto* eventThread = Scripts::FindScriptThread(randomEventScripts[m_SelectedEvent]))
		{
			if (auto* netComponent = reinterpret_cast<GtaThread*>(eventThread)->m_NetComponent)
			{
				if (netComponent->IsLocalPlayerHost())
				{
					ScriptFunction setFMContentScriptServerState(randomEventScripts[m_SelectedEvent], ScriptPointer("SetFMContentScriptServerState", "5D ? ? ? 55 2E 00 5D").Add(1).Rip());
					setFMContentScriptServerState.Call<void>(3);
				}
				else
				{
					ScriptFunction setFMContentScriptClientState(randomEventScripts[m_SelectedEvent], ScriptPointer("SetFMContentScriptClientState", "5D ? ? ? 55 08 00 74").Add(1).Rip());
					setFMContentScriptClientState.Call<void>(3);
				}
			}
		}
		else
		{
			Notifications::Show("Random Events", "Event script is not active. Are you a participant?", NotificationType::Error);
		}
	}

	void RandomEventsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		EnsureCooldownPatchesEnabled();

		// Every gating condition below is its own fresh, self-contained
		// lookup rather than something cached on this class between
		// populate() calls - GPBD_FM_2::Get()/GSBD_RandomEvents::Get()/
		// RANDOM_EVENTS_FREEMODE_DATA::Get() are all cheap pointer
		// resolves (no native calls), so there's no cost to just calling
		// them again inside populate() itself right after, same
		// "recompute on demand rather than cache" trade-off the
		// original's own per-frame ImGuiItem lambda already makes.
		if (!watchCondition([] {
			    return GPBD_FM_2::Get() != nullptr && GSBD_RandomEvents::Get() != nullptr;
		    }))
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Freemode global block is not loaded.", Theme::kError));
			return;
		}

		if (!watchCondition([] {
			    auto* gpbd = GPBD_FM_2::Get();
			    return gpbd && gpbd->Entries[Self::GetPlayer().GetId()].RandomEventsClientData.InitState == eRandomEventClientInitState::INITIALIZED;
		    }))
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Random Events are not initialized.", Theme::kError));
			return;
		}

		if (!watchCondition([] {
			    return Scripts::FindScriptThread("freemode"_J) != nullptr;
		    }))
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Freemode is not running.", Theme::kError));
			return;
		}

		auto* freemodeThread = Scripts::FindScriptThread("freemode"_J);
		auto* fmRandomEvents = RANDOM_EVENTS_FREEMODE_DATA::Get(freemodeThread);

		if (!watchCondition([] {
			    auto* freemode = Scripts::FindScriptThread("freemode"_J);
			    return freemode && RANDOM_EVENTS_FREEMODE_DATA::Get(freemode) != nullptr;
		    }))
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Freemode stack is not valid.", Theme::kError));
			return;
		}

		auto* gpbdFm2 = GPBD_FM_2::Get();
		auto* gsbdRandomEvents = GSBD_RandomEvents::Get();

		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [gsbdRandomEvents] {
			    std::vector<std::string> rows;
			    rows.reserve(randomEventNames.size());
			    for (int i = 0; i < static_cast<int>(randomEventNames.size()); i++)
			    {
				    const char* stateStr = "?";
				    switch (gsbdRandomEvents->EventData[i].State)
				    {
				    case eRandomEventState::INACTIVE:
					    stateStr = "Inactive";
					    break;
				    case eRandomEventState::AVAILABLE:
					    stateStr = "Available";
					    break;
				    case eRandomEventState::ACTIVE:
					    stateStr = "Active";
					    break;
				    case eRandomEventState::CLEANUP:
					    stateStr = "Cleanup";
					    break;
				    }
				    rows.push_back(std::format("{} [{}]", randomEventNames[i], stateStr));
			    }
			    return rows;
		    },
		    [this](std::size_t index, const std::string&, bool, bool, bool) {
			    if (index >= randomEventNames.size())
				    return;

			    const int event = static_cast<int>(index);
			    FiberPool::queueJob([this, event] {
				    m_SelectedEvent = event;
				    OnEventChanged();
			    });
		    }));

		items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
		    kItemH,
		    std::format("Select Location (0-{})", m_NumSubvariations),
		    m_SelectedSubvariation,
		    0,
		    m_NumSubvariations,
		    [this](int value) {
			    m_SelectedSubvariation = value;
		    }));

		const int numActiveEvents = GetNumLocallyActiveEvents(gpbdFm2);
		static Tunable maxEventsTune{"FMREMAXACTIVATEDEVENTS"_J};
		const int maxActiveEvents = maxEventsTune.IsReady() ? maxEventsTune.Get<int>() : 0;
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth,
		    kItemH,
		    std::format("Active Events: {}/{}", numActiveEvents, maxActiveEvents),
		    numActiveEvents >= maxActiveEvents ? Theme::kError : Theme::kText));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Launch Event", [this] {
			FiberPool::queueJob([this] {
				auto* gsbd = GSBD_RandomEvents::Get();
				auto* freemode = Scripts::FindScriptThread("freemode"_J);
				auto* fm = freemode ? RANDOM_EVENTS_FREEMODE_DATA::Get(freemode) : nullptr;
				if (!gsbd || !fm)
					return;

				const int event = m_SelectedEvent;
				if (gsbd->EventData[event].State != eRandomEventState::ACTIVE)
				{
					SCRIPT_EVENT_REQUEST_RANDOM_EVENT eventData;
					eventData.FMMCType = fm->MissionData.FMMCData[event].FMMCType;
					eventData.Subvariation = m_SelectedSubvariation;
					eventData.PlayersToSend = 1; // Set FORCE_LAUNCH bit of all players
					eventData.Send();
					Script::current()->yield(100);
					if (gsbd->EventData[event].State == eRandomEventState::INACTIVE)
						Notifications::Show("Random Events", "Failed to launch event. Are you freemode host?", NotificationType::Error);
				}
				else
				{
					Notifications::Show("Random Events", "Event is already active.", NotificationType::Error);
				}
			});
		}));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Kill Event", [this] {
			FiberPool::queueJob([this] {
				auto* gsbd = GSBD_RandomEvents::Get();
				if (!gsbd)
					return;

				const int event = m_SelectedEvent;
				if (gsbd->EventData[event].State == eRandomEventState::AVAILABLE)
					gsbd->EventData[event].State = eRandomEventState::CLEANUP;
				else if (gsbd->EventData[event].State == eRandomEventState::ACTIVE)
					KillActiveEvent();
				else
					Notifications::Show("Random Events", "Event is not active.", NotificationType::Error);
			});
		}));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Teleport to Event", [this] {
			FiberPool::queueJob([this] {
				auto* gsbd = GSBD_RandomEvents::Get();
				if (!gsbd)
					return;

				const int event = m_SelectedEvent;
				if (gsbd->EventData[event].State >= eRandomEventState::AVAILABLE)
				{
					if (auto coords = gsbd->EventData[event].TriggerPosition)
						Self::GetPed().TeleportTo(coords);
					else
						Notifications::Show("Random Events", "Failed to teleport to event. Coordinates are not valid.", NotificationType::Error);
				}
				else
				{
					Notifications::Show("Random Events", "Event is not active.", NotificationType::Error);
				}
			});
		}));

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Launch/Kill/Set Cooldown/Set Availability require freemode script host.", Theme::kPlaceholderText));

		if (gsbdRandomEvents->EventData[m_SelectedEvent].State == eRandomEventState::ACTIVE)
		{
			if (auto* eventThread = Scripts::FindScriptThread(randomEventScripts[m_SelectedEvent]))
			{
				if (auto* netComponent = reinterpret_cast<GtaThread*>(eventThread)->m_NetComponent)
				{
					if (auto* host = netComponent->GetHost())
						items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Host: {}", host->GetName()), Theme::kText));

					if (!netComponent->IsLocalPlayerHost())
					{
						items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Take Control", [eventThread] {
							FiberPool::queueJob([eventThread] {
								Scripts::ForceScriptHost(eventThread);
							});
						}));
					}
				}
			}
		}

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("State: {}", GetEventStateString(gsbdRandomEvents, fmRandomEvents, m_SelectedEvent)), Theme::kText));

		if (gsbdRandomEvents->EventData[m_SelectedEvent].State == eRandomEventState::INACTIVE)
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Location: N/A", Theme::kText));
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Trigger Range: N/A", Theme::kText));
		}
		else
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Location: {}", gsbdRandomEvents->EventData[m_SelectedEvent].Subvariation), Theme::kText));
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Trigger Range: {:.2f}", gsbdRandomEvents->EventData[m_SelectedEvent].TriggerRange), Theme::kText));
		}

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Cooldown & Availability", Theme::kText));

		items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
		    kItemH,
		    "Cooldown (ms)",
		    m_SetCooldown,
		    std::numeric_limits<int>::min(),
		    std::numeric_limits<int>::max(),
		    [this](int value) {
			    m_SetCooldown = value;
		    }));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Set Cooldown", [this] {
			const int value = m_ApplyInMinutes ? (m_SetCooldown * 60000) : m_SetCooldown;
			const int event = m_SelectedEvent;
			FiberPool::queueJob([event, value] {
				if (auto* freemode = Scripts::FindScriptThread("freemode"_J))
					if (auto* fm = RANDOM_EVENTS_FREEMODE_DATA::Get(freemode))
						fm->EventData[event].InactiveTime = value;
			});
		}));

		items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
		    kItemH,
		    "Availability (ms)",
		    m_SetAvailability,
		    std::numeric_limits<int>::min(),
		    std::numeric_limits<int>::max(),
		    [this](int value) {
			    m_SetAvailability = value;
		    }));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Set Availability", [this] {
			const int value = m_ApplyInMinutes ? (m_SetAvailability * 60000) : m_SetAvailability;
			const int event = m_SelectedEvent;
			FiberPool::queueJob([event, value] {
				if (auto* freemode = Scripts::FindScriptThread("freemode"_J))
					if (auto* fm = RANDOM_EVENTS_FREEMODE_DATA::Get(freemode))
						fm->EventData[event].AvailableTime = value;
			});
		}));

		items_draft.push_back(std::make_unique<GridItemBoundToggle>(
		    Theme::kContentWidth,
		    kItemH,
		    "Apply in Minutes",
		    [this] {
			    return m_ApplyInMinutes;
		    },
		    [this](bool value) {
			    m_ApplyInMinutes = value;
		    }));
	}
}
