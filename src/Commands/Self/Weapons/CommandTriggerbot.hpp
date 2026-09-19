#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/Pools.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

#include <cmath>
#include <set>

namespace Stand
{
	class CommandTriggerbotToggle : public CommandToggle
	{
	public:
		CommandToggle* players{};
		CommandToggle* peds{};
		CommandToggle* vehicles{};
		CommandSlider* range{};
		CommandToggle* excludeFriends{};
		CommandToggle* excludeCrewMembers{};
		CommandToggle* excludeStandUsers{};
		CommandToggle* excludeOrgMembers{};
		CommandToggle* magicBullets{};

	private:
		bool IsPlayerExcluded(int pedHandle)
		{
			if (!PED::IS_PED_A_PLAYER(pedHandle))
				return false;

			const int playerId = NETWORK::NETWORK_GET_PLAYER_INDEX_FROM_PED(pedHandle);
			if (playerId < 0 || playerId > 31)
				return false;

			if (excludeFriends && excludeFriends->m_on)
			{
				if (NETWORK::NETWORK_IS_FRIEND_IN_SAME_TITLE(PLAYER::GET_PLAYER_NAME(playerId)))
					return true;
			}

			if (excludeOrgMembers && excludeOrgMembers->m_on)
			{
				const int localId   = Stand::Self::GetPlayer().GetId();
				const int theirTeam = PLAYER::GET_PLAYER_TEAM(playerId);
				const int localTeam = PLAYER::GET_PLAYER_TEAM(localId);
				if (theirTeam != -1 && theirTeam == localTeam)
					return true;
			}

			return false;
		}

	public:
		explicit CommandTriggerbotToggle(CommandList* parent)
			: CommandToggle(parent, LIT("Triggerbot"), CMDNAMES("triggerbot"),
			                LIT("Automatically fires when your crosshair is over a valid target."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			auto selfPed = Stand::Self::GetPed();
			if (!selfPed)
				return;

			const int selfHandle = selfPed.GetHandle();
			const int localId    = Stand::Self::GetPlayer().GetId();

			if (!PLAYER::IS_PLAYER_FREE_AIMING(localId))
				return;

			int target = 0;
			if (!PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(localId, &target) || target == 0)
				return;

			if (!ENTITY::DOES_ENTITY_EXIST(target) || ENTITY::IS_ENTITY_DEAD(target, false))
				return;

			const int entityType = ENTITY::GET_ENTITY_TYPE(target);
			const bool isPed     = (entityType == 1);
			const bool isVehicle = (entityType == 2);

			if (isPed)
			{
				if (target == selfHandle)
					return;

				const bool isPlayer = PED::IS_PED_A_PLAYER(target);

				if (isPlayer && !(players && players->m_on))
					return;
				if (!isPlayer && !(peds && peds->m_on))
					return;
				if (isPlayer && IsPlayerExcluded(target))
					return;
			}
			else if (isVehicle)
			{
				if (!(vehicles && vehicles->m_on))
					return;
				if (VEHICLE::IS_VEHICLE_SEAT_FREE(target, -1, true))
					return;
			}
			else
			{
				return;
			}

			if (range)
			{
				const Vector3 selfPos   = ENTITY::GET_ENTITY_COORDS(selfHandle, true);
				const Vector3 targetPos = ENTITY::GET_ENTITY_COORDS(target, true);
				const float dx  = targetPos.x - selfPos.x;
				const float dy  = targetPos.y - selfPos.y;
				const float dz  = targetPos.z - selfPos.z;
				const float dist = std::sqrtf(dx*dx + dy*dy + dz*dz);
				if (dist > static_cast<float>(range->value))
					return;
			}

			if (magicBullets && magicBullets->m_on)
			{
				const Hash weaponHash = WEAPON::GET_SELECTED_PED_WEAPON(selfHandle);
				const Vector3 camPos  = CAMERA::GET_GAMEPLAY_CAM_COORD();
				Vector3 targetPos;
				if (isPed)
					targetPos = PED::GET_PED_BONE_COORDS(target, 31086, 0.f, 0.f, 0.f);
				else
					targetPos = ENTITY::GET_ENTITY_COORDS(target, true);

				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(
					camPos.x, camPos.y, camPos.z,
					targetPos.x, targetPos.y, targetPos.z,
					100, false, weaponHash, selfHandle,
					true, false, -1.0f);
			}
			else
			{
				PAD::SET_CONTROL_VALUE_NEXT_FRAME(0, 24, 1.0f);
			}
		}

		~CommandTriggerbotToggle() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};

	class CommandTriggerbot : public CommandList
	{
	public:
		CommandTriggerbotToggle* const toggle;
		CommandToggle*           const magicBullets;
		CommandList*             const targetSelection;
		CommandToggle*           const players;
		CommandToggle*           const peds;
		CommandToggle*           const vehicles;
		CommandSlider*           const range;
		CommandToggle*           const excludeFriends;
		CommandToggle*           const excludeCrewMembers;
		CommandToggle*           const excludeStandUsers;
		CommandToggle*           const excludeOrgMembers;

		explicit CommandTriggerbot(CommandList* parent)
			: CommandList(parent, LIT("Triggerbot"), CMDNAMES("triggerbotlist")),
			  toggle(createChild<CommandTriggerbotToggle>()),
			  magicBullets(createChild<CommandToggle>(
			      LIT("Magic Bullets"), CMDNAMES("magicbullets"),
			      LIT("Makes sure the triggerbot never misses."))),
			  targetSelection(createChild<CommandList>(LIT("Target Selection"), CMDNAMES("triggerbottargets"))),
			  players(targetSelection->createChild<CommandToggle>(
			      LIT("Players"), CMDNAMES("triggerbotplayers"), NOLABEL, true)),
			  peds(targetSelection->createChild<CommandToggle>(
			      LIT("Pedestrians"), CMDNAMES("triggerbotpeds"), NOLABEL, false)),
			  vehicles(targetSelection->createChild<CommandToggle>(
			      LIT("Vehicles"), CMDNAMES("triggerbotvehicles"), NOLABEL, false)),
			  range(targetSelection->createChild<CommandSlider>(
			      LIT("Distance"), CMDNAMES("triggerbotrange"),
			      LIT("Maximum trigger distance."),
			      0, 30000, 30000, 100)),
			  excludeFriends(targetSelection->createChild<CommandToggle>(
			      LIT("Exclude Friends"), CMDNAMES("triggerbotexcludefriends"), NOLABEL)),
			  excludeCrewMembers(targetSelection->createChild<CommandToggle>(
			      LIT("Exclude Crew Members"), CMDNAMES("triggerbotexcludecrewmembers"), NOLABEL)),
			  excludeStandUsers(targetSelection->createChild<CommandToggle>(
			      LIT("Exclude Stand Users"), CMDNAMES("triggerbotexcludestandusers"), NOLABEL)),
			  excludeOrgMembers(targetSelection->createChild<CommandToggle>(
			      LIT("Exclude Organisation Members"), CMDNAMES("triggerbotexcludeorgmembers"), NOLABEL))
		{
			toggle->magicBullets     = magicBullets;
			toggle->players          = players;
			toggle->peds             = peds;
			toggle->vehicles         = vehicles;
			toggle->range            = range;
			toggle->excludeFriends   = excludeFriends;
			toggle->excludeCrewMembers = excludeCrewMembers;
			toggle->excludeStandUsers  = excludeStandUsers;
			toggle->excludeOrgMembers  = excludeOrgMembers;
		}
	};
}
