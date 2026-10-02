#pragma once
#include "Core/types.hpp"

namespace Stand
{
    struct DedsecHackExplode;
    struct DedsecHackDestroy;
    struct DedsecHackDelete;
    struct DedsecHackDrive;
    struct DedsecHackEnter;
    struct DedsecHackBurn;
    struct DedsecHackRevive;
    struct DedsecHackMenuPlayer;
    struct DedsecHackMenuPlayerVeh;
    struct DedsecHackFreeze;
    struct DedsecHackUnfreeze;
    struct DedsecHackEmpty;
    struct DedsecHackDisarm;
    struct DedsecHackCower;
    struct DedsecHackFlee;
    struct DedsecHackIgnite;
    struct DedsecHackSlingshot;
    struct DedsecHackCage;
    struct DedsecHackKill;

    struct DedsecHack
    {
        bool enabled;
        const char* const label;
        float x = 0.0f;
        float y = 0.0f;

        explicit constexpr DedsecHack(const char* label, const bool default_on = true)
            : enabled(default_on), label(label)
        {
        }

        virtual void execute(Entity ent) const = 0;

        static DedsecHackFreeze freeze;
        static DedsecHackUnfreeze unfreeze;
        static DedsecHackExplode explode;
        static DedsecHackDisarm disarm;
        static DedsecHackDelete del;
        static DedsecHackDestroy destroy;
        static DedsecHackDrive drive;
        static DedsecHackEnter enter;
        static DedsecHackEmpty empty;
        static DedsecHackIgnite ignite;
        static DedsecHackSlingshot slingshot;
        static DedsecHackMenuPlayerVeh menu_player_veh;
        static DedsecHackBurn burn;
        static DedsecHackFlee flee;
        static DedsecHackCower cower;
        static DedsecHackRevive revive;
        static DedsecHackKill kill;
        static DedsecHackCage cage;
        static DedsecHackMenuPlayer menu_player;
    };

    struct DedsecHackFreeze : public DedsecHack
    {
        explicit constexpr DedsecHackFreeze();
        void execute(Entity ent) const final;
    };

    struct DedsecHackUnfreeze : public DedsecHack
    {
        explicit constexpr DedsecHackUnfreeze();
        void execute(Entity ent) const final;
    };

    struct DedsecHackExplode : public DedsecHack
    {
        explicit constexpr DedsecHackExplode();
        void execute(Entity ent) const final;
    };

    struct DedsecHackDestroy : public DedsecHack
    {
        explicit constexpr DedsecHackDestroy();
        void execute(Entity ent) const final;
    };

    struct DedsecHackDelete : public DedsecHack
    {
        explicit constexpr DedsecHackDelete();
        void execute(Entity ent) const final;
    };

    struct DedsecHackDrive : public DedsecHack
    {
        explicit constexpr DedsecHackDrive();
        void execute(Entity ent) const final;
    };

    struct DedsecHackEnter : public DedsecHack
    {
        explicit constexpr DedsecHackEnter();
        void execute(Entity ent) const final;
    };

    struct DedsecHackBurn : public DedsecHack
    {
        explicit constexpr DedsecHackBurn();
        void execute(Entity ent) const final;
    };

    struct DedsecHackRevive : public DedsecHack
    {
        explicit constexpr DedsecHackRevive();
        void execute(Entity ent) const final;
    };

    struct DedsecHackMenuPlayer : public DedsecHack
    {
        explicit constexpr DedsecHackMenuPlayer();
        void execute(Entity ent) const final;
    };

    struct DedsecHackMenuPlayerVeh : public DedsecHack
    {
        explicit constexpr DedsecHackMenuPlayerVeh();
        void execute(Entity ent) const final;
    };

    struct DedsecHackEmpty : public DedsecHack
    {
        explicit constexpr DedsecHackEmpty();
        void execute(Entity ent) const final;
    };

    struct DedsecHackDisarm : public DedsecHack
    {
        explicit constexpr DedsecHackDisarm();
        void execute(Entity ent) const final;
    };

    struct DedsecHackCower : public DedsecHack
    {
        explicit constexpr DedsecHackCower();
        void execute(Entity ent) const final;
    };

    struct DedsecHackFlee : public DedsecHack
    {
        explicit constexpr DedsecHackFlee();
        void execute(Entity ent) const final;
    };

    struct DedsecHackIgnite : public DedsecHack
    {
        explicit constexpr DedsecHackIgnite();
        void execute(Entity ent) const final;
    };

    struct DedsecHackSlingshot : public DedsecHack
    {
        explicit constexpr DedsecHackSlingshot();
        void execute(Entity ent) const final;
    };

    struct DedsecHackCage : public DedsecHack
    {
        explicit constexpr DedsecHackCage();
        void execute(Entity ent) const final;
    };

    struct DedsecHackKill : public DedsecHack
    {
        explicit constexpr DedsecHackKill();
        void execute(Entity ent) const final;
    };
}
