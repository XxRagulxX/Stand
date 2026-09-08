#include "Rendering/SpawnPedGrid.hpp"

#include "Core/Pointers.hpp"
#include "Scripting/FiberPool.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemToggle.hpp"
#include "Util/Joaat.hpp"
#include "Rendering/Notifications.hpp"
#include "Ped/PedCombatAttribute.hpp"
#include "Ped/PedModels.hpp"
#include "Scripting/NativeHooks.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"
#include "Scripting/Scripts.hpp"
#include "Rendering/Theme.hpp"
#include "Weapons/WeaponsHashes.hpp"

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
	SpawnPedGrid::SpawnPedGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SpawnPedGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Model", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemSelectList>(
		    Theme::kContentWidth,
		    "Search",
		    [] {
			    std::vector<std::string> rows;
			    rows.reserve(g_PedModels.size());
			    for (auto name : g_PedModels)
				    rows.push_back(name);
			    return rows;
		    },
		    [this](size_t, const std::string& value, bool ctrl, bool, bool) {
			    const bool setPlayer = ctrl;

			    FiberPool::queueJob([this, value, setPlayer] {
				    if (m_SpawnInMyVehicle && m_SpawnInMyVehicle->GetState())
				    {
					    auto vehicle = Self::GetVehicle();
					    if (vehicle && !vehicle.IsSeatFree(-2) && !vehicle.IsSeatFree(-1))
					    {
						    Notifications::Show("Spawn Ped",
						        "Cannot spawn ped in vehicle, all seats are occupied, please free a seat first or disable 'Spawn In My Vehicle' option.",
						        NotificationType::Warning);
						    return;
					    }
				    }

				    auto hash = Joaat(value);
				    auto handle = Ped::Create(hash, Self::GetPed().GetPosition(), Self::GetPed().GetHeading());

				    if (!handle)
					    return;

				    handle.SetCombatAttribute(PedCombatAttribute::AlwaysFight, true);
				    handle.SetCombatAttribute(PedCombatAttribute::DisableAllRandomsFlee, true);
				    handle.SetCombatAttribute(PedCombatAttribute::DisableFleeFromCombat, true);
				    handle.SetCombatAttribute(PedCombatAttribute::AlwaysFlee, false);
				    handle.SetCombatAttribute(PedCombatAttribute::FleesFromInvincibleOpponents, false);
				    handle.SetCombatAttribute(PedCombatAttribute::CanUseVehicles, true);
				    handle.SetCombatAttribute(PedCombatAttribute::CanLeaveVehicle, true);

				    if (m_Invincible && m_Invincible->GetState())
					    handle.SetInvincible(true);

				    if (m_SpawnDead && m_SpawnDead->GetState())
					    handle.Kill();

				    if (m_SpawnAsBodyguard && m_SpawnAsBodyguard->GetState() && !setPlayer)
				    {
					    handle.SetCombatAttribute(PedCombatAttribute::CanCharge, true);
					    handle.SetCombatAttribute(PedCombatAttribute::CanCommandeerVehicles, true);
					    handle.SetCombatAttribute(PedCombatAttribute::DisableInjuredOnGround, true);
					    handle.SetCombatAttribute(PedCombatAttribute::AllowDogFighting, true);
					    handle.SetCombatAttribute(PedCombatAttribute::PerfectAccuracy, true);
					    handle.SetCombatAttribute(PedCombatAttribute::UseVehicleAttack, true);
					    handle.SetCombatAttribute(PedCombatAttribute::CanDoDrivebys, true);
					    handle.SetCombatAttribute(PedCombatAttribute::CanThrowSmokeGrenade, true);
					    handle.SetCombatAttribute(PedCombatAttribute::CanSeeUnderwaterPeds, true);

					    auto group = Self::GetPlayer().GetGroup();
					    handle.AddToGroup(group);
					    PED::SET_GROUP_SEPARATION_RANGE(group, 9999.9f);
					    PED::SET_PED_CAN_TELEPORT_TO_GROUP_LEADER(handle.GetHandle(), group, true);
					    PED::SET_GROUP_FORMATION_SPACING(group, 10.0f, 0.0f, 0.0f);
				    }

				    if (m_SpawnAsCop && m_SpawnAsCop->GetState())
					    handle.SetAsCop();

				    if (m_SpawnInMyVehicle && m_SpawnInMyVehicle->GetState())
				    {
					    auto vehicle = Self::GetVehicle();
					    if (vehicle)
					    {
						    if (vehicle.IsSeatFree(-1))
							    handle.SetInVehicle(vehicle, -1);
						    else
							    handle.SetInVehicle(vehicle, -2);
					    }
				    }

				    if (m_GiveAllWeapons && m_GiveAllWeapons->GetState())
				    {
					    for (auto weaponHash : g_WeaponHashes)
						    handle.GiveWeapon(weaponHash);
				    }

				    if (m_RandomizeOutfit && m_RandomizeOutfit->GetState())
					    handle.RandomizeOutfit();

				    if (m_BlipPed && m_BlipPed->GetState())
					    HUD::SET_BLIP_COLOUR(HUD::ADD_BLIP_FOR_ENTITY(handle.GetHandle()), 3);

				    if (m_SpawnAsProstitute && m_SpawnAsProstitute->GetState())
				    {
					    handle.StartScenario("WORLD_HUMAN_PROSTITUTE_HIGH_CLASS");
					    handle.SetKeepTask(true);

					    if (!*Pointers.IsSessionStarted)
					    {
						    int ped = handle.GetHandle();
						    Scripts::StartScript("pb_prostitute"_J, eStackSizes::FRIEND, &ped, 1);
					    }
				    }

				    if (setPlayer)
				    {
					    static bool hooked = [] {
						    NativeHooks::AddHook("freemode"_J, NativeIndex::GET_ENTITY_MODEL, [](rage::scrNativeCallContext* ctx) {
							    auto model = ENTITY::GET_ENTITY_MODEL(ctx->GetArg<int>(0));

							    if (ctx->GetArg<int>(0) == Self::GetPed().GetHandle() && (model != "mp_m_freemode_01"_J && model != "mp_f_freemode_01"_J))
								    return ctx->SetReturnValue("mp_m_freemode_01"_J);

							    return ctx->SetReturnValue(model);
						    });
						    for (auto script : {"main"_J, "respawn_controller"_J, "pi_menu"_J})
						    {
							    NativeHooks::AddHook(script, NativeIndex::GET_ENTITY_MODEL, [](rage::scrNativeCallContext* ctx) {
								    auto model = ENTITY::GET_ENTITY_MODEL(ctx->GetArg<int>(0));

								    if (ctx->GetArg<int>(0) == Self::GetPed().GetHandle() && (model != "player_zero"_J && model != "player_one"_J && model != "player_two"_J))
									    return ctx->SetReturnValue("player_zero"_J);

								    return ctx->SetReturnValue(model);
							    });
						    }
						    return true;
					    }();
					    Self::GetPlayer().SetPed(handle);
				    }
				    else
				    {
					    m_SpawnedPeds.push_back(handle);
				    }
			    });
		    }));

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Options", Theme::kText));

		auto pushToggle = [&items_draft](GridItemToggle*& slot, const std::string& label) {
			auto toggle = std::make_unique<GridItemToggle>(Theme::kContentWidth, kItemH, label, false);
			slot = toggle.get();
			items_draft.push_back(std::move(toggle));
		};

		pushToggle(m_Invincible, "Invincible");
		pushToggle(m_SpawnDead, "Spawn Dead");
		pushToggle(m_SpawnAsBodyguard, "Spawn As Bodyguard");
		pushToggle(m_SpawnAsCop, "Spawn As Cop");
		pushToggle(m_SpawnInMyVehicle, "Spawn In My Vehicle");
		pushToggle(m_GiveAllWeapons, "Give All Weapons");
		pushToggle(m_SpawnAsProstitute, "Spawn As Prostitute");
		pushToggle(m_RandomizeOutfit, "Randomize Outfit");
		pushToggle(m_BlipPed, "Blip Ped");

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Remove All", [this] {
			FiberPool::queueJob([this] {
				for (auto& ped : m_SpawnedPeds)
					if (ped)
						ped.Delete();

				m_SpawnedPeds.clear();
			});
		}));
	}
}
