#pragma once
#include "Commands/Stand/CommandToggleNoCorrelation.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Menu/Click.hpp"
#include "Rendering/GridStandCommandList.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "Weapons/weapon_components.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandWeaponComponents;

	class CommandNoWeapon : public CommandPhysical
	{
	public:
		explicit CommandNoWeapon(CommandList* parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("No weapon in hand"), {})
		{
		}
		void onClick(Click&) override {}
	};

	class CommandWeaponComponent : public CommandToggleNoCorrelation
	{
	public:
		const joaat_t component;

		explicit CommandWeaponComponent(CommandList* parent, const char* display_name, joaat_t comp)
			: CommandToggleNoCorrelation(parent, LIT(display_name), {}), component(comp)
		{
		}

		[[nodiscard]] joaat_t getCurrentWeapon() const noexcept;

		void onEnable(Click& click) override
		{
			const joaat_t weapon = getCurrentWeapon();
			if (!weapon)
				return;
			const int ped = Self::GetPed().GetHandle();
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(ped, weapon, component);
		}

		void onDisable(Click& click) override
		{
			const joaat_t weapon = getCurrentWeapon();
			if (!weapon)
				return;
			const int ped = Self::GetPed().GetHandle();
			WEAPON::REMOVE_WEAPON_COMPONENT_FROM_PED(ped, weapon, component);
		}
	};

	class CommandWeaponComponents : public CommandList
	{
	public:
		joaat_t m_currentWeapon = 0;

		explicit CommandWeaponComponents(CommandList* parent)
			: CommandList(parent, LIT("Components"), CMDNAMES("weaponcomponents"))
		{
			createChild<CommandNoWeapon>();
			CommandTickDispatch::AddCommand(this);
		}

		~CommandWeaponComponents() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			auto ped = Self::GetPed();
			joaat_t weapon = 0;
			int pedHandle = 0;

			if (ped)
			{
				pedHandle = ped.GetHandle();
				WEAPON::GET_CURRENT_PED_WEAPON(pedHandle, reinterpret_cast<Hash*>(&weapon), FALSE);
				static constexpr joaat_t WEAPON_UNARMED = "weapon_unarmed"_J;
				if (weapon == WEAPON_UNARMED)
					weapon = 0;
			}

			if (weapon == m_currentWeapon)
			{
				if (weapon != 0)
				{
					for (auto& child : children)
					{
						auto* cmd = static_cast<CommandWeaponComponent*>(child.get());
						const bool hasIt = WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(pedHandle, weapon, cmd->component) == TRUE;
						if (cmd->m_on != hasIt)
							cmd->m_on = hasIt;
					}
				}
				return;
			}

			m_currentWeapon = weapon;
			children.clear();

			if (weapon != 0)
			{
				for (const auto& wc : g_weapon_components)
				{
					if (WEAPON::DOES_WEAPON_TAKE_WEAPON_COMPONENT(weapon, wc.hash) == TRUE)
					{
						const bool hasIt = WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(pedHandle, weapon, wc.hash) == TRUE;
						auto* cmd = createChild<CommandWeaponComponent>(wc.display_name, wc.hash);
						cmd->m_on = hasIt;
					}
				}
			}
			else
			{
				createChild<CommandNoWeapon>();
			}

			Rendering::GridStandCommandList::GetOrCreate(this).invalidateContent();
		}
	};

	inline joaat_t CommandWeaponComponent::getCurrentWeapon() const noexcept
	{
		return static_cast<CommandWeaponComponents*>(parent)->m_currentWeapon;
	}
}
