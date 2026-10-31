#include "Commands/Widgets/CommandAddLabel.hpp"

#include <fmt/core.h>

#include "Commands/Widgets/CommandEditLabels.hpp"
#include "Rendering/StandPort/get_next_arg.hpp"
#include "Core/Hooking.hpp"
#include "Util/lang.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	CommandAddLabel::CommandAddLabel(CommandList* const parent)
		: CommandAction(parent, LOC("ADDLB"), CMDNAMES("addlabel"), LOC("ADDLB_H"))
	{
	}

	void CommandAddLabel::onCommand(Click& click, std::wstring& args)
	{
		click.setNoResponse();
		auto _arg = get_next_arg(args);
		if (_arg.empty())
		{
			return onClick(click);
		}
		auto arg = StringUtils::utf16_to_utf8(_arg);
		ensureScriptThread(click, [this, tc{ click.thread_context }, arg{ std::move(arg) }]() mutable
		{
			StringUtils::to_upper(arg);
			if (arg != "CELL_EMAIL_BCON")
			{
				auto hash = CommandEditLabels::keyToHash(arg);
				if (const char* text = g_hooking.getLabelText(hash); text != nullptr)
				{
					parent->as<CommandEditLabels>()->addItem(tc, std::move(arg), text);
					return;
				}
			}
			Util::toast(LANG_FMT("ADDLB_E", arg));
		});
	}
}
