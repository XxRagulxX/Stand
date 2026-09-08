#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Network > Session: mirrors MenuNetwork.cpp's
	// joinGroup (the joinSession subgroup only), bountyGroup,
	// trollGroup, miscGroup and enhancements - the working subset of
	// "Session", wired the same way SelfGrid/VehicleGrid wire their own
	// unconditional items.
	//
	// Deliberately NOT included, and for a reason distinct from every
	// other content Grid so far: killall/killexploitall/explodeall/
	// ceokickall/hkickall/setbountyall/sendsextall/deletevehall/
	// sendtopropertyall/sendtointeriorall/bringall - this whole "act on
	// every player" family isn't registered as a Command anywhere in
	// this codebase (Commands::GetCommand<T> would return nullptr for
	// every one of them, same as it already does in the existing ImGui
	// Network > Session category - MenuNetwork.cpp's own leading
	// "TODO: this needs a rework" comment is the tell). Not a limitation
	// of this Grid/GridItem system, so nothing here is skipped that
	// could otherwise work: porting a button wired to a command that
	// doesn't exist would just be a second copy of the same "Unknown!"
	// button the ImGui menu already shows, not a widening of what's
	// actually usable, so it's better to leave it out until those
	// commands exist. This also means the whole toxicGroup and
	// teleportGroup are skipped entirely - toxicGroup because every one
	// of its five items is one of these, teleportGroup because both its
	// tpToProperty/tpToInterior subgroups pair a real list selector with
	// one of these dead "send all" buttons (a selector with nothing that
	// acts on it isn't worth including on its own), on top of bringall
	// itself and customPlayerTp (tpplayertocoordall, also one of these
	// dead "act on everyone" commands - the Vector3 widget itself is no
	// longer the blocker, see GridItemCommandPosition3d).
	//
	// Also NOT included, for the usual reasons:
	// - joinGroup's raw ImGuiItem (a free-text username/RID joiner with
	//   its own two buttons) - GridItemTextInput exists now, but nothing
	//   here composes it with the "Join"/"Cancel" buttons that ImGuiItem
	//   draws alongside it yet.
	// All of the above stay reachable via the existing ImGui Network >
	// Session category, untouched. Populate() also appends a
	// GridItemFolder row for each of Network's other categories
	// (Spoofing, Saved Players, Random Events), each pointing at a real
	// content Grid (NetworkSpoofingGrid/SavedPlayersGrid/
	// RandomEventsGrid, owned here since nothing else reaches them).
	class NetworkGrid : public Grid
	{
	public:
		NetworkGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
