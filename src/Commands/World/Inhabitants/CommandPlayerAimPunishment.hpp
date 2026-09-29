#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Game/Punishments.hpp"
#include "Menu/Click.hpp"
#include "Network/Players.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
    class CommandPlayerAimPunishment : public CommandToggle
    {
        const punishment_t mask;

    public:
        explicit CommandPlayerAimPunishment(CommandList* parent, const Punishment& p)
            : CommandToggle(parent, Label(p.name), {}, Label(p.help_text))
            , mask(p.mask)
        {}

        void onEnable(Click& click) final
        {
            AllEntitiesEveryTick::player_aim_punishments |= mask;
            CommandTickDispatch::AddCommand(this);
        }

        void onDisable(Click& click) final
        {
            AllEntitiesEveryTick::player_aim_punishments &= ~mask;
            CommandTickDispatch::RemoveCommand(this);
        }

        void onTick() final
        {
            int self_h = Self::GetPed().GetHandle();
            int self_id = PLAYER::PLAYER_ID();
            bool need_aiming_at_me = AllEntitiesEveryTick::player_needs_to_aim_at_user;

            for (auto& [idx, player] : Players::GetPlayers())
            {
                if (player.GetId() == self_id) continue;

                int pid = player.GetId();

                if (need_aiming_at_me)
                {
                    if (!PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(pid, self_h)) continue;
                }
                else
                {
                    if (!PLAYER::IS_PLAYER_FREE_AIMING(pid)) continue;
                }

                Ped ped = player.GetPed();
                if (!ped) continue;
                int h = ped.GetHandle();

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
                    if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(h))
                        ENTITY::SET_ENTITY_HEALTH(h, 0, 0, 0);
                    else
                        NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(h);
                    break;
                case PUNISHMENT_UNARM:
                    if (dead) break;
                    if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(h))
                        WEAPON::REMOVE_ALL_PED_WEAPONS(h, TRUE);
                    else
                        NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(h);
                    break;
                case PUNISHMENT_INTERRUPT:
                    if (dead) break;
                    if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(h))
                        TASK::CLEAR_PED_TASKS(h);
                    else
                        NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(h);
                    break;
                case PUNISHMENT_FREEZE:
                    if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(h))
                        ENTITY::FREEZE_ENTITY_POSITION(h, TRUE);
                    else
                        NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(h);
                    break;
                }
            }
        }

        ~CommandPlayerAimPunishment() override
        {
            CommandTickDispatch::RemoveCommand(this);
        }
    };
}
