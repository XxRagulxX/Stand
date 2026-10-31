#include "Commands/Widgets/CommandReadonlyName.hpp"

#include <soup/os.hpp>

namespace Stand
{
	CommandReadonlyName::CommandReadonlyName(CommandList* const parent, Label&& menu_name, Label&& help_text)
		: CommandPhysical(COMMAND_READONLY_NAME, parent, std::move(menu_name), {}, std::move(help_text))
	{
	}

	void CommandReadonlyName::onClick(Click& click)
	{
		soup::os::copyToClipboard(menu_name.getLocalisedUtf8());
	}
}
