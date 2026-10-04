#pragma once
#include "Commands/World/Places/Interiors/CommandInteriorCustomisable.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"

namespace Stand
{
    class CommandInteriorVehware : public CommandInteriorCustomisable
    {
    public:
        CommandListSelect* style = nullptr;

        explicit CommandInteriorVehware(CommandList* parent);

    protected:
        [[nodiscard]] Vector3 getPosition() const final;
        void toggleEntitySets(long long style) const;
        void toggleEntitySets() const final;
        void randomiseEntitySets() const final;
    };
}
