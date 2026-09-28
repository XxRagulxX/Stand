#include "Commands/World/Interiors/CommandInteriorCustomisable.hpp"
#include "Commands/World/Interiors/CommandInterior.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandInteriorCustomisable::CommandInteriorCustomisable(CommandList* parent, std::vector<CommandName>&& command_names)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Teleport"), std::move(command_names))
    {
    }

    void CommandInteriorCustomisable::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            toggleEntitySets();
            CommandInterior::teleport(getPosition(), true);
        });
    }

    void CommandInteriorCustomisable::onOptionChanged() const
    {
        FiberPool::queueJob([this] {
            const auto int_id = getInteriorId();
            CommandInterior::disable(int_id);
            toggleEntitySets();
            CommandInterior::enable(int_id);
        });
    }

    int CommandInteriorCustomisable::getInteriorId() const
    {
        return CommandInterior::getInteriorId(getPosition());
    }

    void CommandInteriorCustomisable::toggleEntitySet(int interior_id, const char* name, bool toggle)
    {
        if (toggle)
            INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(interior_id, name);
        else
            INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(interior_id, name);
    }

    void CommandInteriorCustomisable::toggleEntitySet(const char* name, bool toggle) const
    {
        toggleEntitySet(getInteriorId(), name, toggle);
    }
}
