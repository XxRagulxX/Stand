#pragma once

#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/StandPort/MenuGrid.hpp"
#include "Rendering/StandPort/StandRendererCompat.hpp"

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
			g_MenuGrid.updateSetting([this]
			{
				g_renderer.address_current_list_only = m_on;
			});
		}
	};
}
