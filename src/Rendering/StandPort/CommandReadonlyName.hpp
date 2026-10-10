#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "lib/soup/os.hpp"

namespace Stand
{
	class CommandReadonlyName : public CommandPhysical
	{
	public:
		explicit CommandReadonlyName(CommandList* const parent, Label&& menu_name, Label&& help_text = NOLABEL)
			: CommandPhysical(COMMAND_READONLY_NAME, parent, std::move(menu_name), {}, std::move(help_text))
		{
		}

		void onClick(Click& click) final
		{
			soup::os::copyToClipboard(menu_name.getLocalisedUtf8());
		}
	};
}
