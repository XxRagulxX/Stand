#include "Commands/World/Interiors/CommandInteriorBunker.hpp"

namespace Stand
{
    CommandInteriorBunker::CommandInteriorBunker(CommandList* parent)
        : CommandInteriorCustomisable(parent, CMDNAMES("tpbunker"))
    {
    }

    Vector3 CommandInteriorBunker::getPosition() const
    {
        Vector3 v{};
        v.x = 900.686f;
        v.y = -3224.6824f;
        v.z = -98.27065f;
        return v;
    }

    void CommandInteriorBunker::toggleEntitySets(long long style, bool security, bool equipment_upgrade, bool gun_locker, bool personal_quarters, bool gun_range) const
    {
        const int int_id = getInteriorId();

        toggleEntitySet(int_id, "bunker_style_a", style == 0);
        toggleEntitySet(int_id, "bunker_style_b", style == 1);
        toggleEntitySet(int_id, "bunker_style_c", style == 2);

        toggleEntitySet(int_id, "standard_security_set", !security);
        toggleEntitySet(int_id, "security_upgrade", security);

        toggleEntitySet(int_id, "standard_bunker_set", !equipment_upgrade);
        toggleEntitySet(int_id, "upgrade_bunker_set", equipment_upgrade);

        toggleEntitySet(int_id, "gun_locker_upgrade", gun_locker);

        toggleEntitySet(int_id, "Office_blocker_set", !personal_quarters);
        toggleEntitySet(int_id, "Office_Upgrade_set", personal_quarters);

        toggleEntitySet(int_id, "gun_range_blocker_set", !gun_range);
        toggleEntitySet(int_id, "gun_wall_blocker", !gun_range);
        toggleEntitySet(int_id, "gun_range_lights", gun_range);
    }

    void CommandInteriorBunker::toggleEntitySets() const
    {
        toggleEntitySets(
            style ? style->value : 1,
            security ? security->m_on : true,
            equipment_upgrade ? equipment_upgrade->m_on : true,
            gun_locker ? gun_locker->m_on : true,
            personal_quarters ? personal_quarters->m_on : true,
            gun_range ? gun_range->m_on : true
        );
    }

    void CommandInteriorBunker::randomiseEntitySets() const
    {
    }
}
