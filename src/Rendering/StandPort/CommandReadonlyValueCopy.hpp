#pragma once
#include "Rendering/StandPort/CommandReadonlyValue.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	class CommandReadonlyValueCopy : public CommandReadonlyValue
	{
	public:
		explicit CommandReadonlyValueCopy(CommandList* const parent, Label&& menu_name, std::wstring&& value = {}, const commandflags_t flags = CMDFLAGS_READONLY_VALUE_COPY)
		    : CommandReadonlyValue(parent, std::move(menu_name), LIT("Click to copy"), flags, std::move(value))
		{
		}

		void onClick(Click& click) override;
	};
}
