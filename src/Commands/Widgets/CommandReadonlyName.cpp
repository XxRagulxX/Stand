#include "Commands/Widgets/CommandReadonlyName.hpp"

#include "Localization/lang.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	CommandReadonlyName::CommandReadonlyName(CommandList* const parent, Label&& menu_name, Label&& help_text)
		: CommandPhysical(COMMAND_READONLY_NAME, parent, std::move(menu_name), {}, std::move(help_text))
	{
	}

	void CommandReadonlyName::onClick(Click& click)
	{
		Util::copy_to_clipboard(click, menu_name.getLocalisedUtf8());
	}
}
