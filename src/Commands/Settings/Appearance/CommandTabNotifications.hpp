#pragma once
#include "Commands/CommandColourCustom.hpp"
#include "Commands/Commands.hpp"
#include "Commands/Settings/Appearance/CommandTabColours.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Rendering/NotifySettings.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"
#include "Util/Joaat.hpp"

#include <algorithm>
#include <string>

namespace Stand
{
	class CommandNotifyType : public CommandSlider
	{
		static constexpr const char* kLabels[3] = {"Game", "Stand, Next To Map", "Stand, Custom Position"};

	public:
		explicit CommandNotifyType(CommandList* const parent)
			: CommandSlider(parent, LIT("Type"), CMDNAMES("notifytype"), NOLABEL, 0, 2, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			return kLabels[std::clamp(value, 0, 2)];
		}

		void onChange(Click& click, int prev_value) override
		{
			using T = Rendering::NotifySettings::Type;
			switch (value)
			{
			case 1: Rendering::NotifySettings::kType = T::StandNextToMap; break;
			case 2: Rendering::NotifySettings::kType = T::StandCustomPosition; break;
			default: Rendering::NotifySettings::kType = T::Game; break;
			}
		}
	};

	class CommandNotifyInvertFlow : public CommandToggle
	{
	public:
		explicit CommandNotifyInvertFlow(CommandList* const parent)
			: CommandToggle(parent, LIT("Invert Flow"), CMDNAMES("notifyinvertflow"), NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::NotifySettings::kInvertFlow = m_on;
		}
	};

	class CommandNotifyWidth : public CommandSlider
	{
	public:
		explicit CommandNotifyWidth(CommandList* const parent)
			: CommandSlider(parent, LIT("Width"), CMDNAMES("notifywidth"), NOLABEL, 0, 15000, 400, 10)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::NotifySettings::kWidth = static_cast<float>(value);
		}
	};

	class CommandNotifyPadding : public CommandSlider
	{
	public:
		explicit CommandNotifyPadding(CommandList* const parent)
			: CommandSlider(parent, LIT("Padding"), CMDNAMES("notifypadding"), NOLABEL, 0, 1000, 0, 10)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::NotifySettings::kPadding = static_cast<float>(value);
		}
	};

	class CommandNotifyBorderRainbow : public CommandColourRainbow
	{
	public:
		explicit CommandNotifyBorderRainbow(CommandList* const parent)
			: CommandColourRainbow(parent, LIT("Rainbow Mode"), CMDNAMES("notifyborderrainbow"),
				Rendering::NotifySettings::kBorderColour)
		{
		}
	};

	class CommandNotifyCopyPrimary : public CommandPhysical
	{
	public:
		explicit CommandNotifyCopyPrimary(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Copy Primary Colour"), CMDNAMES("notifycopyprimary"), NOLABEL)
		{
		}

		void onClick(Click& click) override;
	};

	class CommandNotifyFlashRainbow : public CommandColourRainbow
	{
	public:
		explicit CommandNotifyFlashRainbow(CommandList* const parent)
			: CommandColourRainbow(parent, LIT("Rainbow Mode"), CMDNAMES("notifyflashrainbow"),
				Rendering::NotifySettings::kFlashColour)
		{
		}
	};

	class CommandNotifyCopyBg : public CommandPhysical
	{
	public:
		explicit CommandNotifyCopyBg(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Copy Background Colour"), CMDNAMES("notifycopybg"), NOLABEL)
		{
		}

		void onClick(Click& click) override;
	};

	class CommandNotifyFlashNow : public CommandPhysical
	{
	public:
		explicit CommandNotifyFlashNow(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Flash Notification"), CMDNAMES("notifyflashnow"), NOLABEL)
		{
		}

		void onClick(Click& click) override;
	};

	class CommandTabNotifications : public CommandList
	{
	public:
		CommandNotifyType* const type;
		CommandNotifyInvertFlow* const invertFlow;
		CommandNotifyWidth* const width;
		CommandNotifyPadding* const padding;
		CommandColourCustom* const borderColour;
		CommandNotifyBorderRainbow* const borderRainbow;
		CommandNotifyCopyPrimary* const copyPrimary;
		CommandColourCustom* const flashColour;
		CommandNotifyFlashRainbow* const flashRainbow;
		CommandColourCustom* const bgColour;
		CommandNotifyCopyBg* const copyBg;
		CommandNotifyFlashNow* const flashNow;

		explicit CommandTabNotifications()
			: CommandList(nullptr, LIT("Notifications"))
			, type(createChild<CommandNotifyType>())
			, invertFlow(createChild<CommandNotifyInvertFlow>())
			, width(createChild<CommandNotifyWidth>())
			, padding(createChild<CommandNotifyPadding>())
			, borderColour(Commands::GetCommand<CommandColourCustom>("notifyborder"_J))
			, borderRainbow(createChild<CommandNotifyBorderRainbow>())
			, copyPrimary(createChild<CommandNotifyCopyPrimary>())
			, flashColour(Commands::GetCommand<CommandColourCustom>("notifyflash"_J))
			, flashRainbow(createChild<CommandNotifyFlashRainbow>())
			, bgColour(Commands::GetCommand<CommandColourCustom>("notifybg"_J))
			, copyBg(createChild<CommandNotifyCopyBg>())
			, flashNow(createChild<CommandNotifyFlashNow>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabNotifications& GetCommandTabNotifications();
}
