#pragma once
#include "Commands/Weapons/CommandExplosionType.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Scripts.hpp"
#include "Util/Joaat.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandExplosiveAmmo : public CommandToggle
	{
	public:
		CommandExplosionType* m_type   = nullptr;
		CommandSliderFloat*   m_damage = nullptr;
		CommandSliderFloat*   m_shake  = nullptr;

		explicit CommandExplosiveAmmo(CommandList* const parent)
			: CommandToggle(parent, LIT("Explosive Ammo"), CMDNAMES("explosiveammo"), LIT("Makes your bullets create explosions."))
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
			if (!m_type || !m_damage || !m_shake)
				return;

			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;

			const auto ped_handle = ped.GetHandle();

			if (!WEAPON::IS_PED_ARMED(ped_handle, 4)
				|| PED::IS_PED_PERFORMING_MELEE_ACTION(ped_handle))
				return;

			Vector3 impact_coords{};
			if (!WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(ped_handle, &impact_coords))
				return;

			const int   explosion_type = m_type->getTypeInt();
			const float damage_scale   = m_damage->getFloatValue();
			const float camera_shake   = m_shake->getFloatValue();

			Scripts::RunWithSpoofedThreadName("am_mp_orbital_cannon"_J, [=] {
				FIRE::ADD_OWNED_EXPLOSION(
					ped_handle,
					impact_coords.x, impact_coords.y, impact_coords.z,
					explosion_type,
					damage_scale,
					true,
					false,
					camera_shake);
			});
		}

		~CommandExplosiveAmmo() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
