#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/World/CommandListAtmosphere.hpp"
#include "Commands/World/Editor/CommandListEditor.hpp"
#include "Commands/World/Places/CommandListPlaces.hpp"
#include "Commands/World/Inhabitants/CommandListInhabitants.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandTabWorld : public CommandList
    {
    public:
        CommandListAtmosphere* const atmosphere;
        CommandListEditor* const editor;
        CommandListPlaces* const places;
        CommandListInhabitants* const inhabitants;

        explicit CommandTabWorld()
            : CommandList(nullptr, LIT("World")),
              atmosphere(createChild<CommandListAtmosphere>()),
              editor(createChild<CommandListEditor>()),
              places(createChild<CommandListPlaces>()),
              inhabitants(createChild<CommandListInhabitants>())
        {
        }
    };
}

namespace Stand::Features
{
    Stand::CommandTabWorld& GetCommandTabWorld();
}
