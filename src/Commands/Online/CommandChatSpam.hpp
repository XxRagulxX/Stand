#pragma once

#include "Commands/Widgets/CommandToggleUltimate.hpp"

#include "Network/Chat.hpp"
#include "Commands/Widgets/CommandInput.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Scripting/Script.hpp"

namespace Stand
{
	class CommandChatSpam : public CommandToggleUltimate
	{
	private:
		CommandInput* const text;
		CommandSlider* const interval_ms;
		CommandToggleNoCorrelation* const local;
		
	public:
		explicit CommandChatSpam(CommandList* parent, CommandInput* text, CommandSlider* interval_ms, CommandToggleNoCorrelation* local)
			: CommandToggleUltimate(parent, LOC("SPAM"), { CMDNAME("spam") }, LOC("SPAM_H")), text(text), interval_ms(interval_ms), local(local)
		{
		}

		void onEnableInner(Click& click) final
		{
			ensureYieldableScriptThread(click, [this]
			{
				while (m_on)
				{
					Chat::sendMessage(text->value, false, local->m_on, true);

					time_t dont_resume_before = get_current_time_millis() + interval_ms->value;
					do
					{
						Script::current()->yield();
					} while (get_current_time_millis() < dont_resume_before
						&& m_on
						);
				}
			});
		}
	};
}
