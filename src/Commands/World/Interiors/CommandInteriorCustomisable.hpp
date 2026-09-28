#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Core/types.hpp"
#include "Menu/Click.hpp"

#include <vector>

namespace Stand
{
    class CommandInteriorCustomisable : public CommandPhysical
    {
    protected:
        explicit CommandInteriorCustomisable(CommandList* parent, std::vector<CommandName>&& command_names);

    public:
        void onClick(Click& click) final;
        void onOptionChanged() const;

    protected:
        [[nodiscard]] int getInteriorId() const;

        [[nodiscard]] virtual Vector3 getPosition() const = 0;
        virtual void toggleEntitySets() const = 0;
        virtual void randomiseEntitySets() const = 0;

        static void toggleEntitySet(int interior_id, const char* name, bool toggle);
        void toggleEntitySet(const char* name, bool toggle) const;
    };
}
