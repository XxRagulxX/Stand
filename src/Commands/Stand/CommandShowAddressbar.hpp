#pragma once

#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

namespace Stand
{
	class CommandShowAddressbar : public CommandToggle
	{
	public:
		explicit CommandShowAddressbar(CommandList* const parent)
			: CommandToggle(parent, LOC("ADDRBAR"), { CMDNAME("addressbar") }, NOLABEL, true)
		{
		}

		void onChange(Click& click) final
		{
			Rendering::Theme::kAddressBarVisible = m_on;
			Rendering::GridRenderer::InvalidateMenuLayout();
		}
	};
}
