#include "Rendering/InfoGrid.hpp"

#include "Network/CNetGamePlayer.hpp"
#include "Commands/Extra/CommandSavedPlayers.hpp"
#include "Scripting/FiberPool.hpp"
#include "Rendering/Clipboard.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemCommandListSelect.hpp"
#include "Rendering/GridItemCommandPlayer.hpp"
#include "Rendering/GridItemCommandPosition3d.hpp"
#include "Rendering/GridItemLiveText.hpp"
#include "Rendering/GridItemText.hpp"
#include "Util/Joaat.hpp"
#include "Scripting/Natives.hpp"
#include "Network/Players.hpp"
#include "World/Self.hpp"
#include "Rendering/Theme.hpp"

#include <cstdint>
#include <format>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		std::string BuildIPStr(int field1, int field2, int field3, int field4)
		{
			return std::format("{}.{}.{}.{}", field1, field2, field3, field4);
		}

		// Every live-text callback below re-fetches Players::GetSelected()
		// itself rather than capturing it - it needs to react to whoever's
		// selected changing (or disappearing) frame to frame, the same
		// "no populate()-time snapshot" reasoning as GridItemPlayerList
		// (see that class's own comment). GetName()/GetPed()/etc are all
		// non-const Player/Entity methods, so every one of these keeps its
		// own non-const local `selected`/`ped` - never `const auto` - the
		// same MSVC-caught mistake documented on GridItemPlayerList.cpp's
		// own SortedPlayers().
		std::string SelectedName()
		{
			auto selected = Players::GetSelected();
			return selected.IsValid() ? selected.GetName() : "No player selected";
		}

		std::string SelectedRankRP()
		{
			auto selected = Players::GetSelected();
			if (!selected.IsValid())
				return "";

			return std::format("Rank: {} ({} RP)", selected.GetRank(), selected.GetRP());
		}

		std::string SelectedMoney()
		{
			auto selected = Players::GetSelected();
			if (!selected.IsValid())
				return "";

			return std::format("Money: {}", selected.GetMoney());
		}

		std::string SelectedHealth()
		{
			auto selected = Players::GetSelected();
			if (!selected.IsValid())
				return "";

			auto ped = selected.GetPed();
			if (!ped)
				return "Ped missing or deleted";

			auto health = ped.GetHealth();
			auto maxHealth = ped.GetMaxHealth();
			return std::format("HP: {}/{} ({:.2f}%)", health, maxHealth, maxHealth ? (static_cast<float>(health) / static_cast<float>(maxHealth) * 100.0f) : 0.0f);
		}

		std::string SelectedCoords()
		{
			auto selected = Players::GetSelected();
			if (!selected.IsValid())
				return "";

			auto ped = selected.GetPed();
			if (!ped)
				return "";

			auto coords = ped.GetPosition();
			return std::format("Coords: {:.2f}, {:.2f}, {:.2f}", coords.x, coords.y, coords.z);
		}

		std::string SelectedDistance()
		{
			auto selected = Players::GetSelected();
			if (!selected.IsValid())
				return "";

			auto ped = selected.GetPed();
			if (!ped)
				return "";

			auto distance = ped.GetPosition().GetDistance(Self::GetPed().GetPosition());
			return std::format("Distance: {:.2f}", distance);
		}

		std::string SelectedRID()
		{
			auto selected = Players::GetSelected();
			if (!selected.IsValid())
				return "";

			return std::format("RID: {}", selected.GetRID());
		}

		std::string SelectedPlatformId()
		{
			auto selected = Players::GetSelected();
			if (!selected.IsValid())
				return "";

			auto& platformAccountId = selected.GetHandle()->m_PlatformAccountId;
			switch (platformAccountId.m_Platform)
			{
			case PlatformAccountId::PLATFORM_XBOX:
				return std::format("Xbox User ID: {}", platformAccountId.m_XboxUserId);
			case PlatformAccountId::PLATFORM_STEAM:
				return std::format("Steam ID: {}", platformAccountId.m_SteamId);
			case PlatformAccountId::PLATFORM_EPIC:
				return std::format("Epic Account ID: {}", platformAccountId.m_EpicAccountId);
			default:
				return "";
			}
		}

		std::string SelectedIP()
		{
			auto selected = Players::GetSelected();
			if (!selected.IsValid())
				return "";

			auto ip = selected.GetExternalAddress();
			return "IP Address: " + BuildIPStr(ip.m_IpAddress.m_Field1, ip.m_IpAddress.m_Field2, ip.m_IpAddress.m_Field3, ip.m_IpAddress.m_Field4);
		}

		std::string SelectedLatency()
		{
			auto selected = Players::GetSelected();
			if (!selected.IsValid())
				return "";

			return std::format("Latency: {:.2f} | Packet Loss: {:.2f}%", selected.GetAverageLatency(), selected.GetAveragePacketLoss());
		}

		// The plain-value part of the RID/platform-ID/IP live rows above,
		// re-extracted so the "Copy" buttons next to them copy the exact
		// same value shown rather than needing their own second lookup
		// each getting out of sync with the other.
		std::string PlainRID()
		{
			auto selected = Players::GetSelected();
			return selected.IsValid() ? std::to_string(selected.GetRID()) : "";
		}

		std::string PlainPlatformId()
		{
			auto selected = Players::GetSelected();
			if (!selected.IsValid())
				return "";

			auto& platformAccountId = selected.GetHandle()->m_PlatformAccountId;
			switch (platformAccountId.m_Platform)
			{
			case PlatformAccountId::PLATFORM_XBOX:
				return std::to_string(platformAccountId.m_XboxUserId);
			case PlatformAccountId::PLATFORM_STEAM:
				return std::to_string(platformAccountId.m_SteamId);
			case PlatformAccountId::PLATFORM_EPIC:
				return platformAccountId.m_EpicAccountId;
			default:
				return "";
			}
		}

		std::string PlainIP()
		{
			auto selected = Players::GetSelected();
			if (!selected.IsValid())
				return "";

			auto ip = selected.GetExternalAddress();
			return BuildIPStr(ip.m_IpAddress.m_Field1, ip.m_IpAddress.m_Field2, ip.m_IpAddress.m_Field3, ip.m_IpAddress.m_Field4);
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
	InfoGrid::InfoGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void InfoGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Info (Info.cpp's own "Info" Group) - a live readout of whoever's
		// selected, plus its action buttons. spectate isn't repeated here:
		// PlayersGrid already shows that one unconditionally at the root
		// Players page, matching MenuPlayers.cpp's own DrawPlayerList()
		// (Info.cpp's own copy of the same toggle would just be a second,
		// redundant control for the same CommandToggle). No section header
		// of its own - one repeating "Info" while already inside the
		// folder labeled "Info" (PlayersGrid's own row leading here) is a
		// redundant label, not a distinguishing one - see SelfGrid.cpp's
		// identical comment on its own former "Self" header.
		items_draft.push_back(std::make_unique<GridItemLiveText>(Theme::kContentWidth, kItemH, SelectedName, Theme::kText));
		items_draft.push_back(std::make_unique<GridItemLiveText>(Theme::kContentWidth, kItemH, SelectedRankRP, Theme::kText));
		items_draft.push_back(std::make_unique<GridItemLiveText>(Theme::kContentWidth, kItemH, SelectedMoney, Theme::kText));
		items_draft.push_back(std::make_unique<GridItemLiveText>(Theme::kContentWidth, kItemH, SelectedHealth, Theme::kText));
		items_draft.push_back(std::make_unique<GridItemLiveText>(Theme::kContentWidth, kItemH, SelectedCoords, Theme::kText));
		items_draft.push_back(std::make_unique<GridItemLiveText>(Theme::kContentWidth, kItemH, SelectedDistance, Theme::kText));

		// RID/platform ID/IP each get their own live-value row plus a
		// "Copy" button underneath it - Info.cpp's own equivalent packed
		// the value itself into an ImGui::SmallButton's label so clicking
		// the visible number copied it; this system's one-GridItem-per-
		// row buttons always show a fixed label (see GridItemButton's own
		// class comment), so the value and the button that copies it are
		// two rows here instead of one.
		items_draft.push_back(std::make_unique<GridItemLiveText>(Theme::kContentWidth, kItemH, SelectedRID, Theme::kText));
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Copy RID", [] {
			Clipboard::SetText(PlainRID());
		}));
		items_draft.push_back(std::make_unique<GridItemLiveText>(Theme::kContentWidth, kItemH, SelectedPlatformId, Theme::kText));
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Copy Platform ID", [] {
			Clipboard::SetText(PlainPlatformId());
		}));
		items_draft.push_back(std::make_unique<GridItemLiveText>(Theme::kContentWidth, kItemH, SelectedIP, Theme::kText));
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Copy IP Address", [] {
			Clipboard::SetText(PlainIP());
		}));

		// Folds in "More Info"'s own latency/packet-loss readout directly
		// rather than porting a second, nested popup just for two more
		// lines of live text - MenuPopup (see its own class comment) is
		// built for a Yes/No confirm, not an arbitrary content panel, and
		// this doesn't need one.
		items_draft.push_back(std::make_unique<GridItemLiveText>(Theme::kContentWidth, kItemH, SelectedLatency, Theme::kText));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Add to Saved", [] {
			auto selected = Players::GetSelected();
			if (selected.IsValid())
				SavedPlayers::GetPlayerData(selected);
		}));
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "View SC Profile", [] {
			FiberPool::queueJob([] {
				auto selected = Players::GetSelected();
				if (!selected.IsValid())
					return;

				uint64_t handle[13];
				NETWORK::NETWORK_HANDLE_FROM_PLAYER(selected.GetId(), handle, std::size(handle));
				NETWORK::NETWORK_SHOW_PROFILE_UI(handle);
			});
		}));
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Add Friend", [] {
			FiberPool::queueJob([] {
				auto selected = Players::GetSelected();
				if (!selected.IsValid())
					return;

				uint64_t handle[13];
				NETWORK::NETWORK_HANDLE_FROM_PLAYER(selected.GetId(), handle, std::size(handle));
				NETWORK::NETWORK_ADD_FRIEND(handle, "");
			});
		}));

		// Teleport (Info.cpp's own "Teleport" Group) - every row here
		// acts on whoever's selected, same as GridItemCommandPlayer's own
		// class comment describes.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Teleport", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "sendtopropertyindex"_J, "Property"));
		items_draft.push_back(std::make_unique<GridItemCommandPlayer>(Theme::kContentWidth, kItemH, "sendtoproperty"_J));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "sendtointeriorindex"_J, "Interior"));
		items_draft.push_back(std::make_unique<GridItemCommandPlayer>(Theme::kContentWidth, kItemH, "sendtointerior"_J));
		items_draft.push_back(std::make_unique<GridItemCommandPlayer>(Theme::kContentWidth, kItemH, "tptoplayer"_J, "Teleport To"));
		items_draft.push_back(std::make_unique<GridItemCommandPlayer>(Theme::kContentWidth, kItemH, "bring"_J));

		AddVector3CommandRows(*this, items_draft, Theme::kContentWidth, "playertpcoord"_J);
		items_draft.push_back(std::make_unique<GridItemCommandPlayer>(Theme::kContentWidth, kItemH, "tpplayertocoord"_J, "Teleport"));
	}
}
