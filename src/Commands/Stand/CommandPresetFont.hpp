#pragma once

#include "Commands/Widgets/CommandListAction.hpp"

#include "Commands/Widgets/CommandListActionItem.hpp"
#include "Rendering/Renderer.hpp"

namespace Stand
{
	class CommandPresetFont : public CommandListAction
	{
	public:
		explicit CommandPresetFont(CommandList* parent)
			: CommandListAction(parent, LOC("FNTPRST"), { CMDNAME("presetfont"), CMDNAME("fontpreset") }, NOLABEL, {
				{0, LIT("Microsoft YaHei"), CMDNAMES("yahei", "microsoftyahei"), LOC("FNTPRST_H_YAHEI")},
				{1, LIT("Nanum Gothic"), CMDNAMES("nanumgothic"), LOC("FNTPRST_H_NANUMGOTHIC")},
				{2, LIT("Be Vietnam Pro Light"), CMDNAMES("bevietnamprolight"), LOC("FNTPRST_H_BEVIETNAM")},
			})
		{
		}

		void onItemClick(Click& click, CommandListActionItem* item) final
		{
			CommandListAction::onItemClick(click, item);
			g_renderer.switchToPresetFont(static_cast<int>(item->value));
		}
	};
}
