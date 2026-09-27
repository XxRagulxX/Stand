#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Vehicle/SubmarineCar/CommandSubCarAutoSwitch.hpp"

namespace Stand
{
    class CommandListSubmarineCar : public CommandList
    {
    public:
        CommandSubCarAutoSwitch* const mainSwitch;
        CommandToggle* const noAnim;

        explicit CommandListSubmarineCar(CommandList* parent)
            : CommandList(parent, LIT("Auto Transform Submarine Cars")),
              mainSwitch(createChild<CommandSubCarAutoSwitch>()),
              noAnim(createChild<CommandToggle>(LIT("No Animation"), CMDNAMES("noanimsubcar")))
        {
            mainSwitch->config_noAnim = noAnim;
        }
    };
}
