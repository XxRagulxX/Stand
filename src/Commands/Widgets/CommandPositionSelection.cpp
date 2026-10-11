#include "Commands/Widgets/CommandPositionSelection.hpp"

#include "Rendering/Gui.hpp"

namespace Stand
{
	CommandPositionSelection::CommandPositionSelection(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names)
		: CommandWithOnTickFocused(parent, std::move(menu_name), std::move(command_names), NOLABEL, "467.1256, 5543.1304, 785.0175")
	{
	}

	void CommandPositionSelection::onClick(Click& click)
	{
	}

	void CommandPositionSelection::onCommand(Click& click, std::wstring& args)
	{
		onCommandApply(click, args);
		setPosFromValue();
	}

	void CommandPositionSelection::onTickFocused()
	{
	}

	void CommandPositionSelection::setPosFromValue()
	{
	}

	void CommandPositionSelection::onChange()
	{
	}
}
