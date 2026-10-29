#pragma once

#include "Commands/Widgets/CommandSlider.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

namespace Stand
{
	class CommandInfoTextPadding : public CommandSlider
	{
	public:
		explicit CommandInfoTextPadding(CommandList* parent)
			: CommandSlider(parent, LOC("PDDGN"), { CMDNAME("infopadding") }, NOLABEL, -100, 100, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) final
		{
			Rendering::Theme::kInfoPadding = static_cast<int16_t>(value);
			Rendering::GridRenderer::InvalidateMenuLayout();
		}
	};
}
