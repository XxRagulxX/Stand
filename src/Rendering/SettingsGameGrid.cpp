#include "Rendering/SettingsGameGrid.hpp"

#include "Commands/Settings/Game/CommandTabESP.hpp"
#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		// --- Player ESP sub-grid ---
		class PlayerESPGrid : public Grid
		{
		public:
			PlayerESPGrid() : Grid(Theme::GetContentOrigin(), 0) {}

		protected:
			void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
			{
				auto& esp = Features::GetCommandTabESP();
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.drawPlayers));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.drawDeadPlayers));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.namePlayers));
				AddColorCommandRows(items_draft, Theme::kContentWidth, esp.nameColorPlayers, "Name Colour");
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.distancePlayers));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.skeletonPlayers));
				AddColorCommandRows(items_draft, Theme::kContentWidth, esp.skeletonColorPlayers, "Skeleton Colour");
			}
		};

		// --- Ped ESP sub-grid ---
		class PedESPGrid : public Grid
		{
		public:
			PedESPGrid() : Grid(Theme::GetContentOrigin(), 0) {}

		protected:
			void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
			{
				auto& esp = Features::GetCommandTabESP();
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.drawPeds));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.drawDeadPeds));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.modelPeds));
				AddColorCommandRows(items_draft, Theme::kContentWidth, esp.hashColorPeds, "Hash Colour");
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.netInfoPeds));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.scriptInfoPeds));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.distancePeds));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.skeletonPeds));
				AddColorCommandRows(items_draft, Theme::kContentWidth, esp.skeletonColorPeds, "Skeleton Colour");
			}
		};

		// --- Object ESP sub-grid ---
		class ObjectESPGrid : public Grid
		{
		public:
			ObjectESPGrid() : Grid(Theme::GetContentOrigin(), 0) {}

		protected:
			void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
			{
				auto& esp = Features::GetCommandTabESP();
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.drawObjects));
				AddColorCommandRows(items_draft, Theme::kContentWidth, esp.hashColorObjects, "Hash Colour");
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.netInfoObjects));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.scriptInfoObjects));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, esp.distanceObjects));
			}
		};

		PlayerESPGrid g_PlayerESP{};
		PedESPGrid g_PedESP{};
		ObjectESPGrid g_ObjectESP{};
	}

	SettingsGameGrid::SettingsGameGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsGameGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Player ESP", &g_PlayerESP));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Ped ESP", &g_PedESP));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Object ESP", &g_ObjectESP));
	}
}
