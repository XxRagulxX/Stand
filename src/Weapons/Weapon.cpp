#include "Weapons/Weapon.hpp"
#include "Util/Joaat.hpp"

namespace Stand
{
	const std::vector<Weapon> Weapon::weapons = {
		// Melee
		{"WEAPON_UNARMED"_J,             "Unarmed",                  "unarmed",           MELEE},
		{"WEAPON_KNIFE"_J,               "Knife",                    "knife",             MELEE},
		{"WEAPON_KNUCKLE"_J,             "Knuckle Dusters",          "knuckle",           MELEE},
		{"WEAPON_NIGHTSTICK"_J,          "Nightstick",               "nightstick",        MELEE},
		{"WEAPON_HAMMER"_J,              "Hammer",                   "hammer",            MELEE},
		{"WEAPON_BAT"_J,                 "Baseball Bat",             "bat",               MELEE},
		{"WEAPON_GOLFCLUB"_J,            "Golf Club",                "golfclub",          MELEE},
		{"WEAPON_CROWBAR"_J,             "Crowbar",                  "crowbar",           MELEE},
		{"WEAPON_BOTTLE"_J,              "Broken Bottle",            "bottle",            MELEE},
		{"WEAPON_DAGGER"_J,              "Antique Cavalry Dagger",   "dagger",            MELEE},
		{"WEAPON_HATCHET"_J,             "Hatchet",                  "hatchet",           MELEE},
		{"WEAPON_MACHETE"_J,             "Machete",                  "machete",           MELEE},
		{"WEAPON_FLASHLIGHT"_J,          "Flashlight",               "flashlight",        MELEE},
		{"WEAPON_SWITCHBLADE"_J,         "Switchblade",              "switchblade",       MELEE},
		{"WEAPON_POOLCUE"_J,             "Pool Cue",                 "poolcue",           MELEE},
		{"WEAPON_WRENCH"_J,              "Pipe Wrench",              "wrench",            MELEE},
		{"WEAPON_BATTLEAXE"_J,           "Battle Axe",               "battleaxe",         MELEE},
		{"WEAPON_STONE_HATCHET"_J,       "Stone Hatchet",            "stonehatchet",      MELEE},
		{"WEAPON_CANDYCANE"_J,           "Candy Cane",               "candycane",         MELEE},
		{"WEAPON_STUNROD"_J,             "Stun Rod",                 "stunrod",           MELEE},
		{"WEAPON_STRICKLER"_J,           "The Strickler",            "strickler",         MELEE},
		// Throwables
		{"WEAPON_GRENADE"_J,             "Grenade",                  "grenade",           THROWABLE},
		{"WEAPON_STICKYBOMB"_J,          "Sticky Bomb",              "stickybomb",        THROWABLE},
		{"WEAPON_PROXMINE"_J,            "Proximity Mine",           "proxmine",          THROWABLE},
		{"WEAPON_BZGAS"_J,               "BZ Gas",                   "bzgas",             THROWABLE},
		{"WEAPON_SMOKEGRENADE"_J,        "Tear Gas",                 "smokegrenade",      THROWABLE},
		{"WEAPON_MOLOTOV"_J,             "Molotov Cocktail",         "molotov",           THROWABLE},
		{"WEAPON_FIREEXTINGUISHER"_J,    "Fire Extinguisher",        "fireextinguisher",  THROWABLE},
		{"WEAPON_PETROLCAN"_J,           "Jerry Can",                "petrolcan",         THROWABLE},
		{"WEAPON_SNOWBALL"_J,            "Snowball",                 "snowball",          THROWABLE},
		{"WEAPON_FLARE"_J,               "Flare",                    "flare",             THROWABLE},
		{"WEAPON_BALL"_J,                "Baseball",                 "ball",              THROWABLE},
		{"WEAPON_PIPEBOMB"_J,            "Pipe Bomb",                "pipebomb",          THROWABLE},
		{"WEAPON_HAZARDCAN"_J,           "Hazardous Jerry Can",      "hazardcan",         THROWABLE},
		{"WEAPON_FERTILIZERCAN"_J,       "Fertilizer Can",           "fertilizercan",     THROWABLE},
		{"WEAPON_ACIDPACKAGE"_J,         "Acid Package",             "acidpackage",       THROWABLE},
		{"WEAPON_HACKINGDEVICE"_J,       "Hacking Device",           "hackingdevice",     THROWABLE},
		{"WEAPON_BRIEFCASE_03"_J,        "Briefcase",                "briefcase",         THROWABLE},
		// Pistols
		{"WEAPON_REVOLVER"_J,            "Heavy Revolver",           "revolver",          PISTOL},
		{"WEAPON_PISTOL"_J,              "Pistol",                   "pistol",            PISTOL},
		{"WEAPON_PISTOL_MK2"_J,          "Pistol Mk II",             "pistolmk2",         PISTOL},
		{"WEAPON_COMBATPISTOL"_J,        "Combat Pistol",            "combatpistol",      PISTOL},
		{"WEAPON_APPISTOL"_J,            "AP Pistol",                "appistol",          PISTOL},
		{"WEAPON_PISTOL50"_J,            "Pistol .50",               "pistol50",          PISTOL},
		{"WEAPON_SNSPISTOL"_J,           "SNS Pistol",               "snspistol",         PISTOL},
		{"WEAPON_HEAVYPISTOL"_J,         "Heavy Pistol",             "heavypistol",       PISTOL},
		{"WEAPON_VINTAGEPISTOL"_J,       "Vintage Pistol",           "vintagepistol",     PISTOL},
		{"WEAPON_STUNGUN"_J,             "Stun Gun (SP)",            "stungun",           PISTOL},
		{"WEAPON_STUNGUN_MP"_J,          "Stun Gun (MP)",            "stungunmp",         PISTOL},
		{"WEAPON_FLAREGUN"_J,            "Flare Gun",                "flaregun",          PISTOL},
		{"WEAPON_MARKSMANPISTOL"_J,      "Marksman Pistol",          "marksmanpistol",    PISTOL},
		{"WEAPON_SNSPISTOL_MK2"_J,       "SNS Pistol Mk II",         "snspistolmk2",      PISTOL},
		{"WEAPON_REVOLVER_MK2"_J,        "Heavy Revolver Mk II",     "revolvermk2",       PISTOL},
		{"WEAPON_DOUBLEACTION"_J,        "Double-Action Revolver",   "doubleaction",      PISTOL},
		{"WEAPON_RAYPISTOL"_J,           "Up-n-Atomizer",            "raypistol",         PISTOL},
		{"WEAPON_CERAMICPISTOL"_J,       "Ceramic Pistol",           "ceramicpistol",     PISTOL},
		{"WEAPON_NAVYREVOLVER"_J,        "Navy Revolver",            "navyrevolver",      PISTOL},
		{"WEAPON_GADGETPISTOL"_J,        "Perico Pistol",            "gadgetpistol",      PISTOL},
		{"WEAPON_PISTOLXM3"_J,           "Pistol XM3",               "pistolxm3",         PISTOL},
		{"WEAPON_TECPISTOL"_J,           "Tec Pistol",               "tecpistol",         PISTOL},
		// SMGs / Machine Guns
		{"WEAPON_MICROSMG"_J,            "Micro SMG",                "microsmg",          SMG},
		{"WEAPON_MACHINEPISTOL"_J,       "Machine Pistol",           "machinepistol",     SMG},
		{"WEAPON_MINISMG"_J,             "Mini SMG",                 "minismg",           SMG},
		{"WEAPON_SMG"_J,                 "SMG",                      "smg",               SMG},
		{"WEAPON_SMG_MK2"_J,             "SMG Mk II",                "smgmk2",            SMG},
		{"WEAPON_ASSAULTSMG"_J,          "Assault SMG",              "assaultsmg",        SMG},
		{"WEAPON_MG"_J,                  "MG",                       "mg",                SMG},
		{"WEAPON_COMBATMG"_J,            "Combat MG",                "combatmg",          SMG},
		{"WEAPON_COMBATMG_MK2"_J,        "Combat MG Mk II",          "combatmgmk2",       SMG},
		{"WEAPON_RAYCARBINE"_J,          "Unholy Hellbringer",       "raycarbine",        SMG},
		{"WEAPON_COMBATPDW"_J,           "Combat PDW",               "combatpdw",         SMG},
		{"WEAPON_GUSENBERG"_J,           "Gusenberg Sweeper",        "gusenberg",         SMG},
		// Rifles
		{"WEAPON_ASSAULTRIFLE"_J,        "Assault Rifle",            "assaultrifle",      RIFLE},
		{"WEAPON_ASSAULTRIFLE_MK2"_J,    "Assault Rifle Mk II",      "assaultriflemk2",   RIFLE},
		{"WEAPON_CARBINERIFLE"_J,        "Carbine Rifle",            "carbinerifle",      RIFLE},
		{"WEAPON_CARBINERIFLE_MK2"_J,    "Carbine Rifle Mk II",      "carbinerifsemk2",   RIFLE},
		{"WEAPON_ADVANCEDRIFLE"_J,       "Advanced Rifle",           "advancedrifle",     RIFLE},
		{"WEAPON_SPECIALCARBINE"_J,      "Special Carbine",          "specialcarbine",    RIFLE},
		{"WEAPON_SPECIALCARBINE_MK2"_J,  "Special Carbine Mk II",    "specialcarbinemk2", RIFLE},
		{"WEAPON_BULLPUPRIFLE"_J,        "Bullpup Rifle",            "bullpuprifle",      RIFLE},
		{"WEAPON_BULLPUPRIFLE_MK2"_J,    "Bullpup Rifle Mk II",      "bullpupriflemk2",   RIFLE},
		{"WEAPON_COMPACTRIFLE"_J,        "Compact Rifle",            "compactrifle",      RIFLE},
		{"WEAPON_MILITARYRIFLE"_J,       "Military Rifle",           "militaryrifle",     RIFLE},
		{"WEAPON_HEAVYRIFLE"_J,          "Heavy Rifle",              "heavyrifle",        RIFLE},
		{"WEAPON_TACTICALRIFLE"_J,       "Service Carbine",          "tacticalrifle",     RIFLE},
		{"WEAPON_BATTLERIFLE"_J,         "Battle Rifle",             "battlerifle",       RIFLE},
		// Shotguns
		{"WEAPON_PUMPSHOTGUN"_J,         "Pump Shotgun",             "pumpshotgun",       SHOTGUN},
		{"WEAPON_PUMPSHOTGUN_MK2"_J,     "Pump Shotgun Mk II",       "pumpshotgunmk2",    SHOTGUN},
		{"WEAPON_AUTOSHOTGUN"_J,         "Sweeper Shotgun",          "autoshotgun",       SHOTGUN},
		{"WEAPON_SAWNOFFSHOTGUN"_J,      "Sawed-Off Shotgun",        "sawnoff",           SHOTGUN},
		{"WEAPON_BULLPUPSHOTGUN"_J,      "Bullpup Shotgun",          "bullpupshotgun",    SHOTGUN},
		{"WEAPON_ASSAULTSHOTGUN"_J,      "Assault Shotgun",          "assaultshotgun",    SHOTGUN},
		{"WEAPON_MUSKET"_J,              "Musket",                   "musket",            SHOTGUN},
		{"WEAPON_HEAVYSHOTGUN"_J,        "Heavy Shotgun",            "heavyshotgun",      SHOTGUN},
		{"WEAPON_DBSHOTGUN"_J,           "Double Barrel Shotgun",    "dbshotgun",         SHOTGUN},
		{"WEAPON_COMBATSHOTGUN"_J,       "Combat Shotgun",           "combatshotgun",     SHOTGUN},
		// Sniper Rifles
		{"WEAPON_SNIPERRIFLE"_J,         "Sniper Rifle",             "sniperrifle",       SNIPER},
		{"WEAPON_HEAVYSNIPER"_J,         "Heavy Sniper",             "heavysniper",       SNIPER},
		{"WEAPON_HEAVYSNIPER_MK2"_J,     "Heavy Sniper Mk II",       "heavysnipermk2",    SNIPER},
		{"WEAPON_MARKSMANRIFLE"_J,       "Marksman Rifle",           "marksmanrifle",     SNIPER},
		{"WEAPON_MARKSMANRIFLE_MK2"_J,   "Marksman Rifle Mk II",     "marksmanriflemk2",  SNIPER},
		{"WEAPON_PRECISIONRIFLE"_J,      "Precision Rifle",          "precisionrifle",    SNIPER},
		{"WEAPON_REMOTESNIPER"_J,        "Remote Sniper",            "remotesniper",      SNIPER},
		// Heavy
		{"WEAPON_GRENADELAUNCHER"_J,     "Grenade Launcher",         "grenadelauncher",   HEAVY},
		{"WEAPON_GRENADELAUNCHER_SMOKE"_J, "Smoke Grenade Launcher", "smokelauncher",     HEAVY},
		{"WEAPON_RPG"_J,                 "RPG",                      "rpg",               HEAVY},
		{"WEAPON_MINIGUN"_J,             "Minigun",                  "minigun",           HEAVY},
		{"WEAPON_FIREWORK"_J,            "Firework Launcher",        "firework",          HEAVY},
		{"WEAPON_RAILGUN"_J,             "Railgun (SP)",             "railgun",           HEAVY},
		{"WEAPON_RAILGUNXM3"_J,          "Railgun (MP)",             "railgunxm3",        HEAVY},
		{"WEAPON_RAYMINIGUN"_J,          "Widowmaker",               "widowmaker",        HEAVY},
		{"WEAPON_HOMINGLAUNCHER"_J,      "Homing Launcher",          "hominglauncher",    HEAVY},
		{"WEAPON_COMPACTLAUNCHER"_J,     "Compact Grenade Launcher", "compactlauncher",   HEAVY},
		{"WEAPON_EMPLAUNCHER"_J,         "Compact EMP Launcher",     "emplauncher",       HEAVY},
		{"WEAPON_SNOWLAUNCHER"_J,        "Snowball Launcher",        "snowlauncher",      HEAVY},
	};

	std::vector<Hash> Weapon::getAllHashes()
	{
		std::vector<Hash> all;
		all.reserve(weapons.size());
		for (const auto& w : weapons)
			all.push_back(w.hash);
		return all;
	}

	std::vector<Hash> Weapon::getCategoryHashes(Category cat)
	{
		std::vector<Hash> result;
		for (const auto& w : weapons)
			if (w.category == cat)
				result.push_back(w.hash);
		return result;
	}

	const char* Weapon::getCategoryName(Category cat) noexcept
	{
		switch (cat)
		{
		case MELEE:     return "Melee Weapons";
		case THROWABLE: return "Throwables";
		case PISTOL:    return "Pistols";
		case SMG:       return "Machine Guns";
		case RIFLE:     return "Rifles";
		case SHOTGUN:   return "Shotguns";
		case SNIPER:    return "Sniper Rifles";
		case HEAVY:     return "Heavy Weapons";
		}
		return "Unknown";
	}

	const char* Weapon::getCategoryGetCmd(Category cat) noexcept
	{
		switch (cat)
		{
		case MELEE:     return "getmelee";
		case THROWABLE: return "getthrowables";
		case PISTOL:    return "getpistols";
		case SMG:       return "getsmg";
		case RIFLE:     return "getrifles";
		case SHOTGUN:   return "getshotguns";
		case SNIPER:    return "getsnipers";
		case HEAVY:     return "getheavy";
		}
		return "getunknown";
	}

	const char* Weapon::getCategoryRemCmd(Category cat) noexcept
	{
		switch (cat)
		{
		case MELEE:     return "removemelee";
		case THROWABLE: return "removethrowables";
		case PISTOL:    return "removepistols";
		case SMG:       return "removesmg";
		case RIFLE:     return "removerifles";
		case SHOTGUN:   return "removeshotguns";
		case SNIPER:    return "removesnipers";
		case HEAVY:     return "removeheavy";
		}
		return "removeunknown";
	}
}
