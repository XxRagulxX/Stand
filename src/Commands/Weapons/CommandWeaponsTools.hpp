#pragma once
#include "Commands/Weapons/CommandOpenGunLocker.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Core/Pointers.hpp"
#include "Menu/Click.hpp"
#include "Network/GlobalPlayerBD.hpp"
#include "Rendering/Notifications.hpp"
#include "Util/Label.hpp"
#include "Weapons/WeaponsHashes.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandGiveAllWeapons : public CommandPhysical
	{
	public:
		explicit CommandGiveAllWeapons(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Give All Weapons"), CMDNAMES("giveallweapons2"), LIT("Gives you all available weapons."))
		{
		}

		void onClick(Click& click) override
		{
			if (auto ped = Stand::Self::GetPed())
			{
				for (auto weapon : g_WeaponHashes)
					ped.GiveWeapon(weapon);
			}
		}
	};

	class CommandGiveMaxAmmo : public CommandPhysical
	{
	public:
		explicit CommandGiveMaxAmmo(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Give Max Ammo"), CMDNAMES("givemaxammo2"), LIT("Refills your ammo to maximum for all weapons."))
		{
		}

		void onClick(Click& click) override
		{
			if (auto ped = Stand::Self::GetPed())
			{
				for (auto weapon : g_WeaponHashes)
					ped.SetMaxAmmoForWeapon(weapon);
			}
		}
	};

	class CommandOpenGunLockerTree : public CommandPhysical
	{
	public:
		explicit CommandOpenGunLockerTree(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Open Gun Locker"), CMDNAMES("opengunlocker2"), LIT("Allows you to access gun locker remotely."))
		{
		}

		void onClick(Click& click) override
		{
			if (!*Pointers.IsSessionStarted)
			{
				Notifications::Show("Open Gun Locker", "Please join GTA Online.", NotificationType::Error);
				return;
			}

			if (GlobalPlayerBD::Get()->Entries[Stand::Self::GetPlayer().GetId()].SimpleInteriorData.Index != eSimpleInteriorIndex::SIMPLE_INTERIOR_INVALID)
			{
				Notifications::Show("Open Gun Locker", "Cannot open gun locker while in an interior.", NotificationType::Error);
				return;
			}

			Features::OpenGunLocker::SetShouldRunScript(true);
		}
	};

	class CommandWeaponsTools : public CommandList
	{
	public:
		explicit CommandWeaponsTools(CommandList* const parent)
			: CommandList(parent, LIT("Tools"), CMDNAMES("weapontools"))
		{
			createChild<CommandGiveAllWeapons>();
			createChild<CommandGiveMaxAmmo>();
			createChild<CommandOpenGunLockerTree>();
		}
	};
}
