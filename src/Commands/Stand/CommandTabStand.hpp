#pragma once

#include "Commands/Widgets/CommandTab.hpp"
#include "Commands/Stand/CommandEntityPreviews.hpp"
#include "Commands/Widgets/CommandListNotifySettings.hpp"
#include "Commands/Stand/CommandMenuPosition.hpp"
#include "Commands/Stand/Input/CommandTabInput.hpp"

namespace Stand
{
	class CommandTabStand : public CommandTab
	{
	private:
		CommandList* secret = nullptr;

	public:
		CommandMenuPosition* position = nullptr;
		CommandEntityPreviews* entityPreviews = nullptr;
		CommandListNotifySettings* notifications = nullptr;
		CommandTabInput* input = nullptr;

		explicit CommandTabStand(CommandList* const parent);

		void populate();

		void onKonamiCodeEntered();
	};
}

namespace Stand::Features
{
	Stand::CommandTabStand& GetCommandTabStand();
}
