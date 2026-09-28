#pragma once
#include "Commands/World/Interiors/CommandInteriorCustomisable.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Stand/CommandToggleNoCorrelation.hpp"

namespace Stand
{
    class CommandInteriorBunker : public CommandInteriorCustomisable
    {
    public:
        CommandListSelect* style = nullptr;
        CommandToggleNoCorrelation* security = nullptr;
        CommandToggleNoCorrelation* equipment_upgrade = nullptr;
        CommandToggleNoCorrelation* gun_locker = nullptr;
        CommandToggleNoCorrelation* personal_quarters = nullptr;
        CommandToggleNoCorrelation* gun_range = nullptr;

        explicit CommandInteriorBunker(CommandList* parent);

    protected:
        [[nodiscard]] Vector3 getPosition() const final;
        void toggleEntitySets(long long style, bool security, bool equipment_upgrade, bool gun_locker, bool personal_quarters, bool gun_range) const;
        void toggleEntitySets() const final;
        void randomiseEntitySets() const final;
    };
}
