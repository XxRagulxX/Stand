#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandShowNavMesh : public CommandToggle
	{
	public:
		explicit CommandShowNavMesh(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Nav Mesh"), { CMDNAME("navmesh") })
		{
		}

		void onChange(Click& click) final
		{
			onChangeToggleScriptTickEventHandler(click, [this]() -> bool
			{
				return m_on;
			});
		}
	};
}
