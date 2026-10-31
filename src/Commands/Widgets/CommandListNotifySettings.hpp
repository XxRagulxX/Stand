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
		bool is_game_type = false;

		explicit CommandListNotifySettings(CommandList* parent, CommandColourCustom* primaryColour, CommandColourCustom* bgColour);
		~CommandListNotifySettings();

		void onActiveListUpdate() final;
		void onBack(ThreadContext thread_context) final;

		void setToaster(ThreadContext thread_context, Toaster* toaster);
		void updateGameColour(ThreadContext thread_context);

		static void showPersistentToast();
		static void hidePersistentToast();
	};
}
