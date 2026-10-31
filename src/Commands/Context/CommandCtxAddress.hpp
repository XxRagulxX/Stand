#pragma once
#include "Commands/Widgets/CommandListSelect.hpp"

#include "Menu/ContextMenu.hpp"
#include "Rendering/Clipboard.hpp"

namespace Stand
{
	class CommandCtxAddress : public CommandListSelect
	{
	public:
		explicit CommandCtxAddress(CommandList* const parent)
			: CommandListSelect(parent, LOC("CTX_ADDR"), {}, NOLABEL, {
				{0, LOC("CTX_ADDR_0")},
				{1, LOC("CTX_ADDR_1")},
				{2, LOC("CTX_ADDR_2")},
				{3, LOC("CTX_ADDR_3")},
			}, 0)
		{
		}

		void onClick(Click& click) final
		{
			auto* target = ContextMenu::getTarget();
			if (value == 0)
			{
				Rendering::Clipboard::SetText(target->getLocalisedAddress(" > "));
			}
			else if (value == 1)
			{
				Rendering::Clipboard::SetText(target->getLocalisedAddress(" > "));
			}
			else if (value == 2)
			{
				Rendering::Clipboard::SetText(target->getPathConfig());
			}
			else
			{
				Rendering::Clipboard::SetText("https://stand.sh/focus#" + target->getPathConfig());
			}
		}
	};
}
