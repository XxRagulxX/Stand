#pragma once

#include "Commands/Widgets/CommandListSelect.hpp"
#include "Rendering/AbstractRenderer.hpp"
#include "Rendering/StandPort/CommandColour.hpp"

#include "Core/Spinlock.hpp"

namespace Stand
{
    class CommandBirender : public CommandListSelect
    {
    protected:
        Spinlock mtx = {};

    public:
        CommandColour* colour = nullptr;

        explicit CommandBirender(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names, CommandColour* const colour)
            : CommandListSelect(parent, std::move(menu_name), std::move(command_names), NOLABEL, {
                {0, LOC("DOFF")},
                {1, LOC("RNDR_LWLTCY")},
                {2, LOC("RNDR_LGCY")},
            }, 0)
        {
            this->colour = colour;
        }

        void onChange(Click& click, long long prev_value) final;

    protected:
        virtual void onTick() = 0;
        virtual void draw(const AbstractRenderer& renderer) = 0;
    };
}
