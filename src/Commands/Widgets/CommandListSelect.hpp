#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

#include <optional>
#include <utility>
#include <vector>

namespace Stand
{
    class CommandListSelect : public CommandPhysical
    {
    public:
        std::vector<std::pair<long long, Label>> options;
        long long value;
        long long default_value;

        explicit CommandListSelect(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, std::vector<std::pair<long long, Label>>&& options, long long default_value, commandflags_t flags = CMDFLAGS_LIST_SELECT, CommandPerm perm = COMMANDPERM_USERONLY) :
            CommandPhysical(COMMAND_LIST_SELECT, parent, std::move(menu_name), std::move(command_names), std::move(help_text), flags, perm),
            options(std::move(options)),
            value(default_value),
            default_value(default_value)
        {
        }

        bool onLeft(Click& click, bool holding) override;
        bool onRight(Click& click, bool holding) override;

        [[nodiscard]] Label getCurrentValueMenuName() const;
        [[nodiscard]] Label getCurrentValueHelpText() const;

        void setValue(Click& click, long long value);

        [[nodiscard]] std::string getState() const override;
        [[nodiscard]] std::string getDefaultState() const override;
        void setState(Click& click, const std::string& state) override;
        void applyDefaultState() override;

        virtual void onChange(Click& click, long long prev_value)
        {
        }

    private:
        [[nodiscard]] std::optional<Label> getLabelForValue(long long v) const;
        void updateValue(Click& click, long long new_val);
        void updateState(const Click& click);
    };
}
