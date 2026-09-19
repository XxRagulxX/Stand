#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Menu/Click.hpp"
#include "Menu/ClickType.hpp"
#include "Scripting/Natives.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	static constexpr const char* kWeaponTintNames[] = {
		"Default", "Green", "Gold", "Pink", "Army", "LSPD", "Orange", "Platinum"
	};

	class CommandWeaponTint : public CommandSlider
	{
	public:
		joaat_t m_currentWeapon = 0;

		explicit CommandWeaponTint(CommandList* parent)
			: CommandSlider(parent, LIT("Current Weapon: Tint"), CMDNAMES("weapontint"),
				  NOLABEL, 0, 7, 0)
		{
			CommandTickDispatch::AddCommand(this);
		}

		~CommandWeaponTint() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		[[nodiscard]] std::string getValueText() const override
		{
			if (value >= 0 && value < 8)
				return kWeaponTintNames[value];
			return std::to_string(value);
		}

		void onChange(Click& click, int prev_value) override
		{
			auto ped = Self::GetPed();
			if (!ped || !m_currentWeapon)
				return;
			WEAPON::SET_PED_WEAPON_TINT_INDEX(ped.GetHandle(), m_currentWeapon, value);
		}

		void onTick() override
		{
			auto ped = Self::GetPed();
			if (!ped)
			{
				m_currentWeapon = 0;
				return;
			}
			const int pedHandle = ped.GetHandle();

			joaat_t weapon = 0;
			WEAPON::GET_CURRENT_PED_WEAPON(pedHandle, reinterpret_cast<Hash*>(&weapon), FALSE);
			static constexpr joaat_t WEAPON_UNARMED = "weapon_unarmed"_J;
			if (weapon == WEAPON_UNARMED)
				weapon = 0;

			if (weapon != m_currentWeapon)
			{
				m_currentWeapon = weapon;
				if (weapon != 0)
				{
					const int count = WEAPON::GET_WEAPON_TINT_COUNT(weapon);
					setMaxValue(count > 1 ? count - 1 : 0);
				}
			}

			if (weapon != 0)
			{
				const int tint = WEAPON::GET_PED_WEAPON_TINT_INDEX(pedHandle, weapon);
				setValueIndicator(Click(CLICK_AUTO), tint);
			}
		}
	};

	class CommandWeaponTintRainbow : public CommandSlider
	{
	public:
		CommandWeaponTint* m_tint = nullptr;

	private:
		time_t m_lastCycle = 0;

	public:
		explicit CommandWeaponTintRainbow(CommandList* parent)
			: CommandSlider(parent, LIT("Current Weapon: Tint: Rainbow Mode"),
				  CMDNAMES("weapontintrainbow"), NOLABEL, 0, 1000, 0)
		{
			CommandTickDispatch::AddCommand(this);
		}

		~CommandWeaponTintRainbow() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			if (value == 0 || !m_tint || !m_tint->m_currentWeapon)
				return;

			auto ped = Self::GetPed();
			if (!ped)
				return;

			const time_t interval = 5050 - static_cast<time_t>(value) * 5;
			if (GET_MILLIS_SINCE(m_lastCycle) < interval)
				return;
			m_lastCycle = get_current_time_millis();

			const int maxTint = m_tint->max_value;
			const int nextTint = (m_tint->value + 1) % (maxTint + 1);
			WEAPON::SET_PED_WEAPON_TINT_INDEX(ped.GetHandle(), m_tint->m_currentWeapon, nextTint);
		}
	};
}
