#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/Theme.hpp"

namespace Stand
{
	class CommandCursorPosIncludesDividers : public CommandToggle
	{
	public:
		explicit CommandCursorPosIncludesDividers(CommandList* const parent)
			: CommandToggle(parent, LOC("CRSPOSIDIV"))
		{
		}

		void onChange(Click& click) final
		{
			Rendering::Theme::kCursorPosIncludesDividers = m_on;
		}
	};
}
