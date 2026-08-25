#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Network/evtChatEvent.hpp"
#include "Core/FileLogger.hpp"
#include "Util/get_appdata_path.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	class CommandChatLogAuxfile : public CommandToggle
	{
	private:
		static inline FileLogger chat_txt_out{};

		static void onChatEvent(evtChatEvent& e)
		{
			chat_txt_out.log(e.getLogEntry());
		}

	public:
		explicit CommandChatLogAuxfile(CommandList* const parent)
			: CommandToggle(parent, LIT("Chat.txt"))
		{
		}

		void onEnable(Click& click) final
		{
			const auto file_path = get_appdata_path().append(LR"(\Stand\Chat.txt)");
			chat_txt_out.init(file_path);
			if (!chat_txt_out.isInited())
			{
				Util::toast(LANG_FMT("FWERR", StringUtils::utf16_to_utf8(file_path)), TOAST_ALL);
			}
			evtChatEvent::registerHandler(&onChatEvent);
		}

		void onDisable(Click& click) final
		{
			evtChatEvent::unregisterHandler(&onChatEvent);
			chat_txt_out.deinit();
		}
	};
}
