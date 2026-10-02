#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/World/CommandListAtmosphere.hpp"
#include "Commands/World/CommandListWater.hpp"
#include "Commands/World/CommandWorldState.hpp"
#include "Commands/World/CommandListEnhancedOpenWorld.hpp"
#include "Commands/World/Watch_Dogs/CommandListDedsec.hpp"
#include "Commands/World/GeoGuessr/CommandGeoGuessr.hpp"
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
        CommandListWater* const water;
        CommandListEditor* const editor;
        CommandListPlaces* const places;
        CommandListInhabitants* const inhabitants;
        CommandWorldState* const worldstate;
        CommandListEnhancedOpenWorld* const enhancedopenworld;
        CommandListDedsec* const dedsec;
        CommandGeoGuessr* const geoguessr;

        explicit CommandTabWorld()
            : CommandList(nullptr, LIT("World")),
              atmosphere(createChild<CommandListAtmosphere>()),
              water(createChild<CommandListWater>()),
              editor(createChild<CommandListEditor>()),
              places(createChild<CommandListPlaces>()),
              inhabitants(createChild<CommandListInhabitants>()),
              worldstate(createChild<CommandWorldState>()),
              enhancedopenworld(createChild<CommandListEnhancedOpenWorld>()),
              dedsec(createChild<CommandListDedsec>()),
              geoguessr(createChild<CommandGeoGuessr>())
        {
        }
    };
}

namespace Stand::Features
{
    Stand::CommandTabWorld& GetCommandTabWorld();
}
