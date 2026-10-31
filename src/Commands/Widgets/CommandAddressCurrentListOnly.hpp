#pragma once

#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/Theme.hpp"

namespace Stand
{
	class CommandAddressCurrentListOnly : public CommandToggle
	{
	public:
		explicit CommandAddressCurrentListOnly(CommandList* parent)
			: CommandToggle(parent, LOC("SHWCRLSTNLY"))
		{
		}

		void onChange(Click& click) final
		{
			Rendering::Theme::kAddressCurrentListOnly = m_on;
		}
	};
}
