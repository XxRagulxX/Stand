#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandShowNavMeshNeighbors : public CommandToggle
	{
	public:
		explicit CommandShowNavMeshNeighbors(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Nav Mesh Neighbors"))
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
