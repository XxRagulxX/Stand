#pragma once
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandBlackout : public CommandListSelect
    {
    public:
        explicit CommandBlackout(CommandList* parent)
            : CommandListSelect(parent, LIT("Blackout"), CMDNAMES("blackout"),
                LIT("This will only affect your game."),
                {
                    { 0, LIT("Disabled") },
                    { 1, LIT("Enabled") },
                    { 2, LIT("Enabled, Including Vehicles") },
                }, 0)
        {
        }

        ~CommandBlackout() override
        {
            if (m_ticking) CommandTickDispatch::RemoveCommand(this);
        }

        void onChange(Click& click, long long prev_value) override
        {
            if (value != 0)
            {
                if (!m_ticking)
                {
                    CommandTickDispatch::AddCommand(this);
                    m_ticking = true;
                }
            }
            else
            {
                if (m_ticking)
                {
                    CommandTickDispatch::RemoveCommand(this);
                    m_ticking = false;
                }
                ensureScriptThread(click, []
                {
                    GRAPHICS::SET_ARTIFICIAL_LIGHTS_STATE(false);
                });
            }
        }

        void onTick() override
        {
            if (value == 1)
            {
                GRAPHICS::SET_ARTIFICIAL_LIGHTS_STATE(true);
                GRAPHICS::SET_ARTIFICIAL_VEHICLE_LIGHTS_STATE(false);
            }
            else if (value == 2)
            {
                GRAPHICS::SET_ARTIFICIAL_LIGHTS_STATE(true);
                GRAPHICS::SET_ARTIFICIAL_VEHICLE_LIGHTS_STATE(true);
            }
        }

    private:
        bool m_ticking = false;
    };
}
