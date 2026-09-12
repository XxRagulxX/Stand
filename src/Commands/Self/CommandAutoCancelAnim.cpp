#include "Commands/Self/CommandAutoCancelAnim.hpp"

#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Game/Animations.hpp"
#include "Game/ControllerInputs.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	CommandAutoCancelAnim::CommandAutoCancelAnim(CommandList* parent)
		: CommandToggle(parent, LIT("Automatically Cancel"), CMDNAMES("autocancelanim"))
	{
	}

	void CommandAutoCancelAnim::onEnable(Click& click)
	{
		CommandTickDispatch::AddCommand(this);
	}

	void CommandAutoCancelAnim::onDisable(Click& click)
	{
		CommandTickDispatch::RemoveCommand(this);
	}

	void CommandAutoCancelAnim::onTick()
	{
		const bool moving =
			PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_UP_ONLY)
			|| PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_DOWN_ONLY)
			|| PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_LEFT_ONLY)
			|| PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_RIGHT_ONLY);

		if (!moving)
			return;

		const auto ped = Self::GetPed().GetHandle();

		if (Animations::last_complex_animation != nullptr
			&& !ENTITY::IS_ENTITY_PLAYING_ANIM(
				ped,
				Animations::last_complex_animation->anim_dict,
				Animations::last_complex_animation->anim_name,
				3))
		{
			Animations::last_complex_animation = nullptr;
			Animations::reset();
		}
		else if (!Animations::last_animation_was_partial)
		{
			Animations::reset();
		}
		else if (TASK::PED_HAS_USE_SCENARIO_TASK(ped))
		{
			TASK::CLEAR_PED_TASKS_IMMEDIATELY(ped);
		}
	}

	void CommandAutoCancelAnim::applyDefaultState()
	{
		if (m_on)
		{
			m_on = false;
			CommandTickDispatch::RemoveCommand(this);
		}
	}
}
