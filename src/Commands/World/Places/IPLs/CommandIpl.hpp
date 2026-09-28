#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Core/types.hpp"
#include <vector>

namespace Stand
{
    class CommandToggleNoCorrelation;

    class CommandIpl : public CommandToggle
    {
    private:
        const Vector3 coords;
        const std::vector<const char*> m_request;
        const std::vector<const char*> m_remove;
        const std::vector<const char*> m_remove_enable_only;
        CommandToggleNoCorrelation* const m_tp_toggle;
    public:
        explicit CommandIpl(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names,
                            Vector3 coords,
                            std::vector<const char*>&& request,
                            std::vector<const char*>&& remove,
                            std::vector<const char*>&& remove_enable_only,
                            bool needs_mp_world_state,
                            CommandToggleNoCorrelation* tp_toggle);
        void onEnable(Click& click) override;
        void onDisable(Click& click) override;
    };
}
