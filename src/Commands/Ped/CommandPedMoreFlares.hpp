#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Core/AbstractEntity.hpp"
#include "Game/gta_ped.hpp"

namespace Stand
{
	class CommandPedMoreFlares : public CommandToggle
	{
	public:
		explicit CommandPedMoreFlares(CommandList* const parent)
			: CommandToggle(parent, LOC("INCFLARE"), {}, LOC("INCPROJ_H"))
		{
		}

		void onChange(Click& click) final
		{
			onChangeToggleScriptTickEventHandler(click, []
			{
				SOUP_IF_LIKELY(auto p = g_player_ped.getCPed())
				{
					p->m_uFlareGunProjCount = 0;
				}
			});
		}
	};
}
