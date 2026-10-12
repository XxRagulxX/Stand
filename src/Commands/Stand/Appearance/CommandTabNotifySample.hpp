#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Rendering/Notifications.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	class CommandNotifySampleSmall : public CommandPhysical
	{
	public:
		explicit CommandNotifySampleSmall(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Small"), CMDNAMES("notifysamplesmall"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			Notifications::Show("StandEnhanced", "Hi");
		}
	};

	class CommandNotifySampleMedium : public CommandPhysical
	{
	public:
		explicit CommandNotifySampleMedium(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Medium"), CMDNAMES("notifysamplemedium"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			Notifications::Show("StandEnhanced", "This is a sample notification with a medium-length body text.");
		}
	};

	class CommandNotifySampleLarge : public CommandPhysical
	{
	public:
		explicit CommandNotifySampleLarge(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Large"), CMDNAMES("notifysamplelarge"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			Notifications::Show("StandEnhanced", "This is a sample notification with a longer body text to demonstrate how the notification panel handles multi-word content and line wrapping.");
		}
	};

	class CommandTabNotifySample : public CommandList
	{
	public:
		CommandNotifySampleSmall* const small_;
		CommandNotifySampleMedium* const medium;
		CommandNotifySampleLarge* const large_;

		explicit CommandTabNotifySample()
			: CommandList(nullptr, LIT("Sample Notifications"))
			, small_(createChild<CommandNotifySampleSmall>())
			, medium(createChild<CommandNotifySampleMedium>())
			, large_(createChild<CommandNotifySampleLarge>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabNotifySample& GetCommandTabNotifySample();
}
