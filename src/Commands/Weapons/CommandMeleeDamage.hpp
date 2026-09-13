#pragma once
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

#include <cmath>

namespace Stand
{
	class CommandMeleeDamage : public CommandToggle
	{
	public:
		CommandSliderFloat* m_scale = nullptr;

		explicit CommandMeleeDamage(CommandList* const parent)
			: CommandToggle(parent, LIT("Melee Damage"), CMDNAMES("meleedamage2"), LIT("Increases your melee damage output."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(Stand::Self::GetPlayer().GetId(), 1.0f, 1);
		}

		void onTick() override
		{
			if (!m_scale)
				return;

			const float scale = m_scale->getFloatValue();
			if (scale <= 0.0f)
				return;

			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;

			const auto ped_handle = ped.GetHandle();

			PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(Stand::Self::GetPlayer().GetId(), scale, 1);

			const Hash target = PED::GET_MELEE_TARGET_FOR_PED(ped_handle);
			if (target == 0 || !ENTITY::DOES_ENTITY_EXIST(target))
				return;

			if (!PED::IS_PED_PERFORMING_MELEE_ACTION(ped_handle))
				return;

			const Hash weapon = WEAPON::GET_SELECTED_PED_WEAPON(ped_handle);
			if (!WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(target, weapon, 0))
				return;

			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(target);

			const Vector3 from = ENTITY::GET_ENTITY_COORDS(ped_handle, true);
			const Vector3 to   = ENTITY::GET_ENTITY_COORDS(target, true);

			float dx  = to.x - from.x;
			float dy  = to.y - from.y;
			float len = std::sqrt(dx * dx + dy * dy);
			if (len > 0.0f) { dx /= len; dy /= len; }
			dx *= scale;
			dy *= scale;

			ENTITY::APPLY_FORCE_TO_ENTITY(
				target, 0, 0.0f, 0.0f, 15.0f, 0, 0, 0, 0,
				true, false, true, false, true);

			ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(
				target, 0, dx, dy, -1.0f, true, false, true, true);
		}

		~CommandMeleeDamage() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
