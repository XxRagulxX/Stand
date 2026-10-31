#pragma once

#include "Commands/Widgets/CommandListSelect.hpp"
#include "Menu/Hotkey.hpp"

namespace Stand
{
	class CommandHotkeyModkeyBehaviour : public CommandListSelect
	{
	public:
		explicit CommandHotkeyModkeyBehaviour(CommandList* const parent)
			: CommandListSelect(parent, LOC("HMB"), {}, LOC("HMB_H"), {
				{(long long)HMB_STRICT, LOC("STRCT")},
				{(long long)HMB_STRICT_IF_HAS_MODKEY, LOC("SMRT")},
				{(long long)HMB_LAX, LOC("LAX")},
			}, (long long)HMB_STRICT_IF_HAS_MODKEY)
		{
		}

		void onChange(Click& click, long long prev_value) final
		{
			Hotkey::modkey_behaviour = (HotkeyModkeyBehaviour)value;
		}
	};
}
