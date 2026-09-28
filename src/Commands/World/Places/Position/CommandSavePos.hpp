#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandSavePos : public CommandPhysical
    {
    public:
        explicit CommandSavePos(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names);

        [[nodiscard]] std::string getCommandSyntax() const override;

        void onCommand(Click& click, std::wstring& args) override;

    protected:
        [[nodiscard]] virtual std::string getPos() const = 0;
        void saveToFile(Click& click, const std::wstring& name);
    };
}
