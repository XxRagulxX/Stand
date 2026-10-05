#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Rendering/StandPort/Direction.hpp"
#include "Rendering/Gui.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandDivider : public CommandPhysical
	{
	public:
		inline static bool selectable = false;

		explicit CommandDivider(CommandList* const parent, Label&& menu_name)
		    : CommandPhysical(COMMAND_DIVIDER, parent, std::move(menu_name), {}, NOLABEL, CMDFLAG_FEATURELIST_SKIP)
		{
			if (parent != nullptr)
				++parent->dividers;
		}

		void preDelete() override
		{
			if (parent != nullptr)
				--parent->dividers;
		}

		void onFocus() override
		{
			if (!selectable)
			{
				if (g_gui.getCurrentUiList()->canUpdateCursor())
					g_gui.inputDown(TC_OTHER);
			}
		}
	};
}
