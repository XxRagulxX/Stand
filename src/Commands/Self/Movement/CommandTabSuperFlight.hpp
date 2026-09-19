#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

#include <algorithm>
#include <cmath>

namespace Stand
{
	class CommandSuperFlightSpeed : public CommandSliderFloat
	{
	public:
		explicit CommandSuperFlightSpeed(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Super Flight"), CMDNAMES("superflight"), NOLABEL, -1000000, 1000000, 100)
		{
		}
	};

	class CommandSuperFlightJumpToFlight : public CommandToggle
	{
	public:
		explicit CommandSuperFlightJumpToFlight(CommandList* const parent)
			: CommandToggle(parent, LIT("Button Instructions: Super Jump To Flight"), CMDNAMES("superflightjumptoflight"), NOLABEL)
		{
		}
	};

	class CommandSuperFlightControls : public CommandToggle
	{
	public:
		explicit CommandSuperFlightControls(CommandList* const parent)
			: CommandToggle(parent, LIT("Button Instructions: Controls"), CMDNAMES("superflightcontrols"), NOLABEL)
		{
		}
	};

	class CommandSuperFlightMode : public CommandToggle
	{
		CommandSliderFloat* const m_speed;

		static constexpr float kPi = 3.14159265358979323846f;

	public:
		explicit CommandSuperFlightMode(CommandList* const parent, CommandSliderFloat* speed)
			: CommandToggle(parent, LIT("Super Flight"), CMDNAMES("superflightmode"), NOLABEL),
			  m_speed(speed)
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;

			if (!ENTITY::IS_ENTITY_IN_AIR(ped.GetHandle()))
				return;

			const float speed = m_speed->getFloatValue();
			const auto rot = CAMERA::GET_GAMEPLAY_CAM_ROT(2);
			const float pitch = rot.x * (kPi / 180.0f);
			const float yaw = rot.z * (kPi / 180.0f);

			ENTITY::SET_ENTITY_VELOCITY(
				ped.GetHandle(),
				std::cos(pitch) * std::sin(-yaw) * speed,
				std::cos(pitch) * std::cos(-yaw) * speed,
				std::sin(pitch) * speed);
		}

		~CommandSuperFlightMode() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};

	class CommandTabSuperFlight : public CommandList
	{
	public:
		CommandSuperFlightSpeed* const speed;
		CommandSuperFlightJumpToFlight* const jumpToFlight;
		CommandSuperFlightControls* const controls;
		CommandSuperFlightMode* const mode;

		explicit CommandTabSuperFlight()
			: CommandList(nullptr, LIT("Super Flight")),
			  speed(createChild<CommandSuperFlightSpeed>()),
			  jumpToFlight(createChild<CommandSuperFlightJumpToFlight>()),
			  controls(createChild<CommandSuperFlightControls>()),
			  mode(createChild<CommandSuperFlightMode>(speed))
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabSuperFlight& GetCommandTabSuperFlight();
}
