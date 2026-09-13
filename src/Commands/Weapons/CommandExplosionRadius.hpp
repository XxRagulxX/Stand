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
	class CommandExplosionRadius : public CommandToggle
	{
	public:
		CommandSliderFloat* m_scale = nullptr;

		explicit CommandExplosionRadius(CommandList* const parent)
			: CommandToggle(parent, LIT("Weapon Explosion Radius"), CMDNAMES("explosionradius2"), LIT("Increases the explosion radius of your weapons."))
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
			if (!m_scale)
				return;

			if (auto ped = Stand::Self::GetPed())
			{
				Hash weapon;
				if (WEAPON::GET_CURRENT_PED_WEAPON(ped.GetHandle(), &weapon, 0))
					WEAPON::SET_WEAPON_AOE_MODIFIER(weapon, m_scale->getFloatValue());
			}
		}

		~CommandExplosionRadius() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
