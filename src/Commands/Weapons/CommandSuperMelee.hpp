#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

#include <cmath>

namespace Stand
{
	class CommandSuperMelee : public CommandSlider
	{
	public:
		explicit CommandSuperMelee(CommandList* const parent)
			: CommandSlider(parent, LIT("Super Melee"), CMDNAMES("supermelee"),
			                LIT("Slings NPCs and vehicles away after you melee-attack them."),
			                0, 10, 0, 1)
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onTick() override
		{
			if (value == 0)
				return;

			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;

			const auto ped_handle = ped.GetHandle();

			if (!PED::IS_PED_PERFORMING_MELEE_ACTION(ped_handle))
				return;

			const Hash target = PED::GET_MELEE_TARGET_FOR_PED(ped_handle);
			if (target == 0 || !ENTITY::DOES_ENTITY_EXIST(target))
				return;

			if (ENTITY::IS_ENTITY_A_PED(target) && PED::IS_PED_A_PLAYER(target))
				return;

			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(target);

			const Vector3 from = ENTITY::GET_ENTITY_COORDS(ped_handle, true);
			const Vector3 to   = ENTITY::GET_ENTITY_COORDS(target, true);

			float dx  = to.x - from.x;
			float dy  = to.y - from.y;
			float len = std::sqrt(dx * dx + dy * dy);
			if (len > 0.0f) { dx /= len; dy /= len; }

			const float impulse = static_cast<float>(value) * 25.0f;
			dx *= impulse;
			dy *= impulse;

			ENTITY::APPLY_FORCE_TO_ENTITY(
				target, 0, 0.0f, 0.0f, 15.0f, 0, 0, 0, 0,
				true, false, true, false, true);

			ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(
				target, 0, dx, dy, -1.0f, true, false, true, true);

			WEAPON::CLEAR_ENTITY_LAST_WEAPON_DAMAGE(target);
			ENTITY::CLEAR_ENTITY_LAST_DAMAGE_ENTITY(target);
		}

		~CommandSuperMelee() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
