#include "Commands/World/Places/CommandTeleportParticle.hpp"

#include "Util/Label.hpp"

namespace Stand
{
    CommandTeleportParticle::CommandTeleportParticle(CommandList* parent)
        : CommandListSelectParticle(parent, LIT("Teleportation Effect"), NOLABEL, INCLUDE_NONE, 0)
    {
        instance = this;
    }

    void CommandTeleportParticle::playTpParticle(const Vector3& pos)
    {
        if (!instance)
            return;
        if (const auto value = instance->value)
            PlaceParticles::play(value, pos);
    }
}
