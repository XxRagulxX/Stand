#include "Rendering/SavedPlayersGrid.hpp"

#include "Commands/Extra/CommandSavedPlayers.hpp"
#include "Scripting/FiberPool.hpp"
#include "Rendering/GridItemBoundToggle.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemConditional.hpp"
#include "Rendering/GridItemLiveText.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemTextInput.hpp"
#include "Util/Joaat.hpp"
#include "Network/Network.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <cstdlib>
#include <format>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// RenderPlayerItem()'s own status circle - folded into a
		// bracketed state label instead, see this file's own class
		// comment in the header for why (GridItemSelectList only
		// renders plain text rows). "#<rid>" is always the row's own
		// last token, unambiguous to parse back out in onSelect below
		// since a Rockstar Id is always numeric.
		std::string FormatPlayerRow(std::uint64_t rid, const SavedPlayerData& data)
		{
			auto state = data.m_FetchedData ? std::string(FetchedPlayerData::GameStateToString(data.m_FetchedData->m_GameState)) : std::string("UNKNOWN");
			auto name = data.m_Name.empty() ? std::string("(unnamed)") : data.m_Name;
			return std::format("[{}] {} #{}", state, name, rid);
		}

		std::uint64_t ParseRidFromRow(const std::string& row)
		{
			auto pos = row.rfind('#');
			if (pos == std::string::npos)
				return 0;

			return std::strtoull(row.c_str() + pos + 1, nullptr, 10);
		}
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
	SavedPlayersGrid::SavedPlayersGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	SavedPlayerData* SavedPlayersGrid::SelectedPlayerData() const
	{
		if (!m_SelectedRid)
			return nullptr;

		return SavedPlayers::GetPlayerData(m_SelectedRid);
	}

	void SavedPlayersGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Players (RenderPlayerList()) - name-sorted (see this file's own
		// header comment for why), selecting one syncs the editor
		// fields below.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Players", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [] {
			    std::vector<std::string> rows;
			    rows.reserve(SavedPlayers::GetSavedPlayers().size());
			    for (auto& [rid, data] : SavedPlayers::GetSavedPlayers())
				    rows.push_back(FormatPlayerRow(rid, data));
			    std::sort(rows.begin(), rows.end());
			    return rows;
		    },
		    [this](size_t, const std::string& value, bool, bool, bool) {
			    auto rid = ParseRidFromRow(value);
			    if (!rid)
				    return;

			    m_SelectedRid = rid;
			    if (auto* data = SavedPlayers::GetPlayerData(rid))
			    {
				    if (m_NameInput)
					    m_NameInput->SetValue(data->m_Name);
				    if (m_RidInput)
					    m_RidInput->SetValue(std::to_string(rid));
			    }
		    }));

		// Player Editor (RenderPlayerEditor()) - every row gated on a
		// selection existing, matching the original's own "return early
		// with nothing selected" guard. Deliberately still
		// GridItemConditional here, not watchCondition(): nameInput/
		// ridInput cache their own live-edited text (SetValue()'d from
		// the select callback above, read back by their own change
		// callback), not backed by a Command the way every
		// GridItemCommandToggle/Float/Int/List/String/Button elsewhere
		// is - watchCondition()'s repopulate-on-change would recreate
		// them from their default (empty) constructor value on the very
		// next frame after a selection, discarding whatever the select
		// callback just wrote in. Leaving the reserved-blank-space
		// tradeoff here (see GridItemConditional's own class comment) is
		// safer than a real data-loss bug.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Player Editor", Theme::kText));

		auto nameInput = std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "Name", "", [this](const std::string& value) {
			if (auto* data = SelectedPlayerData())
				data->m_Name = value;
		});
		m_NameInput = nameInput.get();
		items_draft.push_back(std::make_unique<GridItemConditional>(std::move(nameInput), [this] {
			return m_SelectedRid != 0;
		}));

		auto ridInput = std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "Rockstar Id", "", [this](const std::string& value) {
			const auto newRid = std::strtoull(value.c_str(), nullptr, 10);
			if (!newRid || newRid == m_SelectedRid)
				return;

			// Invalidates every existing SavedPlayerData* (see
			// UpdateRockstarId()'s own TODO comment in
			// CommandSavedPlayers.hpp) - m_NameInput/m_RidInput
			// themselves aren't pointers into it, so nothing else here
			// needs re-syncing.
			SavedPlayers::UpdateRockstarId(m_SelectedRid, newRid);
			m_SelectedRid = newRid;
		});
		m_RidInput = ridInput.get();
		items_draft.push_back(std::make_unique<GridItemConditional>(std::move(ridInput), [this] {
			return m_SelectedRid != 0;
		}));

		items_draft.push_back(std::make_unique<GridItemConditional>(
		    std::make_unique<GridItemBoundToggle>(
		        Theme::kContentWidth,
		        kItemH,
		        "Track Player",
		        [this] {
			        auto* data = SelectedPlayerData();
			        return data && data->m_TrackPlayer;
		        },
		        [this](bool value) {
			        if (auto* data = SelectedPlayerData())
				        data->m_TrackPlayer = value;
		        }),
		    [this] {
			    return m_SelectedRid != 0;
		    }));

		// Live session-state readout - the first line covers "nothing
		// selected"/"not fetched yet" for the whole block (matching the
		// original's own single fallback line); the rest just go blank
		// in either case rather than repeating the same message six
		// more times.
		items_draft.push_back(std::make_unique<GridItemLiveText>(
		    Theme::kContentWidth,
		    kItemH,
		    [this] {
			    if (!m_SelectedRid)
				    return std::string();

			    auto* data = SelectedPlayerData();
			    if (!data || !data->m_FetchedData)
				    return std::string("Data not fetched yet");

			    return std::format("Session Type: {}", FetchedPlayerData::GameStateToString(data->m_FetchedData->m_GameState));
		    },
		    Theme::kText));
		items_draft.push_back(std::make_unique<GridItemLiveText>(
		    Theme::kContentWidth,
		    kItemH,
		    [this] {
			    auto* data = SelectedPlayerData();
			    if (!data || !data->m_FetchedData)
				    return std::string();

			    return std::format("Host of Session: {}", data->m_FetchedData->m_HostOfSession ? "Yes" : "No");
		    },
		    Theme::kText));
		items_draft.push_back(std::make_unique<GridItemLiveText>(
		    Theme::kContentWidth,
		    kItemH,
		    [this] {
			    auto* data = SelectedPlayerData();
			    if (!data || !data->m_FetchedData)
				    return std::string();

			    return std::format("Is Spectating: {}", data->m_FetchedData->m_Spectating ? "Yes" : "No");
		    },
		    Theme::kText));
		items_draft.push_back(std::make_unique<GridItemLiveText>(
		    Theme::kContentWidth,
		    kItemH,
		    [this] {
			    auto* data = SelectedPlayerData();
			    if (!data || !data->m_FetchedData)
				    return std::string();

			    return std::format("Is Job Lobby: {}", data->m_FetchedData->m_InTransition ? "Yes" : "No");
		    },
		    Theme::kText));
		items_draft.push_back(std::make_unique<GridItemLiveText>(
		    Theme::kContentWidth,
		    kItemH,
		    [this] {
			    auto* data = SelectedPlayerData();
			    if (!data || !data->m_FetchedData)
				    return std::string();

			    return std::format("Host of Job Lobby: {}", data->m_FetchedData->m_HostOfTransition ? "Yes" : "No");
		    },
		    Theme::kText));
		items_draft.push_back(std::make_unique<GridItemLiveText>(
		    Theme::kContentWidth,
		    kItemH,
		    [this] {
			    auto* data = SelectedPlayerData();
			    if (!data || !data->m_FetchedData || data->m_FetchedData->m_MissionType == FetchedPlayerData::MissionType::NONE)
				    return std::string();

			    return std::format("Mission Type: {}", FetchedPlayerData::MissionTypeToString(data->m_FetchedData->m_MissionType));
		    },
		    Theme::kText));
		items_draft.push_back(std::make_unique<GridItemLiveText>(
		    Theme::kContentWidth,
		    kItemH,
		    [this] {
			    auto* data = SelectedPlayerData();
			    if (!data || !data->m_FetchedData || data->m_FetchedData->m_MissionType == FetchedPlayerData::MissionType::NONE
			        || !data->m_FetchedData->m_MissionName)
				    return std::string();

			    return std::format("Mission Name: {}", *data->m_FetchedData->m_MissionName);
		    },
		    Theme::kText));

		items_draft.push_back(std::make_unique<GridItemConditional>(
		    std::make_unique<GridItemButton>(Theme::kContentWidth,
		        kItemH,
		        "Join",
		        [this] {
			        const auto rid = m_SelectedRid;
			        FiberPool::queueJob([rid] {
				        Network::JoinRockstarId(rid);
			        });
		        }),
		    [this] {
			    return m_SelectedRid != 0;
		    }));
		items_draft.push_back(std::make_unique<GridItemConditional>(
		    std::make_unique<GridItemButton>(Theme::kContentWidth,
		        kItemH,
		        "Save",
		        [] {
			        SavedPlayers::Save();
		        }),
		    [this] {
			    return m_SelectedRid != 0;
		    }));
		items_draft.push_back(std::make_unique<GridItemConditional>(
		    std::make_unique<GridItemButton>(Theme::kContentWidth,
		        kItemH,
		        "Remove",
		        [this] {
			        SavedPlayers::RemovePlayerData(m_SelectedRid);
			        m_SelectedRid = 0;
			        if (m_NameInput)
				        m_NameInput->SetValue("");
			        if (m_RidInput)
				        m_RidInput->SetValue("");
		        }),
		    [this] {
			    return m_SelectedRid != 0;
		    }));

		// New (RenderAddNewPlayer())
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "New", Theme::kText));
		auto addUsernameInput = std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "Username", "", nullptr);
		m_AddUsernameInput = addUsernameInput.get();
		items_draft.push_back(std::move(addUsernameInput));
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Add", [this] {
			if (!m_AddUsernameInput)
				return;

			auto username = m_AddUsernameInput->GetValue();
			FiberPool::queueJob([username] {
				if (auto rid = Network::ResolveRockstarId(username))
					SavedPlayers::AddPlayerData(*rid, username);
				else
					Notifications::Show("Saved Players", "Failed to get RID from username", NotificationType::Error);
			});
		}));

		// Tracking (tracking Group)
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Tracking", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "playerdbautoupdate"_J, "Auto Update"));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "playerdbupdatenow"_J, "Update Now"));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "playerdbnotify"_J, "Tracking Notifications"));

		// Notifications (notifications Group) - gated on playerdbnotify.
		// watchCondition() (not GridItemConditional) is safe here, unlike
		// the Player Editor section above: every one of these is a plain
		// Command-backed GridItemCommandToggle with no cached value of
		// its own to lose on repopulation.
		if (watchCondition("playerdbnotify"_J))
		{
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "playerdbnotifywhenjoinable"_J));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "playerdbnotifywhenunjoinable"_J));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "playerdbnotifywhenonline"_J));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "playerdbnotifywhenoffline"_J));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "playerdbnotifyonseschange"_J));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "playerdbnotifyonmischange"_J));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "playerdbnotifyonjoblobby"_J));
		}
	}
}
