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
	class CommandFloppySpeed : public CommandSliderFloat
	{
	public:
		explicit CommandFloppySpeed(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Movement Speed"), CMDNAMES("floppyspeed"), NOLABEL, 1, 1000000, 100)
		{
		}
	};

	class CommandFloppySprint : public CommandSliderFloat
	{
	public:
		explicit CommandFloppySprint(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Sprint Multiplier"), CMDNAMES("floppysprint"), LIT("How fast you go while you press [Space]."), 1, 500, 150)
		{
		}
	};

	class CommandFloppyInstructions : public CommandToggle
	{
	public:
		explicit CommandFloppyInstructions(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Button Instructions"), CMDNAMES("floppyinstructions"), NOLABEL)
		{
		}
	};

	class CommandFloppySynch : public CommandToggle
	{
	public:
		explicit CommandFloppySynch(CommandList* const parent)
			: CommandToggle(parent, LIT("Prioritise Syncing"), CMDNAMES("floppysynch"), LIT("Caps the speed and disables ascend, descend, and sprint to ensure this will sync."))
		{
		}
	};

	class CommandFloppy : public CommandToggle
	{
		CommandSliderFloat* const m_speed;
		CommandSliderFloat* const m_sprint;
		CommandToggle* const m_synch;

		static constexpr float kPi = 3.14159265358979323846f;

	public:
		explicit CommandFloppy(CommandList* const parent, CommandSliderFloat* speed, CommandSliderFloat* sprint, CommandToggle* synch)
			: CommandToggle(parent, LIT("Floppy Mode"), CMDNAMES("floppy"), NOLABEL),
			  m_speed(speed),
			  m_sprint(sprint),
			  m_synch(synch)
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			if (!Stand::Self::GetPed())
				return;
			ENTITY::SET_ENTITY_HAS_GRAVITY(Stand::Self::GetPed().GetHandle(), true);
			PED::SET_PED_GRAVITY(Stand::Self::GetPed().GetHandle(), true);
		}

		void onTick() override
		{
			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;

			const auto handle = ped.GetHandle();
			ENTITY::SET_ENTITY_HAS_GRAVITY(handle, false);
			PED::SET_PED_GRAVITY(handle, false);

			const auto rot = CAMERA::GET_GAMEPLAY_CAM_ROT(2);
			const float yaw = rot.z * (kPi / 180.0f);

			const bool syncing = m_synch->m_on;
			float speed = m_speed->getFloatValue();
			if (syncing)
				speed = std::min(speed, 5.0f);

			const float fwd_x = std::sin(-yaw);
			const float fwd_y = std::cos(-yaw);
			const float rgt_x = std::cos(yaw);
			const float rgt_y = std::sin(yaw);

			const bool sprinting = !syncing && PAD::IS_CONTROL_PRESSED(0, 22);
			const float effectiveSpeed = sprinting ? speed * m_sprint->getFloatValue() : speed;

			float vx = 0.0f, vy = 0.0f, vz = 0.0f;
			if (PAD::IS_CONTROL_PRESSED(0, 32)) { vx += fwd_x * effectiveSpeed; vy += fwd_y * effectiveSpeed; }
			if (PAD::IS_CONTROL_PRESSED(0, 33)) { vx -= fwd_x * effectiveSpeed; vy -= fwd_y * effectiveSpeed; }
			if (PAD::IS_CONTROL_PRESSED(0, 34)) { vx -= rgt_x * effectiveSpeed; vy -= rgt_y * effectiveSpeed; }
			if (PAD::IS_CONTROL_PRESSED(0, 35)) { vx += rgt_x * effectiveSpeed; vy += rgt_y * effectiveSpeed; }

			if (!syncing)
			{
				if (sprinting)
					vz += speed;
				if (PAD::IS_CONTROL_PRESSED(0, 36))
					vz -= speed;
			}

			ENTITY::SET_ENTITY_VELOCITY(handle, vx, vy, vz);
		}

		~CommandFloppy() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};

	class CommandTabFloppy : public CommandList
	{
	public:
		CommandFloppySpeed* const speed;
		CommandFloppySprint* const sprint;
		CommandFloppyInstructions* const instructions;
		CommandFloppySynch* const synch;
		CommandFloppy* const floppy;

		explicit CommandTabFloppy()
			: CommandList(nullptr, LIT("Floppy Mode")),
			  speed(createChild<CommandFloppySpeed>()),
			  sprint(createChild<CommandFloppySprint>()),
			  instructions(createChild<CommandFloppyInstructions>()),
			  synch(createChild<CommandFloppySynch>()),
			  floppy(createChild<CommandFloppy>(speed, sprint, synch))
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabFloppy& GetCommandTabFloppy();
}
