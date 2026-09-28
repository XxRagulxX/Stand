#pragma once
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/World/Places/Position/CommandVector3.hpp"
#include "Core/types.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
    template <float Vector3::* axis_offset>
    class CommandVector3Slider : public CommandSliderFloat
    {
    public:
        explicit CommandVector3Slider(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names)
            : CommandSliderFloat(parent, std::move(menu_name), std::move(command_names), NOLABEL, -1000000, 1000000, 0, 200, CMDFLAGS_SLIDER & ~CMDFLAG_SUPPORTS_STATE_OPERATIONS)
        {
            CommandTickDispatch::AddCommand(this);
        }

        ~CommandVector3Slider() override
        {
            CommandTickDispatch::RemoveCommand(this);
        }

        void onTick() override
        {
            syncFromVec();
        }

        bool onLeft(Click& click, bool holding) override
        {
            syncFromVec();
            return CommandSliderFloat::onLeft(click, holding);
        }

        bool onRight(Click& click, bool holding) override
        {
            syncFromVec();
            return CommandSliderFloat::onRight(click, holding);
        }

        void onChange(Click& click, int prev_value) override
        {
            if (!click.isStand())
            {
                auto vec = parent->as<CommandVector3>()->getVec();
                vec.*axis_offset = getFloatValue();
                parent->as<CommandVector3>()->setVec(vec);
            }
        }

    private:
        void syncFromVec()
        {
            auto vec = parent->as<CommandVector3>()->getVec();
            Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
            CommandSliderFloat::setValue(vec.*axis_offset, click);
        }
    };
}
