#include "Commands/World/Places/Interiors/CommandInteriorVehware.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
    CommandInteriorVehware::CommandInteriorVehware(CommandList* parent)
        : CommandInteriorCustomisable(parent, CMDNAMES("tpvehware"))
    {
    }

    Vector3 CommandInteriorVehware::getPosition() const
    {
        Vector3 v{};
        v.x = 975.0267f;
        v.y = -3000.2334f;
        v.z = -39.648537f;
        return v;
    }

    void CommandInteriorVehware::toggleEntitySets(long long style) const
    {
        const int int_id = getInteriorId();

        toggleEntitySet(int_id, "Basic_style_set", style <= 0);
        toggleEntitySet(int_id, "Urban_style_set", style == 1);
        toggleEntitySet(int_id, "Branded_style_set", style == 2);

        if (style <= 0)
        {
            INTERIOR::SET_INTERIOR_ENTITY_SET_TINT_INDEX(int_id, "Basic_style_set", static_cast<int>(style) * -1);
        }
    }

    void CommandInteriorVehware::toggleEntitySets() const
    {
        toggleEntitySets(style ? style->value : 1);
    }

    void CommandInteriorVehware::randomiseEntitySets() const
    {
    }
}
