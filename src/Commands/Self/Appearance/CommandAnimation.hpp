#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Game/Animations.hpp"
#include "Menu/Click.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

#include <type_traits>

namespace Stand
{
	template <class AnimationType>
	class CommandAnimation final : public CommandPhysical
	{
		AnimationType m_anim;

		[[nodiscard]] bool shouldUsePartialMovement() const
		{
			if (m_anim.movement_override == ForcedMovementType::FORCE_MOVEMENT)
				return true;
			if (m_anim.movement_override == ForcedMovementType::FORCE_STILL)
				return false;
			return Animations::partial_movement;
		}

	public:
		explicit CommandAnimation(CommandList* parent, AnimationType anim)
			: CommandPhysical(COMMAND_ACTION, parent,
				LIT(anim.display_name), {}, NOLABEL, CMDFLAGS_ACTION),
			  m_anim(anim)
		{
		}

		void onClick(Click& click) override
		{
			FiberPool::queueJob([this]
			{
				Animations::reset();
				const auto ped = Self::GetPed().GetHandle();

				if constexpr (std::is_same_v<SimpleAnimation, AnimationType>)
				{
					TASK::CLEAR_PED_TASKS_IMMEDIATELY(ped);
					TASK::TASK_START_SCENARIO_IN_PLACE(ped, m_anim.raw_name, 0, true);
				}
				else
				{
					const bool partial    = shouldUsePartialMovement();
					const char* dict      = m_anim.anim_dict;
					const char* clip      = m_anim.anim_name;
					int flag              = 1;
					if (partial)
						flag |= 120;

					if constexpr (std::is_same_v<ComplexObjectAnimation, AnimationType>)
					{
						const auto& od = m_anim.object_data;
						STREAMING::REQUEST_MODEL(od.model);
						while (!STREAMING::HAS_MODEL_LOADED(od.model))
							Script::current()->yield();
						const auto pos = Self::GetPed().GetPosition();
						const Object obj = OBJECT::CREATE_OBJECT_NO_OFFSET(
							od.model, pos.x, pos.y, pos.z, false, false, true, 0);
						if (obj != 0)
						{
							const int boneIdx = PED::GET_PED_BONE_INDEX(ped, od.bone);
							ENTITY::ATTACH_ENTITY_TO_ENTITY(
								obj, ped, boneIdx,
								od.ox, od.oy, od.oz,
								od.rx, od.ry, od.rz,
								true, true, false, true, 1, true, 0);
							STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(od.model);
							Animations::last_object_attachment = obj;
						}
					}

					Animations::loadDict(dict);
					TASK::CLEAR_PED_TASKS_IMMEDIATELY(ped);
					TASK::TASK_PLAY_ANIM(
						ped, dict, clip,
						2.0f, 2.0f,
						Animations::looped
							? -1
							: static_cast<int>(ENTITY::GET_ANIM_DURATION(dict, clip) * 1000.0f),
						flag,
						1.0f, false, false, false);
					Animations::unloadDict(dict);

					Animations::last_complex_animation      = &m_anim;
					Animations::last_animation_was_partial  = partial;
				}
			});
		}
	};
}
