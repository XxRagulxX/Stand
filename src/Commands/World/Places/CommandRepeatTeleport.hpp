#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandRepeatTeleport : public CommandPhysical
    {
    public:
        explicit CommandRepeatTeleport(CommandList* parent);
        void onClick(Click& click) override;
    };
}
