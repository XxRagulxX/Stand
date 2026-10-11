#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandColourCustom;
	class Toaster;

	class CommandListNotifySettings : public CommandList
	{
	public:
		CommandList* timing;
		bool is_active = false;

		explicit CommandListNotifySettings(CommandList* parent, CommandColourCustom* primaryColour, CommandColourCustom* bgColour);

		void onActiveListUpdate() final;

		void setToaster(ThreadContext thread_context, Toaster* toaster);
		void updateGameColour(ThreadContext thread_context);
	};
}
