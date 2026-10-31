#include "Commands/Widgets/CommandScrollbar.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

namespace Stand
{
	CommandScrollbar::CommandScrollbar(CommandList* parent)
		: CommandListSelect(parent, LOC("SCRLBR"), {}, NOLABEL, {
			{(long long)Rendering::Theme::ScrollbarMode::Enabled, LOC("ON")},
			{(long long)Rendering::Theme::ScrollbarMode::EnabledWhenNeeded, LOC("SCRLBR_1")},
			{(long long)Rendering::Theme::ScrollbarMode::Disabled, LOC("DOFF")},
		}, (long long)Rendering::Theme::ScrollbarMode::EnabledWhenNeeded)
	{
	}

	void CommandScrollbar::onChange(Click& click, long long prev_value)
	{
		Rendering::Theme::kScrollbarMode = (Rendering::Theme::ScrollbarMode)value;
		Rendering::GridRenderer::InvalidateMenuLayout();
	}
}
