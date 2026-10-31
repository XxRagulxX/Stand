#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandApartments : public CommandToggle
	{
	public:
		explicit CommandApartments(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Apartments"), CMDNAMES("showapartments"), LIT("Pos. ID -> Apt. ID(s)"))
		{
		}

		void onChange(Click& click) final
		{
			onChangeToggleScriptTickEventHandler(click, [this]() -> bool
			{
				if (!m_on)
				{
					return false;
				}
				return true;
			});
		}
	};
}
