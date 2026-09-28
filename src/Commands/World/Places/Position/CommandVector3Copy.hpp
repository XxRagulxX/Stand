#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/World/Places/Position/CommandVector3.hpp"
#include "Core/types.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Clipboard.hpp"
#include "Util/Label.hpp"

#include <iomanip>
#include <sstream>

namespace Stand
{
    class CommandVector3Copy : public CommandPhysical
    {
    public:
        explicit CommandVector3Copy(CommandList* parent, std::vector<CommandName>&& command_names)
            : CommandPhysical(COMMAND_ACTION, parent, LIT("Copy Position To Clipboard"), std::move(command_names))
        {
        }

        void onClick(Click& click) override
        {
            click.ensureScriptThread([this](Click&) {
                auto vec = parent->as<CommandVector3>()->getVec();
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(6) << vec.x << ", " << vec.y << ", " << vec.z;
                Rendering::Clipboard::SetText(oss.str());
            });
        }
    };
}
