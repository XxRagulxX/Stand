#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"

#include <string>

namespace Stand
{
	class CommandLeftTextures : public CommandSlider
	{
		static constexpr const char* kLabels[5] = {
		    "Disabled",
		    "All Commands",
		    "All Commands, Compact",
		    "Toggles Only",
		    "Toggles Only, Compact",
		};

	public:
		explicit CommandLeftTextures(CommandList* const parent)
			: CommandSlider(parent, LIT("Leftbound"), CMDNAMES("lefttextures"),
				NOLABEL, 0, 4, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			return kLabels[std::clamp(value, 0, 4)];
		}

		void onChange(Click& click, int prev_value) override
		{
			using M = Rendering::Theme::LeftTexturesMode;
			switch (value)
			{
			case 1: Rendering::Theme::kLeftTextures = M::AllCommands; break;
			case 2: Rendering::Theme::kLeftTextures = M::AllCommandsCompact; break;
			case 3: Rendering::Theme::kLeftTextures = M::TogglesOnly; break;
			case 4: Rendering::Theme::kLeftTextures = M::TogglesOnlyCompact; break;
			default: Rendering::Theme::kLeftTextures = M::Disabled; break;
			}
		}
	};

	class CommandTabTextures : public CommandList
	{
	public:
		CommandLeftTextures* const leftTextures;

		explicit CommandTabTextures()
			: CommandList(nullptr, LIT("Textures"), CMDNAMES())
			, leftTextures(createChild<CommandLeftTextures>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabTextures& GetCommandTabTextures();
}
