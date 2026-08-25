#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Core/AbstractEntity.hpp"
#include "Game/gta_input.hpp"
#include "Core/input.hpp"
#include "Game/natives.hpp"

namespace Stand
{
	class CommandSuperHandbrake : public CommandToggle
	{
	public:
		inline static std::vector<Hotkey> default_hotkeys{ VK_SPACE };

		explicit CommandSuperHandbrake(CommandList* const parent)
			: CommandToggle(parent, LOC("SPRHBRK"), { CMDNAME("superhandbrake"), CMDNAME("superhandbreak") }, NOLABEL, false, CMDFLAGS_TOGGLE | CMDFLAG_TUTORIAL_CTX_POINT, COMMANDPERM_USERONLY, default_hotkeys)
		{
		}

		[[nodiscard]] const std::vector<Hotkey>& getDefaultHotkeys() const noexcept final
		{
			return default_hotkeys;
		}

		void onEnable(Click& click) final
		{
			registerScriptTickEventHandler(click, [this]()
			{
				if (!m_on)
				{
					return false;
				}
				if (g_player_veh.isValid()
					&& Input::isAnyKeyPressed(hotkeys, INPUT_VEH_HANDBRAKE)
					&& g_player_veh.isOwnerOfVehicleAndDriver()
					)
				{
					ENTITY::SET_ENTITY_VELOCITY(g_player_veh, 0, 0, 0);
				}
				return true;
			});
		}
	};
}
