#include "Commands/World/Places/Interiors/CommandInterior.hpp"
#include "Commands/World/Places/TpUtil.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandInterior::CommandInterior(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names, Vector3 pos, bool needs_mp_world_state)
        : CommandPhysical(COMMAND_ACTION, parent, std::move(menu_name), std::move(command_names),
                          needs_mp_world_state ? LIT("Teleporting here will ensure the Online world state which might make the game unresponsive shortly.") : NOLABEL)
        , pos(pos)
        , needs_mp_world_state(needs_mp_world_state)
    {
    }

    CommandInterior::CommandInterior(CommandList* parent, Label&& menu_name, Vector3 pos, bool needs_mp_world_state)
        : CommandInterior(parent, std::move(menu_name), {}, pos, needs_mp_world_state)
    {
    }

    int CommandInterior::getInteriorId() const
    {
        return getInteriorId(pos);
    }

    int CommandInterior::getInteriorId(Vector3 pos)
    {
        return INTERIOR::GET_INTERIOR_AT_COORDS(pos.x, pos.y, pos.z);
    }

    void CommandInterior::disable(int interior_id)
    {
        if (!INTERIOR::IS_INTERIOR_CAPPED(interior_id))
            INTERIOR::CAP_INTERIOR(interior_id, TRUE);
        if (!INTERIOR::IS_INTERIOR_DISABLED(interior_id))
            INTERIOR::DISABLE_INTERIOR(interior_id, TRUE);
        INTERIOR::UNPIN_INTERIOR(interior_id);
    }

    void CommandInterior::enable() const
    {
        enable(pos, needs_mp_world_state);
    }

    void CommandInterior::enable(Vector3 pos, bool)
    {
        if (auto int_id = getInteriorId(pos))
            enable(int_id);
    }

    void CommandInterior::enable(int interior_id)
    {
        INTERIOR::PIN_INTERIOR_IN_MEMORY(interior_id);
        if (INTERIOR::IS_INTERIOR_CAPPED(interior_id))
            INTERIOR::CAP_INTERIOR(interior_id, FALSE);
        if (INTERIOR::IS_INTERIOR_DISABLED(interior_id))
            INTERIOR::DISABLE_INTERIOR(interior_id, FALSE);
        INTERIOR::REFRESH_INTERIOR(interior_id);
    }

    void CommandInterior::teleport() const
    {
        enable();
        TpUtil::teleport_exact(pos.x, pos.y, pos.z);
    }

    void CommandInterior::teleport(Vector3 pos, bool needs_mp_world_state)
    {
        enable(pos, needs_mp_world_state);
        TpUtil::teleport_exact(pos.x, pos.y, pos.z);
    }

    void CommandInterior::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            teleport();
        });
    }
}
