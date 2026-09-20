#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	class CommandInputPresetFlexible : public CommandPhysical
	{
	public:
		explicit CommandInputPresetFlexible(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Flexible"), CMDNAMES("inputpresetflexible"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			Rendering::Theme::kInputPreset = Rendering::Theme::InputPreset::Flexible;
		}
	};

	class CommandInputPresetTkl : public CommandPhysical
	{
	public:
		explicit CommandInputPresetTkl(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("No Numpad"), CMDNAMES("inputpresettkl"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			Rendering::Theme::kInputPreset = Rendering::Theme::InputPreset::NoNumpad;
		}
	};

	class CommandInputPresetFull : public CommandPhysical
	{
	public:
		explicit CommandInputPresetFull(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Full"), CMDNAMES("inputpresetfull"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			Rendering::Theme::kInputPreset = Rendering::Theme::InputPreset::Numpad;
		}
	};

	class CommandTabInputPresets : public CommandList
	{
	public:
		CommandInputPresetFlexible* const flexible;
		CommandInputPresetTkl* const tkl;
		CommandInputPresetFull* const full;

		explicit CommandTabInputPresets()
			: CommandList(nullptr, LIT("Input Presets"), CMDNAMES())
			, flexible(createChild<CommandInputPresetFlexible>())
			, tkl(createChild<CommandInputPresetTkl>())
			, full(createChild<CommandInputPresetFull>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabInputPresets& GetCommandTabInputPresets();
}
