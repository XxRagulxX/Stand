#pragma once

#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/StandPort/MenuGrid.hpp"

namespace Stand
{
	class CommandAddressbarWidthAffectedByColumns : public CommandToggle
	{
	public:
		explicit CommandAddressbarWidthAffectedByColumns(CommandList* parent)
			: CommandToggle(parent, LOC("WDTAFFBCLMNS"))
		{
		}

		void onChange(Click& click) final
		{
			g_MenuGrid.updateSetting([this]
			{
				g_MenuGrid.addressbar_width_affected_by_columns = m_on;
			});
		}
	};
}
