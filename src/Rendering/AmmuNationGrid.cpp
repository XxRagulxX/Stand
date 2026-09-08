#include "Rendering/AmmuNationGrid.hpp"

#include "Scripting/FiberPool.hpp"
#include "Rendering/Grid.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemLiveText.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Util/Joaat.hpp"
#include "Scripting/Natives.hpp"
#include "Core/Pointers.hpp"
#include "Scripting/Script.hpp"
#include "Scripting/ScriptFunction.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Scripting/Scripts.hpp"
#include "World/Self.hpp"
#include "Rendering/Theme.hpp"
#include "Weapons/WeaponsHashes.hpp"
#include "Scripting/scrThread.hpp"

#include <algorithm>
#include <format>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		struct WeaponDisplay
		{
			std::string name;
			std::string desc;
			joaat_t hash;
		};

		// RenderAmmuNationMenu()'s own weaponDisplays cache - built once
		// via FiberPool (the mp_weapons script calls below only work on
		// the game's own fiber), same "static bool init = [] {
		// FiberPool::queueJob(...); return true; }()" pattern
		// VehicleSpawnNewGrid's EnsureVehicleNamesLoaded() uses, just
		// waiting for the "startup" script to finish first, matching the
		// original exactly.
		std::vector<WeaponDisplay> g_WeaponDisplays;

		void EnsureWeaponsLoaded()
		{
			static bool started = [] {
				FiberPool::queueJob([] {
					while (Scripts::IsScriptActive("startup"_J))
						Script::current()->yield();

					uint64_t garbage[4]{};
					if (auto id = Scripts::StartScript("mp_weapons"_J, eStackSizes::PAUSE_MENU_SCRIPT, &garbage, 4))
					{
						if (auto thread = Scripts::FindScriptThreadByID(id))
						{
							thread->m_Context.m_State = rage::scrThread::State::PAUSED;

							for (const auto& weap : g_WeaponHashes)
							{
								static ScriptFunction getWeaponNameLabel("mp_weapons"_J, ScriptPointer("GetWeaponNameLabel", "2D 02 2B 00 00"));
								static ScriptFunction getWeaponDescLabel("mp_weapons"_J, ScriptPointer("GetWeaponDescLabel", "2D 02 A0 00 00"));

								std::string nameGxt = getWeaponNameLabel.Call<const char*>(weap, false);
								std::string descGxt = getWeaponDescLabel.Call<const char*>(weap, false);

								std::string nameDisplay = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(nameGxt.c_str());
								std::string descDisplay = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(descGxt.c_str());

								g_WeaponDisplays.push_back({(nameDisplay.empty() || nameDisplay == "NULL" || nameDisplay == "Invalid") ? "" : nameDisplay,
								    (descDisplay.empty() || descDisplay == "NULL" || descDisplay == "Invalid") ? "" : descDisplay,
								    weap});
							}

							thread->Kill();
							thread->m_Context.m_State = rage::scrThread::State::KILLED;
						}
					}
				});
				return true;
			}();
		}

		// FetchWeaponStats() - same shape as the original, moved here
		// since it was private to Weapons.cpp.
		void FetchWeaponStats(joaat_t weaponHash, int& kills, int& deaths, float& kd, int& headshots, int& accuracy)
		{
			uint64_t garbage[4]{};
			if (auto id = Scripts::StartScript("mp_weapons"_J, eStackSizes::PAUSE_MENU_SCRIPT, &garbage, 4))
			{
				if (auto thread = Scripts::FindScriptThreadByID(id))
				{
					thread->m_Context.m_State = rage::scrThread::State::PAUSED;

					static ScriptFunction getWeaponKills("mp_weapons"_J, ScriptPointer("GetWeaponKills", "5D ? ? ? 39 0F 38 00").Add(1).Rip());
					static ScriptFunction getWeaponDeaths("mp_weapons"_J, ScriptPointer("GetWeaponDeaths", "5D ? ? ? 39 10").Add(1).Rip());
					static ScriptFunction getWeaponKDRatio("mp_weapons"_J, ScriptPointer("GetWeaponKDRatio", "5D ? ? ? 39 12").Add(1).Rip());
					static ScriptFunction getWeaponHeadshots("mp_weapons"_J, ScriptPointer("GetWeaponHeadshots", "5D ? ? ? 39 11").Add(1).Rip());
					static ScriptFunction getWeaponAccuracy("mp_weapons"_J, ScriptPointer("GetWeaponAccuracy", "2D 01 09 00 00"));

					kills = getWeaponKills.Call<int>(weaponHash, -1);
					deaths = getWeaponDeaths.Call<int>(weaponHash, -1);
					kd = getWeaponKDRatio.Call<float>(weaponHash, -1);
					headshots = getWeaponHeadshots.Call<int>(weaponHash, -1);
					accuracy = static_cast<int>(getWeaponAccuracy.Call<float>(weaponHash));

					thread->Kill();
					thread->m_Context.m_State = rage::scrThread::State::KILLED;
				}
			}
		}

		joaat_t g_SelectedWeaponHash{};
		std::string g_SelectedWeaponDesc;
		int g_Kills{}, g_Deaths{}, g_Headshots{}, g_Accuracy{};
		float g_KdRatio{};
	}

	void AddAmmuNationRows(Grid& grid, std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width)
	{
		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    width,
		    "Search weapons",
		    [] {
			    EnsureWeaponsLoaded();

			    std::vector<std::string> rows;
			    rows.reserve(g_WeaponDisplays.size());
			    for (auto& weapon : g_WeaponDisplays)
				    if (!weapon.name.empty())
					    rows.push_back(weapon.name);
			    return rows;
		    },
		    [](size_t, const std::string& value, bool, bool, bool) {
			    for (auto& weapon : g_WeaponDisplays)
			    {
				    if (weapon.name == value)
				    {
					    g_SelectedWeaponHash = weapon.hash;
					    g_SelectedWeaponDesc = weapon.desc;

					    FiberPool::queueJob([] {
						    FetchWeaponStats(g_SelectedWeaponHash, g_Kills, g_Deaths, g_KdRatio, g_Headshots, g_Accuracy);
					    });
					    break;
				    }
			    }
		    }));

		// Stands in for the original's own hover tooltip - see this
		// function's own doc comment in the header for why.
		items_draft.push_back(std::make_unique<GridItemLiveText>(
		    width,
		    kItemH,
		    [] {
			    return g_SelectedWeaponDesc.empty() ? std::string() : std::format("Description: {}", g_SelectedWeaponDesc);
		    },
		    Theme::kText));

		items_draft.push_back(std::make_unique<GridItemButton>(width, kItemH, "Give Weapon", [] {
			FiberPool::queueJob([] {
				Self::GetPed().GiveWeapon(g_SelectedWeaponHash, true);
			});
		}));
		items_draft.push_back(std::make_unique<GridItemButton>(width, kItemH, "Remove Weapon", [] {
			FiberPool::queueJob([] {
				Self::GetPed().RemoveWeapon(g_SelectedWeaponHash);
			});
		}));

		auto hasStats = [] {
			return *Pointers.IsSessionStarted && g_SelectedWeaponHash != 0;
		};

		// watchCondition() (not GridItemConditional) so these five rows
		// don't reserve their own layout slot while hidden - see
		// Grid::watchCondition()'s own doc comment.
		if (grid.watchCondition(hasStats))
		{
			items_draft.push_back(std::make_unique<GridItemLiveText>(
			    width,
			    kItemH,
			    [] {
				    return std::format("Kills With: {}", g_Kills);
			    },
			    Theme::kText));
			items_draft.push_back(std::make_unique<GridItemLiveText>(
			    width,
			    kItemH,
			    [] {
				    return std::format("Deaths By: {}", g_Deaths);
			    },
			    Theme::kText));
			items_draft.push_back(std::make_unique<GridItemLiveText>(
			    width,
			    kItemH,
			    [] {
				    return std::format("K/D Ratio: {:.2f}", g_KdRatio);
			    },
			    Theme::kText));
			items_draft.push_back(std::make_unique<GridItemLiveText>(
			    width,
			    kItemH,
			    [] {
				    return std::format("Headshots: {}", g_Headshots);
			    },
			    Theme::kText));
			items_draft.push_back(std::make_unique<GridItemLiveText>(
			    width,
			    kItemH,
			    [] {
				    return std::format("Accuracy: {}%", g_Accuracy);
			    },
			    Theme::kText));
		}
	}
}
