#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Game/atArray.hpp"
#include "Game/ControlMgr.hpp"
#include "Game/natives.hpp"
#include "Game/pointers.hpp"
#include "Game/script_thread.hpp"

namespace Stand
{
	class CommandMobileRadio : public CommandToggle
	{
	public:
		explicit CommandMobileRadio(CommandList* const parent)
			: CommandToggle(parent, LOC("MBLRD"), { CMDNAME("ipod"), CMDNAME("mobileradio") }, LOC("MBLRD_H"))
		{
		}

		void onEnable(Click& click) final
		{
			ControlMgr::mobile_radio = true;
			ControlMgr::update();
			registerScriptTickEventHandler(click, [this]
			{
				HANDLER_CHECK(m_on);
				AUDIO::SET_AUDIO_FLAG("AllowRadioDuringSwitch", m_on);
				AUDIO::SET_AUDIO_FLAG("MobileRadioInGame", m_on);
				HANDLER_END
			});
		}

		void onDisable(Click& click) final
		{
			ControlMgr::mobile_radio = false;
			ControlMgr::update();
			for (const auto& t : *pointers::script_threads)
			{
				t->spoofAs([]
				{
					AUDIO::SET_AUDIO_FLAG("AllowRadioDuringSwitch", FALSE);
					AUDIO::SET_AUDIO_FLAG("MobileRadioInGame", FALSE);
				});
			}
		}
	};
}
