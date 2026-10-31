#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Game/AbstractEntity.hpp"
#include "Scripting/Natives.hpp"

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
			onChangeToggleScriptTickEventHandler(click, [this]() -> bool
			{
				if (!m_on)
				{
					return false;
				}
				[[maybe_unused]] auto interior_id = INTERIOR::GET_INTERIOR_FROM_ENTITY(g_player_ped.getHandle());
				return true;
			});
		}
	};
}
