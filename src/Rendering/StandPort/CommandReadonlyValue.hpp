#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "lib/soup/unicode.hpp"

namespace Stand
{
	class CommandReadonlyValue : public CommandPhysical
	{
	public:
		std::wstring value;

		explicit CommandReadonlyValue(CommandList* parent, Label&& menu_name, Label&& help_text = NOLABEL, commandflags_t flags = CMDFLAGS_READONLY_VALUE, std::wstring&& value = {})
		    : CommandPhysical(COMMAND_READONLY_VALUE, parent, std::move(menu_name), {}, std::move(help_text), flags)
		    , value(std::move(value))
		{
		}

		void setValue(const std::string& v)
		{
			value = soup::unicode::utf8_to_utf16(v);
		}

		void setValue(std::wstring&& v)
		{
			value = std::move(v);
		}
	};
}
