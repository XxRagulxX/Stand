#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Game/Pools.hpp"
#include "Game/Punishments.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
    class CommandNpcHostilityPunishment : public CommandToggle
    {
        const punishment_t mask;

    public:
        explicit CommandNpcHostilityPunishment(CommandList* parent, const Punishment& p)
            : CommandToggle(parent, Label(p.name), {}, Label(p.help_text))
            , mask(p.mask)
        {}

        void onEnable(Click& click) final
        {
            AllEntitiesEveryTick::npc_hostility_punishments |= mask;
            CommandTickDispatch::AddCommand(this);
        }

        void onDisable(Click& click) final
        {
            AllEntitiesEveryTick::npc_hostility_punishments &= ~mask;
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
            bool punish_all = AllEntitiesEveryTick::npc_hostility_include_everyone
                           || AllEntitiesEveryTick::npc_hostility_include_everyone_in_missions;

            for (auto ped : Pools::GetPeds())
            {
                if (!ped || ped.IsPlayer()) continue;
                int h = ped.GetHandle();

                if (!punish_all && !PED::IS_PED_IN_COMBAT(h, self_h)) continue;

                bool dead = PED::IS_PED_DEAD_OR_DYING(h, TRUE);

                switch (mask)
                {
                case PUNISHMENT_EXPLODE_ANON:
                    if (dead) break;
                    {
                        Vector3 pos = ENTITY::GET_ENTITY_COORDS(h, TRUE);
                        FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 2, 1.f, TRUE, FALSE, 1.f, FALSE);
                    }
                    break;
                case PUNISHMENT_EXPLODE_OWNED:
                    if (dead) break;
                    {
                        Vector3 pos = ENTITY::GET_ENTITY_COORDS(h, TRUE);
                        FIRE::ADD_OWNED_EXPLOSION(self_h, pos.x, pos.y, pos.z, 2, 1.f, TRUE, FALSE, 1.f);
                    }
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
                    {
                        Vector3 pos = ENTITY::GET_ENTITY_COORDS(self_h, TRUE);
                        TASK::TASK_SMART_FLEE_COORD(h, pos.x, pos.y, pos.z, 100.f, -1, FALSE, FALSE);
                    }
                    break;
                case PUNISHMENT_PUSH:
                    if (dead) break;
                    ENTITY::APPLY_FORCE_TO_ENTITY(h, 1, 0.f, 0.f, 10.f, 0.f, 0.f, 0.f, 0, FALSE, TRUE, TRUE, FALSE, TRUE);
                    break;
                case PUNISHMENT_WEAKEN:
                    if (dead) break;
                    ENTITY::SET_ENTITY_HEALTH(h, 1, 0, 0);
                    break;
                }
            }
        }

        ~CommandNpcHostilityPunishment() override
        {
            CommandTickDispatch::RemoveCommand(this);
        }
    };
}
