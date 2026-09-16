#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Joaat.hpp"
#include "Util/Label.hpp"
#include "Weapons/Weapon.hpp"

namespace Stand
{
	class CommandBodyguardWeaponEntry : public CommandPhysical
	{
		Hash* m_target;
		Hash m_hash;
	public:
		CommandBodyguardWeaponEntry(CommandList* parent, Label&& label, Hash hash, Hash* target)
			: CommandPhysical(COMMAND_ACTION, parent, std::move(label), {}, NOLABEL)
			, m_target(target), m_hash(hash)
		{}
		void onClick(Click&) override { *m_target = m_hash; }
	};

	class CommandBGWeaponCategory : public CommandList
	{
	public:
		CommandBGWeaponCategory(CommandList* parent, const char* label, Weapon::Category cat, Hash* target)
			: CommandList(parent, LIT(label))
		{
			for (const auto& w : Weapon::weapons)
				if (w.category == cat)
					createChild<CommandBodyguardWeaponEntry>(LIT(w.name), w.hash, target);
		}
	};

	class CommandBodyguardPrimaryWeapon : public CommandList
	{
		Hash m_weapon;
	public:
		explicit CommandBodyguardPrimaryWeapon(CommandList* parent)
			: CommandList(parent, LIT("Primary Weapon"), CMDNAMES("bodyguardprimary"))
			, m_weapon("WEAPON_CARBINERIFLE"_J)
		{
			createChild<CommandBGWeaponCategory>("Melee Weapons", Weapon::MELEE,   &m_weapon);
			createChild<CommandBGWeaponCategory>("Pistols",        Weapon::PISTOL,  &m_weapon);
			createChild<CommandBGWeaponCategory>("Machine Guns",   Weapon::SMG,     &m_weapon);
			createChild<CommandBGWeaponCategory>("Rifles",         Weapon::RIFLE,   &m_weapon);
			createChild<CommandBGWeaponCategory>("Shotguns",       Weapon::SHOTGUN, &m_weapon);
			createChild<CommandBGWeaponCategory>("Sniper Rifles",  Weapon::SNIPER,  &m_weapon);
			createChild<CommandBGWeaponCategory>("Heavy Weapons",  Weapon::HEAVY,   &m_weapon);
		}
		[[nodiscard]] Hash GetWeapon() const { return m_weapon; }
		void SetWeapon(Hash h) { m_weapon = h; }
	};

	class CommandBodyguardSecondaryWeapon : public CommandList
	{
		Hash m_weapon;
	public:
		explicit CommandBodyguardSecondaryWeapon(CommandList* parent)
			: CommandList(parent, LIT("Secondary Weapon"), CMDNAMES("bodyguardsecondary"))
			, m_weapon("WEAPON_COMBATPISTOL"_J)
		{
			createChild<CommandBGWeaponCategory>("Melee Weapons", Weapon::MELEE,   &m_weapon);
			createChild<CommandBGWeaponCategory>("Pistols",        Weapon::PISTOL,  &m_weapon);
			createChild<CommandBGWeaponCategory>("Machine Guns",   Weapon::SMG,     &m_weapon);
			createChild<CommandBGWeaponCategory>("Rifles",         Weapon::RIFLE,   &m_weapon);
			createChild<CommandBGWeaponCategory>("Shotguns",       Weapon::SHOTGUN, &m_weapon);
			createChild<CommandBGWeaponCategory>("Sniper Rifles",  Weapon::SNIPER,  &m_weapon);
			createChild<CommandBGWeaponCategory>("Heavy Weapons",  Weapon::HEAVY,   &m_weapon);
		}
		[[nodiscard]] Hash GetWeapon() const { return m_weapon; }
		void SetWeapon(Hash h) { m_weapon = h; }
	};
}
