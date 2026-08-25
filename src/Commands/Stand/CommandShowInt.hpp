#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Core/AbstractEntity.hpp"
#include "Rendering/drawDebugText.hpp"
#include "Game/natives.hpp"

namespace Stand
{
	class CommandShowInt : public CommandToggle
	{
	public:
		explicit CommandShowInt(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Interior ID"), { CMDNAME("showinteriorid") })
		{
		}

		void onChange(Click& click) final
		{
			onChangeToggleScriptTickEventHandler(click, []()
			{
				drawInfoText(fmt::to_string(INTERIOR::GET_INTERIOR_FROM_ENTITY(g_player_ped)), "Interior ID");
			});
		}
	};
}
