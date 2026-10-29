#pragma once

#include "Commands/Widgets/CommandTab.hpp"
#include "Commands/Stand/Appearance/CommandTabAppearanceDirect.hpp"
#include "Commands/Stand/Appearance/CommandTabBorder.hpp"
#include "Commands/Stand/Appearance/CommandTabCommands.hpp"
#include "Commands/Stand/Appearance/CommandTabEntityPreviews.hpp"
#include "Commands/Stand/Appearance/CommandTabFontText.hpp"
#include "Commands/Widgets/CommandListNotifySettings.hpp"
#include "Commands/Stand/CommandMenuPosition.hpp"
#include "Commands/Stand/Appearance/CommandTabPresetFont.hpp"
#include "Commands/Stand/Appearance/CommandTabTextures.hpp"
#include "Commands/Stand/Input/CommandTabInput.hpp"

namespace Stand
{
	class CommandTabStand : public CommandTab
	{
	private:
		CommandList* secret = nullptr;

	public:
		CommandMenuPosition* position = nullptr;
		CommandTabCommands* commands = nullptr;
		CommandTabBorder* border = nullptr;
		CommandTabTextures* textures = nullptr;
		CommandTabEntityPreviews* entityPreviews = nullptr;
		CommandListNotifySettings* notifications = nullptr;
		CommandTabFontText* fontText = nullptr;
		CommandTabPresetFont* presetFont = nullptr;
		CommandTabAppearanceDirect* appearanceDirect = nullptr;
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
