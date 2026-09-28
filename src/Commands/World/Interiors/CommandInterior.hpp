#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Core/types.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"

#include <vector>

namespace Stand
{
    class CommandInterior : public CommandPhysical
    {
    private:
        const Vector3 pos;
        const bool needs_mp_world_state;

    public:
        explicit CommandInterior(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names, Vector3 pos, bool needs_mp_world_state = false);
        explicit CommandInterior(CommandList* parent, Label&& menu_name, Vector3 pos, bool needs_mp_world_state = false);

        [[nodiscard]] int getInteriorId() const;
        [[nodiscard]] static int getInteriorId(Vector3 pos);

        static void disable(int interior_id);

        void enable() const;
        static void enable(Vector3 pos, bool needs_mp_world_state = false);
        static void enable(int interior_id);

        void teleport() const;
        static void teleport(Vector3 pos, bool needs_mp_world_state);

        void onClick(Click& click) final;
    };
}
