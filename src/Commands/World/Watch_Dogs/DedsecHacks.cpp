#include "Commands/World/Watch_Dogs/DedsecHacks.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Joaat.hpp"

#include <cmath>

namespace Stand
{
    constexpr DedsecHackFreeze::DedsecHackFreeze()
        : DedsecHack("Freeze")
    {
    }

    void DedsecHackFreeze::execute(Entity ent) const
    {
        ENTITY::FREEZE_ENTITY_POSITION(ent, TRUE);
    }

    constexpr DedsecHackUnfreeze::DedsecHackUnfreeze()
        : DedsecHack("Unfreeze")
    {
    }

    void DedsecHackUnfreeze::execute(Entity ent) const
    {
        ENTITY::FREEZE_ENTITY_POSITION(ent, FALSE);
    }

    constexpr DedsecHackExplode::DedsecHackExplode()
        : DedsecHack("Explode")
    {
    }

    void DedsecHackExplode::execute(Entity ent) const
    {
        Vector3 pos = ENTITY::GET_ENTITY_COORDS(ent, FALSE);
        FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 2, 100.0f, TRUE, FALSE, 0.0f, FALSE);
    }

    constexpr DedsecHackDestroy::DedsecHackDestroy()
        : DedsecHack("Destroy")
    {
    }

    void DedsecHackDestroy::execute(Entity ent) const
    {
        VEHICLE::SET_VEHICLE_ENGINE_HEALTH(ent, -4000.0f);
        Vector3 pos = ENTITY::GET_ENTITY_COORDS(ent, FALSE);
        FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 4, 1.0f, TRUE, FALSE, 0.0f, FALSE);
    }

    constexpr DedsecHackDelete::DedsecHackDelete()
        : DedsecHack("Delete")
    {
    }

    void DedsecHackDelete::execute(Entity ent) const
    {
        Entity e = ent;
        ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&e);
        ENTITY::DELETE_ENTITY(&e);
    }

    constexpr DedsecHackDrive::DedsecHackDrive()
        : DedsecHack("Drive")
    {
    }

    void DedsecHackDrive::execute(Entity ent) const
    {
        Ped player = PLAYER::GET_PLAYER_PED(-1);
        Ped driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(ent, -1, FALSE);
        if (driver != 0 && driver != player)
            TASK::CLEAR_PED_TASKS_IMMEDIATELY(driver);
        PED::SET_PED_INTO_VEHICLE(player, ent, -1);
    }

    constexpr DedsecHackEnter::DedsecHackEnter()
        : DedsecHack("Enter")
    {
    }

    void DedsecHackEnter::execute(Entity ent) const
    {
        Ped player = PLAYER::GET_PLAYER_PED(-1);
        int max_seats = VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(ent) + 1;
        int seat = -1;
        for (int i = 0; i < max_seats; ++i)
        {
            if (VEHICLE::IS_VEHICLE_SEAT_FREE(ent, i, FALSE))
            {
                seat = i;
                break;
            }
        }
        PED::SET_PED_INTO_VEHICLE(player, ent, seat);
    }

    constexpr DedsecHackBurn::DedsecHackBurn()
        : DedsecHack("Burn")
    {
    }

    void DedsecHackBurn::execute(Entity ent) const
    {
        if (!FIRE::IS_ENTITY_ON_FIRE(ent))
            FIRE::START_ENTITY_FIRE(ent);
    }

    constexpr DedsecHackRevive::DedsecHackRevive()
        : DedsecHack("Revive")
    {
    }

    void DedsecHackRevive::execute(Entity ent) const
    {
        ENTITY::SET_ENTITY_HEALTH(ent, ENTITY::GET_ENTITY_MAX_HEALTH(ent), 0, 0);
        if (FIRE::IS_ENTITY_ON_FIRE(ent))
            FIRE::STOP_ENTITY_FIRE(ent);
        TASK::CLEAR_PED_TASKS_IMMEDIATELY(ent);
    }

    constexpr DedsecHackMenuPlayer::DedsecHackMenuPlayer()
        : DedsecHack("In Stand")
    {
    }

    void DedsecHackMenuPlayer::execute(Entity ent) const
    {
    }

    constexpr DedsecHackMenuPlayerVeh::DedsecHackMenuPlayerVeh()
        : DedsecHack("In Stand")
    {
    }

    void DedsecHackMenuPlayerVeh::execute(Entity ent) const
    {
        Ped driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(ent, -1, FALSE);
        if (driver != 0)
            menu_player.execute(driver);
    }

    constexpr DedsecHackEmpty::DedsecHackEmpty()
        : DedsecHack("Empty")
    {
    }

    void DedsecHackEmpty::execute(Entity ent) const
    {
        int max_seats = VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(ent);
        for (int i = -1; i < max_seats; ++i)
        {
            Ped p = VEHICLE::GET_PED_IN_VEHICLE_SEAT(ent, i, FALSE);
            if (p != 0)
                TASK::CLEAR_PED_TASKS_IMMEDIATELY(p);
        }
    }

    constexpr DedsecHackDisarm::DedsecHackDisarm()
        : DedsecHack("Disarm")
    {
    }

    void DedsecHackDisarm::execute(Entity ent) const
    {
        WEAPON::REMOVE_ALL_PED_WEAPONS(ent, FALSE);
    }

    constexpr DedsecHackCower::DedsecHackCower()
        : DedsecHack("Cower")
    {
    }

    void DedsecHackCower::execute(Entity ent) const
    {
        TASK::TASK_COWER(ent, -1);
    }

    constexpr DedsecHackFlee::DedsecHackFlee()
        : DedsecHack("Flee")
    {
    }

    void DedsecHackFlee::execute(Entity ent) const
    {
        TASK::TASK_SMART_FLEE_PED(ent, PLAYER::GET_PLAYER_PED(-1), 100.0f, -1, TRUE, FALSE);
    }

    constexpr DedsecHackIgnite::DedsecHackIgnite()
        : DedsecHack("Ignite", false)
    {
    }

    void DedsecHackIgnite::execute(Entity ent) const
    {
        VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(ent, -1.0f);
    }

    constexpr DedsecHackSlingshot::DedsecHackSlingshot()
        : DedsecHack("Slingshot")
    {
    }

    void DedsecHackSlingshot::execute(Entity ent) const
    {
        ENTITY::APPLY_FORCE_TO_ENTITY(ent, 1, 0.0f, 0.0f, 200.0f, 0.0f, 0.0f, 0.0f, 0, TRUE, TRUE, TRUE, FALSE, TRUE);
    }

    constexpr DedsecHackCage::DedsecHackCage()
        : DedsecHack("Cage", false)
    {
    }

    void DedsecHackCage::execute(Entity ent) const
    {
        Vector3 pos = ENTITY::GET_ENTITY_COORDS(ent, FALSE);
        float heading = ENTITY::GET_ENTITY_HEADING(ent);
        static const joaat_t kCageProp = "prop_gold_bar_01"_J;
        for (int i = 0; i < 6; ++i)
        {
            float angle = (float)i * 1.0472f;
            float ox = cosf(angle) * 1.5f;
            float oy = sinf(angle) * 1.5f;
            Object obj = OBJECT::CREATE_OBJECT(kCageProp, pos.x + ox, pos.y + oy, pos.z, TRUE, TRUE, FALSE);
            ENTITY::FREEZE_ENTITY_POSITION(obj, TRUE);
        }
    }

    constexpr DedsecHackKill::DedsecHackKill()
        : DedsecHack("Kill", false)
    {
    }

    void DedsecHackKill::execute(Entity ent) const
    {
        ENTITY::SET_ENTITY_HEALTH(ent, 0, 0, 0);
    }

    DedsecHackFreeze DedsecHack::freeze = DedsecHackFreeze();
    DedsecHackUnfreeze DedsecHack::unfreeze = DedsecHackUnfreeze();
    DedsecHackExplode DedsecHack::explode = DedsecHackExplode();
    DedsecHackDisarm DedsecHack::disarm = DedsecHackDisarm();
    DedsecHackDelete DedsecHack::del = DedsecHackDelete();
    DedsecHackDestroy DedsecHack::destroy = DedsecHackDestroy();
    DedsecHackDrive DedsecHack::drive = DedsecHackDrive();
    DedsecHackEnter DedsecHack::enter = DedsecHackEnter();
    DedsecHackEmpty DedsecHack::empty = DedsecHackEmpty();
    DedsecHackIgnite DedsecHack::ignite = DedsecHackIgnite();
    DedsecHackSlingshot DedsecHack::slingshot = DedsecHackSlingshot();
    DedsecHackMenuPlayerVeh DedsecHack::menu_player_veh = DedsecHackMenuPlayerVeh();
    DedsecHackBurn DedsecHack::burn = DedsecHackBurn();
    DedsecHackFlee DedsecHack::flee = DedsecHackFlee();
    DedsecHackCower DedsecHack::cower = DedsecHackCower();
    DedsecHackRevive DedsecHack::revive = DedsecHackRevive();
    DedsecHackKill DedsecHack::kill = DedsecHackKill();
    DedsecHackCage DedsecHack::cage = DedsecHackCage();
    DedsecHackMenuPlayer DedsecHack::menu_player = DedsecHackMenuPlayer();
}
