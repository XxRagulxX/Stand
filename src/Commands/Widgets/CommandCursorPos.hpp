#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/Theme.hpp"

namespace Stand
{
	class CommandCursorPos : public CommandToggle
	{
	public:
		explicit CommandCursorPos(CommandList* const parent)
			: CommandToggle(parent, LOC("SHWCRSPOS"), CMDNAMES("cursorpos", "showcursorpos"), LOC("SHWCRSPOS_H"))
		{
		}

		void onChange(Click& click) final
		{
			Rendering::Theme::kShowCursorPos = m_on;
		}
	};
}
