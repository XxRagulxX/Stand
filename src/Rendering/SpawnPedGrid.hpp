#pragma once
#include "Rendering/Grid.hpp"
#include "Ped/Ped.hpp"

#include <vector>

namespace Stand::Rendering
{
	class GridItemToggle;

	// Content grid for World > Spawn Ped: mirrors SpawnPed.cpp's
	// BuildSpawnPedMenu() - a searchable ped-model picker (g_PedModels,
	// PedModels.hpp) plus the same nine spawn-option toggles and a
	// "Remove All" button for every ped spawned this way.
	//
	// The original's Ctrl+Click ("set as player model" instead of
	// spawning) becomes Left Ctrl + Left/Right on the picker - see
	// GridItemSelectList's own class comment for why a keyboard move can
	// carry live Ctrl state now. Every other option (invincible, spawn
	// dead, bodyguard, cop, in-my-vehicle, give weapons, prostitute,
	// randomize outfit, blip) is a local GridItemToggle, not backed by a
	// real Command - same as the original's own function-local statics,
	// just held here instead since populate() only runs once.
	class SpawnPedGrid : public Grid
	{
	public:
		SpawnPedGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		GridItemToggle* m_Invincible = nullptr;
		GridItemToggle* m_SpawnDead = nullptr;
		GridItemToggle* m_SpawnAsBodyguard = nullptr;
		GridItemToggle* m_SpawnAsCop = nullptr;
		GridItemToggle* m_SpawnInMyVehicle = nullptr;
		GridItemToggle* m_GiveAllWeapons = nullptr;
		GridItemToggle* m_SpawnAsProstitute = nullptr;
		GridItemToggle* m_RandomizeOutfit = nullptr;
		GridItemToggle* m_BlipPed = nullptr;

		std::vector<Ped> m_SpawnedPeds;
	};
}
