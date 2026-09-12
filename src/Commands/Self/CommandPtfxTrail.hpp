#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Util/Label.hpp"

#include <ctime>

namespace Stand
{
	class CommandPtfxTrail final : public CommandToggle
	{
	public:
		int    bone;
		time_t last_applied = 0;

		explicit CommandPtfxTrail(CommandList* parent, int bone, const char* name,
		                          const char* cmdname, bool default_on)
			: CommandToggle(parent, LIT(name), CMDNAMES_1(cmdname), NOLABEL, default_on),
			  bone(bone)
		{
		}
	};
}
