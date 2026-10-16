#pragma once

#include <fstream>
#include <unordered_map>

#include "lib/soup/cat.hpp"
#include "lib/soup/catTreeReader.hpp"
#include "lib/soup/IstreamReader.hpp"

#include "Commands/Stand/CommandProfile.hpp"
#include "Rendering/StandPort/CommandAction.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Core/ExceptionHandler.hpp"
#include "Rendering/Gui.hpp"
#include "Scripting/FiberPool.hpp"

namespace Stand
{
	class CommandProfileLoad : public CommandAction
	{
	public:
		explicit CommandProfileLoad(CommandList* const parent)
			: CommandAction(parent, LOC("LOAD"), { CMDNAME("load"), CMDNAME("loadprofile") }, LOC("LSTATE_H2"))
		{
			addSuffixToCommandNames(parent->menu_name.getLiteralForCommandName());
		}

		void onClick(Click& click) final
		{
			click.stopInputIfAllowed();
			click.setNoResponse();
			Exceptional::createManagedExceptionalThread([this]
			{
				std::wstring path = parent->as<CommandProfile>()->getFilePath();
				std::ifstream file_in(path);
				if (!file_in.is_open())
					return;
				soup::IstreamReader r(file_in);
				auto root = soup::cat::parse(r);
				if (!root)
					return;
				soup::catTreeReader tr;
				auto state = tr.toMap(root.get(), false);
				FiberPool::queueJob([state = std::move(state)]() mutable
				{
					g_gui.applyDefaultState();
					g_gui.applyStateFromMemory(std::move(state));
				});
			});
		}
	};
}
