#pragma once
#include "Rendering/Grid.hpp"
#include "Scripting/ScriptPatches.hpp"

#include <string>
#include <vector>

struct GSBD_RandomEvents;
struct GPBD_FM_2;
struct RANDOM_EVENTS_FREEMODE_DATA;

namespace Stand::Rendering
{
	// Content grid for Network > Random Events: mirrors MenuRandomEvents.cpp's
	// BuildRandomEventsMenu() in full - a single "Triggerer/inspector" page
	// for one of 24 fixed freemode random events at a time (Select Event),
	// showing its live server-side state and offering Launch/Kill/Teleport
	// actions plus a per-event cooldown/availability override, exactly the
	// one Group the original has (there's no sub-navigation here at all).
	//
	// Select Event uses GridItemSelectList the same way TransactionsGrid's
	// own Category/Action pickers do - 24 fixed named options, same shape
	// as those 28/60-entry arrays, just sourced from World/RandomEvents.hpp
	// (a real shared header with external linkage, unlike Transactions.cpp/
	// VehicleEditor.cpp's own file-static tables - included directly here
	// rather than duplicated, same as this project already does for e.g.
	// Network/CNetShopTransaction.hpp). Every other file-static helper the
	// original's own .cpp has (GetEventStateString/GetNumLocallyActiveEvents/
	// ResetEventTunables/OnComboChange/KillActiveEvent, and the hardcoded
	// ScriptFunction/ScriptPointer signatures inside them) is still mirrored
	// verbatim as this class's own private methods, same "mirror the
	// original's logic, don't reuse it" approach as every other port in
	// this series.
	//
	// Every native/global-state read the original's own ImGuiItem lambda
	// makes every frame (GPBD_FM_2::Get()/GSBD_RandomEvents::Get()/
	// RANDOM_EVENTS_FREEMODE_DATA::Get(), each event's own live State) is
	// re-read fresh inside its own watchCondition() predicate or fresh
	// local lookup at the top of populate() rather than cached on this
	// class - see populate()'s own comment for why (the same staleness
	// hazard SyncThread()/SyncVehicle() exist to avoid elsewhere, just
	// solved here by never caching the pointers across a populate() call
	// instead of needing a dedicated Sync method, since nothing here reads
	// them from draw()/drawText()/findItemAt() directly).
	//
	// selectedEvent/selectedSubvariation/setCooldown/setAvailability/
	// applyInMinutes were file-static locals in the original (persisting
	// across menu open/closes, shared by every ImGuiItem frame) - this
	// class's own instance members serve the exact same role, since this
	// Grid instance is itself the same kind of long-lived singleton (see
	// NetworkGrid.cpp's note on why content Grids are owned as file-scope
	// statics).
	class RandomEventsGrid : public Grid
	{
	public:
		RandomEventsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		// Mirrors GetEventStateString().
		std::string GetEventStateString(GSBD_RandomEvents* gsbd, RANDOM_EVENTS_FREEMODE_DATA* fm, int event) const;

		// Mirrors GetNumLocallyActiveEvents().
		int GetNumLocallyActiveEvents(GPBD_FM_2* gpbd) const;

		// Mirrors ResetEventTunables().
		void ResetEventTunables(int event);

		// Mirrors OnComboChange() - called from inside the same FiberPool
		// job that assigns m_SelectedEvent (see populate()'s own Select
		// Event row), not a separately-queued one, same as the original's
		// own OnComboChange() being called directly inside
		// BuildRandomEventsMenu()'s own queueJob([event]{ selectedEvent =
		// event; OnComboChange(); }).
		void OnEventChanged();

		// Mirrors KillActiveEvent().
		void KillActiveEvent();

		// Mirrors BuildRandomEventsMenu()'s own one-time
		// sendUpdateRECoordsTSECooldownPatches setup - built and enabled
		// once, the first time populate() runs (which only ever happens
		// once per this Grid's own lifetime - see Grid.hpp's own class
		// comment), same "once at menu-build time" moment the original's
		// own call site (MenuNetwork.cpp calling BuildRandomEventsMenu()
		// once at startup) already ran this at.
		void EnsureCooldownPatchesEnabled();

		int m_SelectedEvent = 0;
		int m_SelectedSubvariation = 0;
		int m_NumSubvariations = 29;
		int m_SetCooldown = 1800000;
		int m_SetAvailability = 900000;
		bool m_ApplyInMinutes = false;

		bool m_PatchesEnabled = false;
		std::vector<ScriptPatch> m_TSECooldownPatches;
	};
}
