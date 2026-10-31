#pragma once

#include "Commands/Widgets/CommandListAction.hpp"
#include "Commands/Widgets/CommandListActionItem.hpp"

#include "Rendering/StandPort/GridToaster.hpp"
#include "Util/Label.hpp"
#include "Util/lang.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	class CommandSampleToast : public CommandListAction
	{
	public:
		explicit CommandSampleToast(CommandList* const parent)
			: CommandListAction(parent, LOC("SAMPNOT"), {}, NOLABEL, {
				{0, LOC("SIZE_S")},
				{1, LOC("SIZE_M")},
				{2, LOC("SIZE_L")},
			})
		{
		}

		void onItemClick(Click& click, CommandListActionItem* item) final
		{
			CommandListAction::onItemClick(click, item);

			switch (item->value)
			{
			case 0:
				g_grid_toaster.toast(LANG_GET_W("HI"));
				break;

			case 1:
				g_grid_toaster.toast(LANG_GET_W("FREE_T"));
				break;

			case 2:
			{
				std::string msg{ LANG_GET("WOT1") };
				msg.push_back('\n');
				msg.append(LANG_GET("WOT2")).push_back('\n');
				msg.append(LANG_GET("WOT3")).push_back('\n');
				msg.append(LANG_GET("WOT4")).push_back('\n');
				msg.append(LANG_GET("WOT5"));
				g_grid_toaster.toast(StringUtils::utf8_to_utf16(msg));
				break;
			}

			default:
				break;
			}
		}
	};
}
