#pragma once

#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/StandPort/MenuGrid.hpp"

namespace Stand
{
	class CommandAddressbarWidthAffectedByScrollbar : public CommandToggle
	{
	public:
		explicit CommandAddressbarWidthAffectedByScrollbar(CommandList* parent)
			: CommandToggle(parent, LOC("WDTAFFBSCRLBR"))
		{
		}

		void onChange(Click& click) final
		{
			g_MenuGrid.updateSetting([this]
			{
				g_MenuGrid.addressbar_width_affected_by_scrollbar = m_on;
			});
		}
	};
}
