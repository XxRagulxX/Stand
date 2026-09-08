#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandAutoHeal : public CommandToggle
	{
	public:
		explicit CommandAutoHeal(CommandList* const parent)
			: CommandToggle(parent, LIT("Auto Heal"), { CMDNAME("demigodmode"), CMDNAME("semigodmode") }, LIT("Keeps your character at maximum health and armour."))
		{
		}

		void onChange(Click& click) final
		{
			onChangeToggleScriptTickEventHandler(click, [this]() -> bool {
				auto ped = Stand::Self::GetPed();
				if (ped)
				{
					const auto max_health = static_cast<float>(ped.GetMaxHealth());
					const float health = static_cast<float>(ped.GetHealth());
					if (health > 0.0f && health < max_health)
						ped.SetHealth(static_cast<int>(max_health));

					const float max_armor = static_cast<float>(Stand::Self::GetPlayer().GetMaxArmour());
					if (static_cast<float>(ped.GetArmour()) != max_armor)
						ped.SetArmour(static_cast<int>(max_armor));
				}
				return m_on;
			});
		}
	};
}

