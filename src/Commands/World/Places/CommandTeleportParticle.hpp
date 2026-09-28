#pragma once
#include "Commands/World/Places/CommandListSelectParticle.hpp"

namespace Stand
{
    class CommandTeleportParticle : public CommandListSelectParticle
    {
    public:
        inline static CommandListSelect* instance = nullptr;

        explicit CommandTeleportParticle(CommandList* parent);

        static void playTpParticle(const Vector3& pos);
    };
}
