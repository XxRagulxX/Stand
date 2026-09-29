#pragma once
#include "Game/typedecl.hpp"
#include "Util/Label.hpp"

#define PUNISHMENT_EXPLODE_ANON   (Stand::punishment_t)0b1
#define PUNISHMENT_EXPLODE_OWNED  (Stand::punishment_t)0b10
#define PUNISHMENT_BURN           (Stand::punishment_t)0b100
#define PUNISHMENT_DIE            (Stand::punishment_t)0b1000
#define PUNISHMENT_UNARM          (Stand::punishment_t)0b10000
#define PUNISHMENT_INTERRUPT      (Stand::punishment_t)0b100000
#define PUNISHMENT_FREEZE         (Stand::punishment_t)0b1000000
#define PUNISHMENT_COWER          (Stand::punishment_t)0b10000000
#define PUNISHMENT_FLEE           (Stand::punishment_t)0b100000000
#define PUNISHMENT_PUSH           (Stand::punishment_t)0b1000000000
#define PUNISHMENT_DELETE         (Stand::punishment_t)0b10000000000
#define PUNISHMENT_MARK           (Stand::punishment_t)0b100000000000
#define PUNISHMENT_SHOWMODELNAME  (Stand::punishment_t)0b1000000000000
#define PUNISHMENT_DRIVE          (Stand::punishment_t)0b10000000000000
#define PUNISHMENT_REVIVE         (Stand::punishment_t)0b100000000000000
#define PUNISHMENT_RAGDOLL        (Stand::punishment_t)0b1000000000000000
#define PUNISHMENT_REPAIR         (Stand::punishment_t)0b10000000000000000
#define PUNISHMENT_WEAKEN         (Stand::punishment_t)0b100000000000000000

#define PUNISHMENTFOR_NPC          (uint8_t)0b0000001
#define PUNISHMENTFOR_PLAYER       (uint8_t)0b0000010
#define PUNISHMENTFOR_WORLD        (uint8_t)0b0000100
#define PUNISHMENTFOR_AIMINGAUSER  (uint8_t)0b0001000
#define PUNISHMENTFOR_NOTBROAD     (uint8_t)0b0010000
#define PUNISHMENTFOR_USERACTION   (uint8_t)0b0100000
#define PUNISHMENTFOR_NPCHOSTILITY (uint8_t)0b1000000

namespace Stand
{
    class Punishment
    {
    public:
        const punishment_t mask;
        const Label name;
        const uint8_t _for;
        const Label help_text;

        Punishment(punishment_t mask, Label&& name, uint8_t _for, Label&& help_text = NOLABEL)
            : mask(mask), name(std::move(name)), _for(_for), help_text(std::move(help_text))
        {}

        [[nodiscard]] bool isApplicable(uint8_t _to) const noexcept
        {
            constexpr uint8_t target_selector = PUNISHMENTFOR_NPC | PUNISHMENTFOR_PLAYER | PUNISHMENTFOR_WORLD | PUNISHMENTFOR_NPCHOSTILITY;
            if (!((_for & target_selector) & _to)) return false;
            if ((_for & PUNISHMENTFOR_AIMINGAUSER) && !(_to & PUNISHMENTFOR_AIMINGAUSER)) return false;
            if ((_for & PUNISHMENTFOR_NOTBROAD) && !(_to & PUNISHMENTFOR_NOTBROAD)) return false;
            if ((_for & PUNISHMENTFOR_USERACTION) && !(_to & PUNISHMENTFOR_USERACTION)) return false;
            return true;
        }
    };

    struct Punishments
    {
        inline static Punishment explode_anon{PUNISHMENT_EXPLODE_ANON, LIT("Anonymous Explosion"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_PLAYER | PUNISHMENTFOR_NPCHOSTILITY | PUNISHMENTFOR_WORLD};
        inline static Punishment explode_owned{PUNISHMENT_EXPLODE_OWNED, LIT("Owned Explosion"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_PLAYER | PUNISHMENTFOR_NPCHOSTILITY | PUNISHMENTFOR_WORLD};
        inline static Punishment burn{PUNISHMENT_BURN, LIT("Burn"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_PLAYER | PUNISHMENTFOR_NPCHOSTILITY | PUNISHMENTFOR_WORLD};
        inline static Punishment die{PUNISHMENT_DIE, LIT("Die"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_PLAYER | PUNISHMENTFOR_NPCHOSTILITY};
        inline static Punishment unarm{PUNISHMENT_UNARM, LIT("Disarm"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_PLAYER | PUNISHMENTFOR_NPCHOSTILITY,
            LIT("Will leave them with nothing but their fists.")};
        inline static Punishment interrupt{PUNISHMENT_INTERRUPT, LIT("Interrupt"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_PLAYER | PUNISHMENTFOR_AIMINGAUSER | PUNISHMENTFOR_NPCHOSTILITY};
        inline static Punishment freeze{PUNISHMENT_FREEZE, LIT("Freeze"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_PLAYER | PUNISHMENTFOR_NPCHOSTILITY};
        inline static Punishment cower{PUNISHMENT_COWER, LIT("Cower"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_NPCHOSTILITY};
        inline static Punishment flee{PUNISHMENT_FLEE, LIT("Flee"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_NPCHOSTILITY};
        inline static Punishment push{PUNISHMENT_PUSH, LIT("Push Away"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_NPCHOSTILITY};
        inline static Punishment drive{PUNISHMENT_DRIVE, LIT("Drive Vehicle"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_USERACTION};
        inline static Punishment revive{PUNISHMENT_REVIVE, LIT("Revive"),
            PUNISHMENTFOR_NPC};
        inline static Punishment ragdoll{PUNISHMENT_RAGDOLL, LIT("Ragdoll"),
            PUNISHMENTFOR_PLAYER | PUNISHMENTFOR_NPC | PUNISHMENTFOR_NOTBROAD | PUNISHMENTFOR_NPCHOSTILITY};
        inline static Punishment repair{PUNISHMENT_REPAIR, LIT("Repair Vehicle"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_USERACTION};
        inline static Punishment weaken{PUNISHMENT_WEAKEN, LIT("Weaken"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_NPCHOSTILITY,
            LIT("Reduces their health so they'll die easily.")};
        inline static Punishment del{PUNISHMENT_DELETE, LIT("Delete"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_NOTBROAD | PUNISHMENTFOR_NPCHOSTILITY};
        inline static Punishment mark{PUNISHMENT_MARK, LIT("AR Marker"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_PLAYER | PUNISHMENTFOR_NOTBROAD | PUNISHMENTFOR_NPCHOSTILITY};
        inline static Punishment showmodelname{PUNISHMENT_SHOWMODELNAME, LIT("Show Model Name"),
            PUNISHMENTFOR_NPC | PUNISHMENTFOR_PLAYER | PUNISHMENTFOR_NOTBROAD | PUNISHMENTFOR_NPCHOSTILITY | PUNISHMENTFOR_WORLD};

        inline static const Punishment* all[] = {
            &explode_anon, &explode_owned, &burn, &die, &unarm, &interrupt, &freeze,
            &cower, &flee, &push, &drive, &revive, &ragdoll, &repair, &weaken, &del, &mark, &showmodelname,
        };
    };
}
