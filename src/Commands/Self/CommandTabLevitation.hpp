#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Self/CommandLevitate.hpp"
#include "Commands/Self/CommandLevitationPresets.hpp"

namespace Stand
{
	class CommandLevitateSpeed : public CommandSliderFloat
	{
	public:
		explicit CommandLevitateSpeed(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Movement Speed"), CMDNAMES("levitatespeed"), NOLABEL, 1, 1000000, 100, 10)
		{
		}
	};

	class CommandLevitateSprintMultiplier : public CommandSliderFloat
	{
	public:
		explicit CommandLevitateSprintMultiplier(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Sprint Multiplier"), CMDNAMES("levitatesprintmultiplier"), LIT("How fast you go while you press Sprint."), 1, 1000000, 500, 10)
		{
		}
	};

	class CommandLevitateAcceleration : public CommandSliderFloat
	{
	public:
		explicit CommandLevitateAcceleration(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Acceleration"), CMDNAMES("levitateaccel"), LIT("Acceleration increases the speed of levitation the longer you move. Resets once you sprint or stop moving."), 0, 1000, 0, 10)
		{
		}
	};

	class CommandLevitateIgnorePitch : public CommandToggle
	{
	public:
		explicit CommandLevitateIgnorePitch(CommandList* const parent)
			: CommandToggle(parent, LIT("Movement Ignores Pitch"), CMDNAMES("levitateignorepitch"), LIT("Disables forward and backward movement affecting height depending on where you're looking."), true)
		{
		}
	};

	class CommandLevitateKeepMomentum : public CommandToggle
	{
	public:
		explicit CommandLevitateKeepMomentum(CommandList* const parent)
			: CommandToggle(parent, LIT("Keep Momentum"), CMDNAMES("keepmomentum"), NOLABEL, false)
		{
		}
	};

	class CommandLevitateApplyPitch : public CommandToggle
	{
	public:
		explicit CommandLevitateApplyPitch(CommandList* const parent)
			: CommandToggle(parent, LIT("Apply Pitch to Entity"), CMDNAMES("levitateapplypitch"), NOLABEL, false)
		{
		}
	};

	class CommandLevitateOnlyRotateOnMovement : public CommandToggle
	{
	public:
		explicit CommandLevitateOnlyRotateOnMovement(CommandList* const parent)
			: CommandToggle(parent, LIT("Only Rotate On Movement"), CMDNAMES("levitaterotate"), NOLABEL, true)
		{
		}
	};

	class CommandLevitateButtonInstructions : public CommandToggle
	{
	public:
		explicit CommandLevitateButtonInstructions(CommandList* const parent)
			: CommandToggle(parent, LIT("Show Button Instructions"), CMDNAMES("levitatebuttoninstructions"), NOLABEL, true)
		{
		}
	};

	class CommandLevitatePassiveMin : public CommandSliderFloat
	{
	public:
		explicit CommandLevitatePassiveMin(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Min Distance From Ground"), CMDNAMES("levitatepassivemin"), NOLABEL, -1000000, 1000000, 0, 10)
		{
		}
	};

	class CommandLevitatePassiveMax : public CommandSliderFloat
	{
	public:
		explicit CommandLevitatePassiveMax(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Max Distance From Ground"), CMDNAMES("levitatepassivemax"), NOLABEL, -1000000, 1000000, 60, 10)
		{
		}
	};

	class CommandLevitatePassiveSpeed : public CommandSlider
	{
	public:
		explicit CommandLevitatePassiveSpeed(CommandList* const parent)
			: CommandSlider(parent, LIT("Speed"), CMDNAMES("levitatepassivespeed"), NOLABEL, 0, 1000000, 5, 1)
		{
		}
	};

	class CommandLevitateAssistUp : public CommandSliderFloat
	{
	public:
		explicit CommandLevitateAssistUp(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Upward Force"), CMDNAMES("levitateassistup"), NOLABEL, 0, 1000000, 60, 10)
		{
		}
	};

	class CommandLevitateAssistDown : public CommandSliderFloat
	{
	public:
		explicit CommandLevitateAssistDown(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Downward Force"), CMDNAMES("levitateassistdown"), NOLABEL, 0, 1000000, 60, 10)
		{
		}
	};

	class CommandLevitateAssistDeadzone : public CommandSlider
	{
	public:
		explicit CommandLevitateAssistDeadzone(CommandList* const parent)
			: CommandSlider(parent, LIT("Downward Deadzone"), CMDNAMES("levitateassistdeadzone"), LIT("How far from the ground you have to be before the assistant stops trying to pull you down."), 0, 100000, 13, 1)
		{
		}
	};

	class CommandLevitateAssistSnap : public CommandSliderFloat
	{
	public:
		explicit CommandLevitateAssistSnap(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Snapping"), CMDNAMES("levitateassistsnap"), NOLABEL, 0, 1000000, 10, 10)
		{
		}
	};

	class CommandTabLevitation : public CommandList
	{
	public:
		CommandLevitateSpeed* const speed;
		CommandLevitateSprintMultiplier* const sprintMultiplier;
		CommandLevitateAcceleration* const accel;
		CommandLevitateIgnorePitch* const ignorePitch;
		CommandLevitateKeepMomentum* const keepMomentum;
		CommandLevitateApplyPitch* const applyPitch;
		CommandLevitateOnlyRotateOnMovement* const onlyRotateOnMovement;
		CommandLevitateButtonInstructions* const buttonInstructions;
		CommandLevitatePassiveMin* const passiveMin;
		CommandLevitatePassiveMax* const passiveMax;
		CommandLevitatePassiveSpeed* const passiveSpeed;
		CommandLevitateAssistUp* const assistUp;
		CommandLevitateAssistDown* const assistDown;
		CommandLevitateAssistDeadzone* const assistDeadzone;
		CommandLevitateAssistSnap* const assistSnap;
		CommandLevitationPresets* const presets;
		CommandLevitate* const levitate;

		explicit CommandTabLevitation() :
		    CommandList(nullptr, LIT("Levitation")),
		    speed(createChild<CommandLevitateSpeed>()),
		    sprintMultiplier(createChild<CommandLevitateSprintMultiplier>()),
		    accel(createChild<CommandLevitateAcceleration>()),
		    ignorePitch(createChild<CommandLevitateIgnorePitch>()),
		    keepMomentum(createChild<CommandLevitateKeepMomentum>()),
		    applyPitch(createChild<CommandLevitateApplyPitch>()),
		    onlyRotateOnMovement(createChild<CommandLevitateOnlyRotateOnMovement>()),
		    buttonInstructions(createChild<CommandLevitateButtonInstructions>()),
		    passiveMin(createChild<CommandLevitatePassiveMin>()),
		    passiveMax(createChild<CommandLevitatePassiveMax>()),
		    passiveSpeed(createChild<CommandLevitatePassiveSpeed>()),
		    assistUp(createChild<CommandLevitateAssistUp>()),
		    assistDown(createChild<CommandLevitateAssistDown>()),
		    assistDeadzone(createChild<CommandLevitateAssistDeadzone>()),
		    assistSnap(createChild<CommandLevitateAssistSnap>()),
		    presets(createChild<CommandLevitationPresets>(this)),
		    levitate(createChild<CommandLevitate>(speed, sprintMultiplier, accel, ignorePitch, keepMomentum, applyPitch, onlyRotateOnMovement, buttonInstructions, passiveMin, passiveMax, passiveSpeed, assistUp, assistDown, assistDeadzone, assistSnap))
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabLevitation& GetCommandTabLevitation();
}
