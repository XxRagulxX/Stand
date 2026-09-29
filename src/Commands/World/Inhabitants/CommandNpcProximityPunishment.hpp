#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Game/Pools.hpp"
#include "Game/Punishments.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

#include <cmath>

namespace Stand
{
    class CommandNpcProximityPunishment : public CommandToggle
    {
        const punishment_t mask;

    public:
        explicit CommandNpcProximityPunishment(CommandList* parent, const Punishment& p)
            : CommandToggle(parent, Label(p.name), {}, Label(p.help_text))
            , mask(p.mask)
        {}

        void onEnable(Click& click) final
        {
            AllEntitiesEveryTick::npc_proximity_punishments |= mask;
            CommandTickDispatch::AddCommand(this);
        }

        void onDisable(Click& click) final
        {
            AllEntitiesEveryTick::npc_proximity_punishments &= ~mask;
            CommandTickDispatch::RemoveCommand(this);
            if (mask == PUNISHMENT_FREEZE)
            {
                click.ensureScriptThread([] {
                    for (auto ped : Pools::GetPeds())
                        if (ped && !ped.IsPlayer())
                            ENTITY::FREEZE_ENTITY_POSITION(ped.GetHandle(), FALSE);
                });
            }
        }

        void onTick() final
        {
            int self_h = Self::GetPed().GetHandle();
            Vector3 player_pos = ENTITY::GET_ENTITY_COORDS(self_h, TRUE);
            float limit = AllEntitiesEveryTick::npc_punishable_proximity;
            float limit_sq = limit * limit;

            for (auto ped : Pools::GetPeds())
            {
                if (!ped || ped.IsPlayer()) continue;
                int h = ped.GetHandle();

                Vector3 ped_pos = ENTITY::GET_ENTITY_COORDS(h, TRUE);
                float dx = player_pos.x - ped_pos.x;
                float dy = player_pos.y - ped_pos.y;
                float dz = player_pos.z - ped_pos.z;
                if (dx * dx + dy * dy + dz * dz > limit_sq) continue;

                bool dead = PED::IS_PED_DEAD_OR_DYING(h, TRUE);

                switch (mask)
                {
                case PUNISHMENT_EXPLODE_ANON:
                    if (dead) break;
                    FIRE::ADD_EXPLOSION(ped_pos.x, ped_pos.y, ped_pos.z, 2, 1.f, TRUE, FALSE, 1.f, FALSE);
                    break;
                case PUNISHMENT_EXPLODE_OWNED:
                    if (dead) break;
                    FIRE::ADD_OWNED_EXPLOSION(self_h, ped_pos.x, ped_pos.y, ped_pos.z, 2, 1.f, TRUE, FALSE, 1.f);
                    break;
                case PUNISHMENT_BURN:
                    if (dead) break;
                    FIRE::START_ENTITY_FIRE(h);
                    break;
                case PUNISHMENT_DIE:
                    if (dead) break;
                    ENTITY::SET_ENTITY_HEALTH(h, 0, 0, 0);
                    break;
                case PUNISHMENT_UNARM:
                    if (dead) break;
                    WEAPON::REMOVE_ALL_PED_WEAPONS(h, TRUE);
                    break;
                case PUNISHMENT_FREEZE:
                    ENTITY::FREEZE_ENTITY_POSITION(h, TRUE);
                    break;
                case PUNISHMENT_COWER:
                    if (dead) break;
                    TASK::TASK_COWER(h, 5000);
                    break;
                case PUNISHMENT_FLEE:
                    if (dead) break;
                    TASK::TASK_SMART_FLEE_COORD(h, player_pos.x, player_pos.y, player_pos.z, 100.f, -1, FALSE, FALSE);
                    break;
                case PUNISHMENT_PUSH:
                    if (dead) break;
                    ENTITY::APPLY_FORCE_TO_ENTITY(h, 1, 0.f, 0.f, 10.f, 0.f, 0.f, 0.f, 0, FALSE, TRUE, TRUE, FALSE, TRUE);
                    break;
                }
            }
        }

        ~CommandNpcProximityPunishment() override
        {
            CommandTickDispatch::RemoveCommand(this);
        }
    };
}
