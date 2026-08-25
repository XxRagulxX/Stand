#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Game/pointers.hpp"

namespace Stand
{
	class CommandUncapFps : public CommandToggle
	{
	public:
		explicit CommandUncapFps(CommandList* const parent)
			: CommandToggle(parent, LOC("UNCAPFPS"), CMDNAMES("uncapfps", "removefpslimit"), LOC("UNCAPFPS_H"))
		{
		}

		void onChange(Click& click) final
		{
			*pointers::audNorthAudioEngine__sm_RunUpdateInSeperateThread = !m_on;
		}
	};
}
