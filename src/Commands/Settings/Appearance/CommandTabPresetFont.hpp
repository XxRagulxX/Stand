#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Rendering/Notifications.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	class CommandPresetFontYahei : public CommandPhysical
	{
	public:
		explicit CommandPresetFontYahei(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Microsoft YaHei"), CMDNAMES("presetfontyahei"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			Notifications::Show("StandEnhanced", "Microsoft YaHei font is not bundled in this build.");
		}
	};

	class CommandPresetFontNanumGothic : public CommandPhysical
	{
	public:
		explicit CommandPresetFontNanumGothic(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Nanum Gothic"), CMDNAMES("presetfontnanumgothic"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			Notifications::Show("StandEnhanced", "Nanum Gothic font is not bundled in this build.");
		}
	};

	class CommandPresetFontBeVietnamProLight : public CommandPhysical
	{
	public:
		explicit CommandPresetFontBeVietnamProLight(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Be Vietnam Pro Light"), CMDNAMES("presetfontbevietnamprolight"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			Notifications::Show("StandEnhanced", "Be Vietnam Pro Light font is not bundled in this build.");
		}
	};

	class CommandTabPresetFont : public CommandList
	{
	public:
		CommandPresetFontYahei* const yahei;
		CommandPresetFontNanumGothic* const nanumGothic;
		CommandPresetFontBeVietnamProLight* const beVietnamProLight;

		explicit CommandTabPresetFont()
			: CommandList(nullptr, LIT("Preset Fonts"))
			, yahei(createChild<CommandPresetFontYahei>())
			, nanumGothic(createChild<CommandPresetFontNanumGothic>())
			, beVietnamProLight(createChild<CommandPresetFontBeVietnamProLight>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabPresetFont& GetCommandTabPresetFont();
}
