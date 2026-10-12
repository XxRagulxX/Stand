#pragma once

#include "Commands/Widgets/CommandActionName.hpp"

namespace Stand
{
	class CommandProfilesCreate : public CommandActionName
	{
	public:
		explicit CommandProfilesCreate(CommandList* const parent)
			: CommandActionName(parent, LOC("NEWPRFL"), { CMDNAME("newprofile"), CMDNAME("createprofile") }, LOC("NEWPRFL_H"), CMDFLAGS_ACTION_NAME | CMDFLAG_STATE_AND_CHECK_FINISHLIST)
		{
		}

		void onCommand(Click& click, std::wstring& args) final
		{
		}
	};
}
