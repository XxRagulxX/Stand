#include "Commands/World/Places/Position/CommandSavePos.hpp"

#include "Menu/Click.hpp"
#include "Util/Label.hpp"

#include <filesystem>
#include <fstream>

namespace Stand
{
    CommandSavePos::CommandSavePos(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names)
        : CommandPhysical(COMMAND_ACTION, parent, std::move(menu_name), std::move(command_names))
    {
    }

    std::string CommandSavePos::getCommandSyntax() const
    {
        if (command_names.empty())
            return {};
        return "Command: " + command_names.front() + " [name]";
    }

    void CommandSavePos::saveToFile(Click& click, const std::wstring& name)
    {
        auto path = std::filesystem::path(std::getenv("appdata")) / "StandEnhanced" / "Places";
        std::filesystem::create_directories(path);
        path /= name + L".txt";
        std::ofstream file(path);
        file << getPos();
        if (file.fail())
            click.setResponse(LIT("Failed to save position."));
        else
            click.setResponse(LIT("Position saved."));
    }

    void CommandSavePos::onCommand(Click& click, std::wstring& args)
    {
        if (args.empty())
            return;
        std::wstring name = args;
        args.clear();
        click.ensureScriptThread([this, name = std::move(name)](Click& click) {
            saveToFile(click, name);
        });
    }
}
