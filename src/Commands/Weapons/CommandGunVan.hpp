#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Menu/Click.hpp"
#include "Network/Tunables.hpp"
#include "Rendering/Notifications.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Util/Joaat.hpp"
#include "Util/Label.hpp"
#include "Weapons/Weapon.hpp"
#include "World/Stats.hpp"

#include <string>

namespace Stand
{
	class CommandGunVanWeapon : public CommandSlider
	{
	public:
		explicit CommandGunVanWeapon(CommandList* parent)
			: CommandSlider(parent, LIT("Weapon"), CMDNAMES("gunvanweapon"), NOLABEL,
				  1, static_cast<int>(Weapon::weapons.size()) - 1, 1)
		{
		}

		[[nodiscard]] std::string getValueText() const override
		{
			return Weapon::weapons[value].name;
		}
	};

	class CommandGunVanSlot : public CommandSlider
	{
	public:
		explicit CommandGunVanSlot(CommandList* parent)
			: CommandSlider(parent, LIT("Slot"), CMDNAMES("gunvanslot"), NOLABEL, 0, 9, 0)
		{
		}

		[[nodiscard]] std::string getValueText() const override
		{
			return "Slot " + std::to_string(value + 1);
		}
	};

	class CommandGunVanApply : public CommandPhysical
	{
		CommandGunVanWeapon* m_weapon;
		CommandGunVanSlot*   m_slot;

	public:
		explicit CommandGunVanApply(CommandList* parent, CommandGunVanWeapon* weapon, CommandGunVanSlot* slot)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Apply"), CMDNAMES("gunvanapply"),
				  LIT("Apply selected weapon to selected Gun Van slot.")),
			  m_weapon(weapon), m_slot(slot)
		{
		}

		void onClick(Click& click) override
		{
			const int idx  = m_weapon->value;
			const int slot = m_slot->value;

			FiberPool::queueJob([idx, slot] {
				if (*ScriptGlobal(2655288).As<int*>() == -1)
				{
					Notifications::Show("Gun Van", "Join a freemode session and try again.", NotificationType::Error);
					return;
				}

				static Tunable gunVanTunable{15999531};
				if (gunVanTunable.IsReady())
					gunVanTunable.Set(2139095040);

				HUD::SET_BLIP_AS_SHORT_RANGE(HUD::GET_FIRST_BLIP_INFO_ID(844), false);

				Stats::SetPackedBool(24, true);
				Script::current()->yield(300);
				Stats::SetPackedBool(24, false);
				Script::current()->yield(300);
				Stats::SetPackedBool(24, true);
				Script::current()->yield(300);
				Stats::SetPackedBool(24, false);

				const Weapon& w        = Weapon::weapons[idx];
				const bool throwable   = (w.category == Weapon::THROWABLE);

				if (throwable && slot > 2)
				{
					Notifications::Show("Gun Van", "Invalid slot. Throwables can only be placed in slots 1-3.", NotificationType::Error);
					return;
				}

				const std::string tunableName = throwable
					? "XM22_GUN_VAN_SLOT_THROWABLE_TYPE_" + std::to_string(slot)
					: "XM22_GUN_VAN_SLOT_WEAPON_TYPE_" + std::to_string(slot);

				Tunable slotTunable{Joaat(tunableName)};
				if (slotTunable.IsReady())
					slotTunable.Set(w.hash);

				Notifications::Show("Gun Van", "Selected items are now available at the Gun Van.", NotificationType::Success);
			});
		}
	};

	class CommandGunVan : public CommandList
	{
	public:
		explicit CommandGunVan(CommandList* parent)
			: CommandList(parent, LIT("Gun Van"), CMDNAMES("gunvan"))
		{
			auto* weapon = createChild<CommandGunVanWeapon>();
			auto* slot   = createChild<CommandGunVanSlot>();
			createChild<CommandGunVanApply>(weapon, slot);
		}
	};
}
