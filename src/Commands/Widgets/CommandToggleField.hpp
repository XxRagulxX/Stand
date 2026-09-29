#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandToggleField : public CommandToggle
    {
        bool* const m_ptr;

    public:
        explicit CommandToggleField(CommandList* parent, bool* ptr, Label&& menu_name,
            std::vector<CommandName>&& command_names = {}, Label&& help_text = NOLABEL,
            commandflags_t flags = CMDFLAGS_TOGGLE)
            : CommandToggle(parent, std::move(menu_name), std::move(command_names),
                            std::move(help_text), ptr ? *ptr : false, flags)
            , m_ptr(ptr)
        {}

        void onChange(Click& click) final
        {
            if (m_ptr) *m_ptr = m_on;
        }
    };
}
