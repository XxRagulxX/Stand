#pragma once
#include "Commands/Self/CommandBodyguardBehaviour.hpp"
#include "Commands/Self/CommandBodyguardModel.hpp"
#include "Commands/Self/CommandBodyguardPresets.hpp"
#include "Commands/Self/CommandBodyguardWeapon.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Menu/Click.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

#include <cmath>

namespace Stand
{
	class CommandBodyguardCount : public CommandSlider
	{
	public:
		explicit CommandBodyguardCount(CommandList* parent)
			: CommandSlider(parent, LIT("Count"), CMDNAMES("bodyguardcount"), NOLABEL, 1, 7, 1)
		{
		}
	};

	class CommandSpawnBodyguards : public CommandPhysical
	{
		CommandBodyguardCount*     m_count;
		CommandBodyguardModel*     m_model;
		CommandBodyguardPrimaryWeapon*   m_primary;
		CommandBodyguardSecondaryWeapon* m_secondary;
		CommandBodyguardBehaviour* m_behaviour;

	public:
		CommandSpawnBodyguards(CommandList* parent, CommandBodyguardCount* count,
			CommandBodyguardModel* model, CommandBodyguardPrimaryWeapon* primary,
			CommandBodyguardSecondaryWeapon* secondary, CommandBodyguardBehaviour* behaviour)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Spawn"), CMDNAMES("spawnbodyguards"), NOLABEL)
			, m_count(count), m_model(model), m_primary(primary), m_secondary(secondary)
			, m_behaviour(behaviour)
		{
		}

		void onClick(Click& click) override
		{
			const int  count        = m_count->value;
			const Hash pedModel     = m_model->GetModel();
			const Hash pedPrimary   = m_primary->GetWeapon();
			const Hash pedSecondary = m_secondary->GetWeapon();
			const int  formation    = m_behaviour->formation->GetFormation();
			const int  aggression   = m_behaviour->aggression->GetAggression();
			const bool exPlayers    = m_behaviour->ignorePlayers->m_on;
			const bool exFriends    = m_behaviour->ignoreFriends->m_on;
			const bool exAuth       = m_behaviour->ignoreAuthorities->m_on;
			const bool exCrew       = m_behaviour->ignoreCrewMembers->m_on;
			const bool exOrg        = m_behaviour->ignoreOrgMembers->m_on;

			FiberPool::queueJob([count, pedModel, pedPrimary, pedSecondary,
				formation, aggression, exPlayers, exFriends, exAuth, exCrew, exOrg] {

				auto ped = Stand::Self::GetPed();
				if (!ped) return;

				STREAMING::REQUEST_MODEL(pedModel);
				int attempts = 0;
				while (!STREAMING::HAS_MODEL_LOADED(pedModel) && attempts++ < 200)
					Script::current()->yield(10);
				if (!STREAMING::HAS_MODEL_LOADED(pedModel)) return;

				const int pedHandle = ped.GetHandle();
				const int group = PLAYER::GET_PLAYER_GROUP(PLAYER::GET_PLAYER_INDEX());
				Vector3 pos = ENTITY::GET_ENTITY_COORDS(pedHandle, true);

				for (int i = 0; i < count; ++i)
				{
					const float angle = static_cast<float>(i) * (6.283185f / static_cast<float>(count));
					int bg = PED::CREATE_PED(4, pedModel,
						pos.x + std::cosf(angle) * 3.0f,
						pos.y + std::sinf(angle) * 3.0f,
						pos.z, 0.0f, true, true);

					Script::current()->yield(10);
					if (!bg) continue;

					PED::SET_PED_AS_GROUP_MEMBER(bg, group);
					PED::SET_PED_NEVER_LEAVES_GROUP(bg, true);
					WEAPON::GIVE_WEAPON_TO_PED(bg, pedPrimary,   9999, false, true);
					WEAPON::GIVE_WEAPON_TO_PED(bg, pedSecondary, 9999, false, false);

					switch (aggression)
					{
					case 0:
						PED::SET_PED_COMBAT_ABILITY(bg, 50);
						PED::SET_PED_COMBAT_ATTRIBUTES(bg, 46, false);
						break;
					case 2:
						PED::SET_PED_COMBAT_ABILITY(bg, 100);
						PED::SET_PED_COMBAT_RANGE(bg, 2);
						PED::SET_PED_COMBAT_ATTRIBUTES(bg, 2, true);
						PED::SET_PED_COMBAT_ATTRIBUTES(bg, 46, true);
						break;
					default:
						PED::SET_PED_COMBAT_ABILITY(bg, 100);
						PED::SET_PED_COMBAT_RANGE(bg, 2);
						PED::SET_PED_COMBAT_ATTRIBUTES(bg, 46, true);
						break;
					}

					PED::SET_PED_FLEE_ATTRIBUTES(bg, 0, false);

					const Hash bgRelGroup = PED::GET_PED_RELATIONSHIP_GROUP_HASH(bg);
					if (exPlayers)    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(1, bgRelGroup, "PLAYER"_J);
					if (exFriends)    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(1, bgRelGroup, "CIVMALE"_J);
					if (exAuth)       PED::SET_RELATIONSHIP_BETWEEN_GROUPS(1, bgRelGroup, "COP"_J);
					if (exCrew)       PED::SET_RELATIONSHIP_BETWEEN_GROUPS(1, bgRelGroup, "PRIVATECREW"_J);
					if (exOrg)        PED::SET_RELATIONSHIP_BETWEEN_GROUPS(1, bgRelGroup, "SECURITY"_J);

					ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&bg);
				}

				if (formation != 0)
					PED::SET_GROUP_FORMATION(group, formation);

				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(pedModel);
			});
		}
	};

	class CommandBodyguardsSpawn : public CommandList
	{
	public:
		CommandBodyguardModel* const model;
		CommandBodyguardPrimaryWeapon* const primary;
		CommandBodyguardSecondaryWeapon* const secondary;
		CommandBodyguardCount* const count;

		explicit CommandBodyguardsSpawn(CommandList* parent, CommandBodyguardBehaviour* behaviour)
			: CommandList(parent, LIT("Spawn"), CMDNAMES("bgspawn"))
			, model(createChild<CommandBodyguardModel>())
			, primary(createChild<CommandBodyguardPrimaryWeapon>())
			, secondary(createChild<CommandBodyguardSecondaryWeapon>())
			, count(createChild<CommandBodyguardCount>())
		{
			createChild<CommandSpawnBodyguards>(count, model, primary, secondary, behaviour);
		}
	};

	class CommandBGImmortality : public CommandToggle
	{
		static void applyToGroup(bool invincible)
		{
			const int group = PLAYER::GET_PLAYER_GROUP(PLAYER::GET_PLAYER_INDEX());
			int count = 0;
			PED::GET_GROUP_SIZE(group, nullptr, &count);
			for (int i = 0; i < count; ++i)
			{
				int member = PED::GET_PED_AS_GROUP_MEMBER(group, i);
				if (member)
					ENTITY::SET_ENTITY_INVINCIBLE(member, invincible, false);
			}
		}
	public:
		explicit CommandBGImmortality(CommandList* parent)
			: CommandToggle(parent, LIT("Immortality"), CMDNAMES("bodyguardimmortality"), NOLABEL)
		{}
		void onEnable(Click& click) override { FiberPool::queueJob([] { applyToGroup(true); }); }
		void onDisable(Click& click) override { FiberPool::queueJob([] { applyToGroup(false); }); }
	};

	class CommandBGTeleportToMe : public CommandPhysical
	{
	public:
		explicit CommandBGTeleportToMe(CommandList* parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Teleport To Me"), CMDNAMES("tpbodyguards"), NOLABEL)
		{}
		void onClick(Click& click) override
		{
			FiberPool::queueJob([] {
				const int player = PLAYER::GET_PLAYER_INDEX();
				const int playerPed = PLAYER::GET_PLAYER_PED(player);
				const int group = PLAYER::GET_PLAYER_GROUP(player);
				Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
				int count = 0;
				PED::GET_GROUP_SIZE(group, nullptr, &count);
				const float step = count > 0 ? (6.283185f / static_cast<float>(count)) : 0.0f;
				for (int i = 0; i < count; ++i)
				{
					int member = PED::GET_PED_AS_GROUP_MEMBER(group, i);
					if (!member) continue;
					const float angle = static_cast<float>(i) * step;
					ENTITY::SET_ENTITY_COORDS(member,
						pos.x + std::cosf(angle) * 2.0f,
						pos.y + std::sinf(angle) * 2.0f,
						pos.z, false, false, false, true);
					Script::current()->yield(10);
				}
			});
		}
	};

	class CommandBGTeleportWhenFar : public CommandToggle
	{
	public:
		explicit CommandBGTeleportWhenFar(CommandList* parent)
			: CommandToggle(parent, LIT("Teleport When Far"), CMDNAMES("bodyguardsteleportwhenfar"), NOLABEL)
		{}
		void onEnable(Click& click) override
		{
			FiberPool::queueJob([this] {
				while (m_on)
				{
					const int player = PLAYER::GET_PLAYER_INDEX();
					const int playerPed = PLAYER::GET_PLAYER_PED(player);
					const int group = PLAYER::GET_PLAYER_GROUP(player);
					Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
					int count = 0;
					PED::GET_GROUP_SIZE(group, nullptr, &count);
					const float step = count > 0 ? (6.283185f / static_cast<float>(count)) : 0.0f;
					for (int i = 0; i < count; ++i)
					{
						int member = PED::GET_PED_AS_GROUP_MEMBER(group, i);
						if (!member) continue;
						Vector3 mPos = ENTITY::GET_ENTITY_COORDS(member, true);
						const float dx = pos.x - mPos.x, dy = pos.y - mPos.y;
						if (dx * dx + dy * dy > 900.0f)
						{
							const float angle = static_cast<float>(i) * step;
							ENTITY::SET_ENTITY_COORDS(member,
								pos.x + std::cosf(angle) * 2.0f,
								pos.y + std::sinf(angle) * 2.0f,
								pos.z, false, false, false, true);
						}
					}
					Script::current()->yield(2000);
				}
			});
		}
	};

	class CommandBGDeleteAll : public CommandPhysical
	{
	public:
		explicit CommandBGDeleteAll(CommandList* parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Delete All Bodyguards"), CMDNAMES("delbodyguards"), NOLABEL)
		{}
		void onClick(Click& click) override
		{
			FiberPool::queueJob([] {
				const int group = PLAYER::GET_PLAYER_GROUP(PLAYER::GET_PLAYER_INDEX());
				int count = 0;
				PED::GET_GROUP_SIZE(group, nullptr, &count);
				std::vector<int> members;
				members.reserve(count);
				for (int i = 0; i < count; ++i)
				{
					int m = PED::GET_PED_AS_GROUP_MEMBER(group, i);
					if (m) members.push_back(m);
				}
				for (int m : members)
				{
					PED::REMOVE_PED_FROM_GROUP(m);
					PED::DELETE_PED(&m);
					Script::current()->yield(10);
				}
			});
		}
	};

	class CommandBodyguards : public CommandList
	{
	public:
		explicit CommandBodyguards(CommandList* parent)
			: CommandList(parent, LIT("Bodyguards"), CMDNAMES("bodyguards"))
		{
			auto* behaviour = createChild<CommandBodyguardBehaviour>();
			auto* spawn = createChild<CommandBodyguardsSpawn>(behaviour);
			createChild<CommandBodyguardPresets>(spawn->model, spawn->primary, spawn->secondary, spawn->count);
			createChild<CommandBGImmortality>();
			createChild<CommandBGTeleportToMe>();
			createChild<CommandBGTeleportWhenFar>();
			createChild<CommandBGDeleteAll>();
		}
	};
}
