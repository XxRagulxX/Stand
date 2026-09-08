#include "Commands/CommandLegacy.hpp"
#include "Commands/CommandListSelect.hpp"
#include "Scripting/Natives.hpp"
#include "World/Stats.hpp"
#include "Rendering/Notifications.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Network/Tunables.hpp"
#include "Scripting/FiberPool.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Features
{
	static const char* GetGunVanWeaponHash(int id)
	{
		switch (id)
		{
		case 1: return "WEAPON_PISTOL";
		case 2: return "WEAPON_COMBATPISTOL";
		case 3: return "WEAPON_APPISTOL";
		case 4: return "WEAPON_PISTOL50";
		case 5: return "WEAPON_SMG";
		case 6: return "WEAPON_MICROSMG";
		case 7: return "WEAPON_ASSAULTRIFLE";
		case 8: return "WEAPON_CARBINERIFLE";
		case 9: return "WEAPON_ADVANCEDRIFLE";
		case 10: return "WEAPON_MG";
		case 11: return "WEAPON_COMBATMG";
		case 12: return "WEAPON_PUMPSHOTGUN";
		case 13: return "WEAPON_SAWNOFFSHOTGUN";
		case 14: return "WEAPON_ASSAULTSHOTGUN";
		case 15: return "WEAPON_HEAVYSNIPER";
		case 16: return "WEAPON_SNIPERRIFLE";
		case 17: return "WEAPON_GRENADELAUNCHER";
		case 18: return "WEAPON_RPG";
		case 19: return "WEAPON_MINIGUN";
		case 20: return "WEAPON_GRENADE";
		case 21: return "WEAPON_SMOKEGRENADE";
		case 22: return "WEAPON_STICKYBOMB";
		case 23: return "WEAPON_MOLOTOV";
		case 24: return "WEAPON_PETROLCAN";
		case 25: return "WEAPON_KNIFE";
		case 26: return "WEAPON_NIGHTSTICK";
		case 27: return "WEAPON_HAMMER";
		case 28: return "WEAPON_BAT";
		case 29: return "WEAPON_CROWBAR";
		case 30: return "WEAPON_GOLFCLUB";
		case 31: return "WEAPON_ASSAULTSMG";
		case 32: return "WEAPON_BULLPUPSHOTGUN";
		case 33: return "WEAPON_BOTTLE";
		case 34: return "WEAPON_SPECIALCARBINE";
		case 35: return "WEAPON_SNSPISTOL";
		case 36: return "WEAPON_HEAVYPISTOL";
		case 37: return "WEAPON_BULLPUPRIFLE";
		case 38: return "WEAPON_GUSENBERG";
		case 39: return "WEAPON_VINTAGEPISTOL";
		case 40: return "WEAPON_DAGGER";
		case 41: return "WEAPON_MUSKET";
		case 42: return "WEAPON_FIREWORK";
		case 43: return "WEAPON_FLAREGUN";
		case 44: return "WEAPON_HEAVYSHOTGUN";
		case 45: return "WEAPON_MARKSMANRIFLE";
		case 46: return "WEAPON_PROXMINE";
		case 47: return "WEAPON_HOMINGLAUNCHER";
		case 48: return "WEAPON_HATCHET";
		case 49: return "WEAPON_COMBATPDW";
		case 50: return "WEAPON_KNUCKLE";
		case 51: return "WEAPON_MARKSMANPISTOL";
		case 52: return "WEAPON_MACHETE";
		case 53: return "WEAPON_MACHINEPISTOL";
		case 54: return "WEAPON_DBSHOTGUN";
		case 55: return "WEAPON_COMPACTRIFLE";
		case 56: return "WEAPON_FLASHLIGHT";
		case 57: return "WEAPON_REVOLVER";
		case 58: return "WEAPON_SWITCHBLADE";
		case 59: return "WEAPON_AUTOSHOTGUN";
		case 60: return "WEAPON_MINISMG";
		case 61: return "WEAPON_COMPACTLAUNCHER";
		case 62: return "WEAPON_BATTLEAXE";
		case 63: return "WEAPON_PIPEBOMB";
		case 64: return "WEAPON_POOLCUE";
		case 65: return "WEAPON_WRENCH";
		case 66: return "WEAPON_DOUBLEACTION";
		case 67: return "WEAPON_RAYPISTOL";
		case 68: return "WEAPON_RAYCARBINE";
		case 69: return "WEAPON_RAYMINIGUN";
		case 70: return "WEAPON_STONE_HATCHET";
		case 71: return "WEAPON_NAVYREVOLVER";
		case 72: return "WEAPON_CERAMICPISTOL";
		case 73: return "WEAPON_GADGETPISTOL";
		case 74: return "WEAPON_MILITARYRIFLE";
		case 75: return "WEAPON_COMBATSHOTGUN";
		case 76: return "WEAPON_HEAVYRIFLE";
		case 77: return "WEAPON_EMPLAUNCHER";
		case 78: return "WEAPON_STUNGUN_MP";
		case 79: return "WEAPON_TACTICALRIFLE";
		case 80: return "WEAPON_PRECISIONRIFLE";
		case 81: return "WEAPON_PISTOLXM3";
		case 82: return "WEAPON_CANDYCANE";
		case 83: return "WEAPON_RAILGUNXM3";
		case 84: return "WEAPON_TECPISTOL";
		case 85: return "WEAPON_BATTLERIFLE";
		case 86: return "WEAPON_SNOWLAUNCHER";
		case 87: return "WEAPON_STUNROD";
		case 88: return "WEAPON_STRICKLER";
		default: return nullptr;
		}
	}
	static bool IsGunVanThrowable(int weaponId)
	{
		switch (weaponId)
		{
		case 20: // Grenade
		case 21: // Tear Gas
		case 22: // Sticky Bomb
		case 23: // Molotov
		case 24: // Jerry Can
		case 46: // Proximity Mine
		case 63: // Pipe Bomb
			return true;
		default:
			return false;
		}
	}

	static std::vector<std::pair<int, const char*>> GunVanWeapons =
	    {
	        {1, "Pistol"},
	        {2, "Combat Pistol"},
	        {3, "AP Pistol"},
	        {4, "Pistol .50"},
	        {5, "SMG"},
	        {6, "Micro SMG"},
	        {7, "Assault Rifle"},
	        {8, "Carbine Rifle"},
	        {9, "Advanced Rifle"},
	        {10, "MG"},
	        {11, "Combat MG"},
	        {12, "Pump Shotgun"},
	        {13, "Sawed-Off Shotgun"},
	        {14, "Assault Shotgun"},
	        {15, "Heavy Sniper"},
	        {16, "Sniper Rifle"},
	        {17, "Grenade Launcher"},
	        {18, "RPG"},
	        {19, "Minigun"},
	        {20, "Grenade"},
	        {21, "Tear Gas"},
	        {22, "Sticky Bomb"},
	        {23, "Molotov"},
	        {24, "Jerry Can"},
	        {25, "Knife"},
	        {26, "Nightstick"},
	        {27, "Hammer"},
	        {28, "Baseball Bat"},
	        {29, "Crowbar"},
	        {30, "Golf Club"},
	        {31, "Assault SMG"},
	        {32, "Bullpup Shotgun"},
	        {33, "Bottle"},
	        {34, "Special Carbine"},
	        {35, "SNS Pistol"},
	        {36, "Heavy Pistol"},
	        {37, "Bullpup Rifle"},
	        {38, "Gusenberg Sweeper"},
	        {39, "Vintage Pistol"},
	        {40, "Antique Cavalry Dagger"},
	        {41, "Musket"},
	        {42, "Firework Launcher"},
	        {43, "Flare Gun"},
	        {44, "Heavy Shotgun"},
	        {45, "Marksman Rifle"},
	        {46, "Proximity Mine"},
	        {47, "Homing Launcher"},
	        {48, "Hatchet"},
	        {49, "Combat PDW"},
	        {50, "Knuckle Duster"},
	        {51, "Marksman Pistol"},
	        {52, "Machete"},
	        {53, "Machine Pistol"},
	        {54, "Double Barrel Shotgun"},
	        {55, "Compact Rifle"},
	        {56, "Flashlight"},
	        {57, "Heavy Revolver"},
	        {58, "Switchblade"},
	        {59, "Sweeper Shotgun"},
	        {60, "Mini SMG"},
	        {61, "Compact Grenade Launcher"},
	        {62, "Battle Axe"},
	        {63, "Pipe Bomb"},
	        {64, "Pool Cue"},
	        {65, "Pipe Wrench"},
	        {66, "Double-Action Revolver"},
	        {67, "Up-n-Atomizer"},
	        {68, "Unholy Hellbringer"},
	        {69, "Widowmaker"},
	        {70, "Stone Hatchet"},
	        {71, "Navy Revolver"},
	        {72, "Ceramic Pistol"},
	        {73, "Perico Pistol"},
	        {74, "Military Rifle"},
	        {75, "Combat Shotgun"},
	        {76, "Heavy Rifle"},
	        {77, "Compact EMP Launcher"},
	        {78, "Stun Gun"},
	        {79, "Service Carbine"},
	        {80, "Precision Rifle"},
	        {81, "WM 29 Pistol"},
	        {82, "Candy Cane"},
	        {83, "Railgun"},
	        {84, "Tactical SMG"},
	        {85, "Battle Rifle"},
	        {86, "Snowball Launcher"},
	        {87, "The Shocker"},
	        {88, "El Strickler"}};

	static CommandListSelect _GunVanWeapon{"gunvan_weapon", "Gun Van Weapon", "Select Gun Van weapon", GunVanWeapons, 0};

	static std::vector<std::pair<int, const char*>> GunVanSlots =
	    {
	        {0, "Slot 1"},
	        {1, "Slot 2"},
	        {2, "Slot 3"},
	        {3, "Slot 4"},
	        {4, "Slot 5"},
	        {5, "Slot 6"},
	        {6, "Slot 7"},
	        {7, "Slot 8"},
	        {8, "Slot 9"},
	        {9, "Slot 10"}};

	static CommandListSelect _GunVanSlot{"gunvan_slot", "Gun Van Slot", "Select Gun Van slot", GunVanSlots, 0};

	class GunVanApply : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			FiberPool::queueJob([] {
				if (*ScriptGlobal(2655288).As<int*>() == -1)
				{
					Notifications::Show("Gun Van Halen", "Join a freemode session and try again.", NotificationType::Error);
					return;
				}

				static Tunable Gunvan{15999531};
				if (Gunvan.IsReady())
					Gunvan.Set(2139095040);

				HUD::SET_BLIP_AS_SHORT_RANGE(HUD::GET_FIRST_BLIP_INFO_ID(844), false);

				Stats::SetPackedBool(24, true);
				Script::current()->yield(300);
				Stats::SetPackedBool(24, false);
				Script::current()->yield(300);
				Stats::SetPackedBool(24, true);
				Script::current()->yield(300);
				Stats::SetPackedBool(24, false);

				int weaponId = _GunVanWeapon.GetState();
				int slot = _GunVanSlot.GetState();

				const char* hash = GetGunVanWeaponHash(weaponId);
				if (!hash)
					return;

				bool isThrowable = IsGunVanThrowable(weaponId);

				if (isThrowable && slot > 2)
				{
					Notifications::Show("Gun Van Halen", "Invalid slot. Throwables can only be placed in slots 1-3.", NotificationType::Error);
					return;
				}

				std::string tunableName = isThrowable ? "XM22_GUN_VAN_SLOT_THROWABLE_TYPE_" + std::to_string(slot) : "XM22_GUN_VAN_SLOT_WEAPON_TYPE_" + std::to_string(slot);

				Tunable slotTunable{Joaat(tunableName)};

				if (slotTunable.IsReady())
				{
					slotTunable.Set(Joaat(hash));
				}

				Notifications::Show("Gun Van Halen", "Selected items are now available at the Gun Van.", NotificationType::Success);
			});
		}
	};


	static GunVanApply _GunVanApply{"gunvan_apply", "Gun Van: Apply Weapon", "Apply selected weapon to selected Gun Van slot"};
}
