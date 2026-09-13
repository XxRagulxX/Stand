#pragma once
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandWeaponDamage : public CommandToggle
	{
	public:
		CommandSliderFloat* m_scale = nullptr;

		explicit CommandWeaponDamage(CommandList* const parent)
			: CommandToggle(parent, LIT("Weapon Damage"), CMDNAMES("weapondamage"), LIT("Increases your weapon damage output."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			if (auto ped = Stand::Self::GetPed())
			{
				Hash weapon;
				if (WEAPON::GET_CURRENT_PED_WEAPON(ped.GetHandle(), &weapon, 0))
				{
					WEAPON::SET_WEAPON_DAMAGE_MODIFIER(weapon, 1.0f);
					WEAPON::_SET_WEAPON_PED_DAMAGE_MODIFIER(ped.GetHandle(), 1.0f);
				}
			}
		}

		void onTick() override
		{
			if (!m_scale)
				return;

			const float scale = m_scale->getFloatValue();
			if (scale <= 0.0f)
				return;

			if (auto ped = Stand::Self::GetPed())
			{
				Hash weapon;
				if (WEAPON::GET_CURRENT_PED_WEAPON(ped.GetHandle(), &weapon, 0))
				{
					WEAPON::SET_WEAPON_DAMAGE_MODIFIER(weapon, scale);
					WEAPON::_SET_WEAPON_PED_DAMAGE_MODIFIER(ped.GetHandle(), scale);
				}
			}
		}

		~CommandWeaponDamage() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
