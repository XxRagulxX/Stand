#pragma once
#include "Commands/Weapons/CommandDisableCriticalHits.hpp"
#include "Commands/Weapons/CommandExplosionRadius.hpp"
#include "Commands/Weapons/CommandExplosionType.hpp"
#include "Commands/Weapons/CommandExplosiveAmmo.hpp"
#include "Commands/Weapons/CommandInfiniteAmmo.hpp"
#include "Commands/Weapons/CommandInfiniteClip.hpp"
#include "Commands/Weapons/CommandInfiniteParachutes.hpp"
#include "Commands/Weapons/CommandMeleeDamage.hpp"
#include "Commands/Weapons/CommandRapidFire.hpp"
#include "Commands/Weapons/CommandWeaponDamage.hpp"
#include "Commands/Weapons/CommandWeaponsTools.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"
#include "Util/Joaat.hpp"
#include "Weapons/WeaponsHashes.hpp"
#include "World/Self.hpp"

#include <span>

namespace Stand
{
	class CommandWeaponSet : public CommandPhysical
	{
		std::span<const Hash> m_hashes;
		bool                  m_give;
	public:
		explicit CommandWeaponSet(CommandList* const parent, Label&& name,
		                          std::vector<CommandName>&& cmds,
		                          std::span<const Hash> hashes, bool give)
			: CommandPhysical(COMMAND_ACTION, parent, std::move(name),
			                  std::move(cmds)),
			  m_hashes(hashes), m_give(give)
		{
		}

		void onClick(Click& click) override
		{
			if (auto ped = Stand::Self::GetPed())
			{
				for (auto hash : m_hashes)
				{
					if (m_give) ped.GiveWeapon(hash);
					else        ped.RemoveWeapon(hash);
				}
			}
		}
	};

	class CommandTabWeapons : public CommandList
	{
		static constexpr Hash kMelee[] = {
			"WEAPON_UNARMED"_J, "WEAPON_KNIFE"_J, "WEAPON_NIGHTSTICK"_J,
			"WEAPON_HAMMER"_J, "WEAPON_BAT"_J, "WEAPON_GOLFCLUB"_J,
			"WEAPON_CROWBAR"_J, "WEAPON_DAGGER"_J, "WEAPON_HATCHET"_J,
			"WEAPON_MACHETE"_J, "WEAPON_FLASHLIGHT"_J, "WEAPON_SWITCHBLADE"_J,
			"WEAPON_WRENCH"_J, "WEAPON_POOLCUE"_J, "WEAPON_BATTLEAXE"_J,
			"WEAPON_STONE_HATCHET"_J, "WEAPON_KNUCKLE"_J, "WEAPON_CANDYCANE"_J,
			"WEAPON_STUNROD"_J, "WEAPON_STRICKLER"_J
		};

		static constexpr Hash kPistols[] = {
			"WEAPON_PISTOL"_J, "WEAPON_COMBATPISTOL"_J, "WEAPON_APPISTOL"_J,
			"WEAPON_PISTOL50"_J, "WEAPON_SNSPISTOL"_J, "WEAPON_HEAVYPISTOL"_J,
			"WEAPON_SNSPISTOL_MK2"_J, "WEAPON_DOUBLEACTION"_J,
			"WEAPON_REVOLVER"_J, "WEAPON_REVOLVER_MK2"_J, "WEAPON_RAYPISTOL"_J,
			"WEAPON_VINTAGEPISTOL"_J, "WEAPON_CERAMICPISTOL"_J,
			"WEAPON_GADGETPISTOL"_J, "WEAPON_NAVYREVOLVER"_J,
			"WEAPON_FLAREGUN"_J, "WEAPON_MARKSMANPISTOL"_J,
			"WEAPON_PISTOL_MK2"_J, "WEAPON_PISTOLXM3"_J,
			"WEAPON_TECPISTOL"_J, "WEAPON_STUNGUN"_J, "WEAPON_STUNGUN_MP"_J
		};

		static constexpr Hash kSMGs[] = {
			"WEAPON_MICROSMG"_J, "WEAPON_SMG"_J, "WEAPON_ASSAULTSMG"_J,
			"WEAPON_GUSENBERG"_J, "WEAPON_COMBATPDW"_J,
			"WEAPON_MACHINEPISTOL"_J, "WEAPON_MINISMG"_J, "WEAPON_SMG_MK2"_J
		};

		static constexpr Hash kAssaultRifles[] = {
			"WEAPON_ASSAULTRIFLE"_J, "WEAPON_CARBINERIFLE"_J,
			"WEAPON_ADVANCEDRIFLE"_J, "WEAPON_BULLPUPRIFLE"_J,
			"WEAPON_SPECIALCARBINE"_J, "WEAPON_SPECIALCARBINE_MK2"_J,
			"WEAPON_BULLPUPRIFLE_MK2"_J, "WEAPON_RAYCARBINE"_J,
			"WEAPON_COMPACTRIFLE"_J, "WEAPON_MILITARYRIFLE"_J,
			"WEAPON_TACTICALRIFLE"_J, "WEAPON_HEAVYRIFLE"_J,
			"WEAPON_CARBINERIFLE_MK2"_J, "WEAPON_ASSAULTRIFLE_MK2"_J,
			"WEAPON_BATTLERIFLE"_J
		};

		static constexpr Hash kShotguns[] = {
			"WEAPON_PUMPSHOTGUN"_J, "WEAPON_SAWNOFFSHOTGUN"_J,
			"WEAPON_ASSAULTSHOTGUN"_J, "WEAPON_BULLPUPSHOTGUN"_J,
			"WEAPON_PUMPSHOTGUN_MK2"_J, "WEAPON_HEAVYSHOTGUN"_J,
			"WEAPON_DBSHOTGUN"_J, "WEAPON_AUTOSHOTGUN"_J,
			"WEAPON_COMBATSHOTGUN"_J
		};

		static constexpr Hash kLMGs[] = {
			"WEAPON_MG"_J, "WEAPON_COMBATMG"_J,
			"WEAPON_COMBATMG_MK2"_J, "WEAPON_RAYMINIGUN"_J
		};

		static constexpr Hash kSniperRifles[] = {
			"WEAPON_SNIPERRIFLE"_J, "WEAPON_HEAVYSNIPER"_J,
			"WEAPON_REMOTESNIPER"_J, "WEAPON_MARKSMANRIFLE"_J,
			"WEAPON_MARKSMANRIFLE_MK2"_J, "WEAPON_HEAVYSNIPER_MK2"_J,
			"WEAPON_MUSKET"_J, "WEAPON_RAILGUN"_J,
			"WEAPON_PRECISIONRIFLE"_J, "WEAPON_RAILGUNXM3"_J
		};

		static constexpr Hash kHeavy[] = {
			"WEAPON_GRENADELAUNCHER"_J, "WEAPON_GRENADELAUNCHER_SMOKE"_J,
			"WEAPON_RPG"_J, "WEAPON_MINIGUN"_J, "WEAPON_HOMINGLAUNCHER"_J,
			"WEAPON_FIREWORK"_J, "WEAPON_COMPACTLAUNCHER"_J,
			"WEAPON_EMPLAUNCHER"_J, "WEAPON_SNOWLAUNCHER"_J,
			"WEAPON_GRENADE"_J, "WEAPON_STICKYBOMB"_J,
			"WEAPON_SMOKEGRENADE"_J, "WEAPON_BZGAS"_J, "WEAPON_MOLOTOV"_J,
			"WEAPON_FIREEXTINGUISHER"_J, "WEAPON_PETROLCAN"_J,
			"WEAPON_BALL"_J, "WEAPON_FLARE"_J, "WEAPON_BOTTLE"_J,
			"WEAPON_ACIDPACKAGE"_J, "WEAPON_PROXMINE"_J,
			"WEAPON_SNOWBALL"_J, "WEAPON_PIPEBOMB"_J,
			"WEAPON_HACKINGDEVICE"_J, "WEAPON_BRIEFCASE_03"_J,
			"WEAPON_HAZARDCAN"_J, "WEAPON_FERTILIZERCAN"_J
		};

	public:
		CommandInfiniteAmmo* const        infiniteAmmo;
		CommandInfiniteClip* const        infiniteClip;
		CommandInfiniteParachutes* const  infiniteParachutes;
		CommandRapidFire* const           rapidFire;
		CommandExplosiveAmmo* const       explosiveAmmo;
		CommandExplosionType* const       explosionType;
		CommandSliderFloat* const         explosionDamage;
		CommandSliderFloat* const         explosionCameraShake;
		CommandWeaponDamage* const        weaponDamage;
		CommandSliderFloat* const         weaponDamageScale;
		CommandMeleeDamage* const         meleeDamage;
		CommandSliderFloat* const         meleeDamageScale;
		CommandExplosionRadius* const     explosionRadius;
		CommandSliderFloat* const         explosionRadiusScale;
		CommandDisableCriticalHits* const disableCriticalHits;
		CommandGiveAllWeapons* const      giveAllWeapons;
		CommandGiveMaxAmmo* const         giveMaxAmmo;
		CommandOpenGunLockerTree* const   openGunLocker;

		explicit CommandTabWeapons()
			: CommandList(nullptr, LIT("Weapons"), CMDNAMES("weapons")),
			  infiniteAmmo(createChild<CommandInfiniteAmmo>()),
			  infiniteClip(createChild<CommandInfiniteClip>()),
			  infiniteParachutes(createChild<CommandInfiniteParachutes>()),
			  rapidFire(createChild<CommandRapidFire>()),
			  explosiveAmmo(createChild<CommandExplosiveAmmo>()),
			  explosionType(createChild<CommandExplosionType>()),
			  explosionDamage(createChild<CommandSliderFloat>(
			      LIT("Explosion Damage Scale"), CMDNAMES("explosiondamage"),
			      NOLABEL, 0, 100000, 100)),
			  explosionCameraShake(createChild<CommandSliderFloat>(
			      LIT("Explosion Camera Shake"), CMDNAMES("explosioncamerashake"),
			      NOLABEL, 0, 1000, 10)),
			  weaponDamage(createChild<CommandWeaponDamage>()),
			  weaponDamageScale(createChild<CommandSliderFloat>(
			      LIT("Weapon Damage Scale"), CMDNAMES("weapondamagescale"),
			      NOLABEL, 0, 1000, 100)),
			  meleeDamage(createChild<CommandMeleeDamage>()),
			  meleeDamageScale(createChild<CommandSliderFloat>(
			      LIT("Melee Damage Scale"), CMDNAMES("meleedamagescale"),
			      NOLABEL, 0, 1000, 100)),
			  explosionRadius(createChild<CommandExplosionRadius>()),
			  explosionRadiusScale(createChild<CommandSliderFloat>(
			      LIT("Explosion Radius Scale"), CMDNAMES("explosionradiusscale"),
			      NOLABEL, 0, 1000, 100)),
			  disableCriticalHits(createChild<CommandDisableCriticalHits>()),
			  giveAllWeapons(createChild<CommandGiveAllWeapons>()),
			  giveMaxAmmo(createChild<CommandGiveMaxAmmo>()),
			  openGunLocker(createChild<CommandOpenGunLockerTree>())
		{
			explosiveAmmo->m_type   = explosionType;
			explosiveAmmo->m_damage = explosionDamage;
			explosiveAmmo->m_shake  = explosionCameraShake;
			weaponDamage->m_scale   = weaponDamageScale;
			meleeDamage->m_scale    = meleeDamageScale;
			explosionRadius->m_scale = explosionRadiusScale;

			struct CatEntry
			{
				const char*           name;
				const char*           get_cmd;
				const char*           rem_cmd;
				std::span<const Hash> hashes;
			};
			static const CatEntry kCats[] = {
				{"Melee",              "getmelee",     "removemelee",     kMelee},
				{"Pistols",            "getpistols",   "removepistols",   kPistols},
				{"Submachine Guns",    "getsmg",       "removesmg",       kSMGs},
				{"Assault Rifles",     "getrifles",    "removerifles",    kAssaultRifles},
				{"Shotguns",           "getshotguns",  "removeshotguns",  kShotguns},
				{"Light Machine Guns", "getlmg",       "removelmg",       kLMGs},
				{"Sniper Rifles",      "getsnipers",   "removesnipers",   kSniperRifles},
				{"Heavy",              "getheavy",     "removeheavy",     kHeavy},
			};

			auto* get = createChild<CommandList>(LIT("Get Weapons"), CMDNAMES("getgun"));
			get->createChild<CommandWeaponSet>(LIT("All Weapons"), CMDNAMES("allguns"),
				std::span<const Hash>(g_WeaponHashes), true);
			for (const auto& cat : kCats)
				get->createChild<CommandWeaponSet>(LIT(cat.name), CMDNAMES_1(cat.get_cmd),
					cat.hashes, true);

			auto* rem = createChild<CommandList>(LIT("Remove Weapons"), CMDNAMES("removegun"));
			rem->createChild<CommandWeaponSet>(LIT("All Weapons"), CMDNAMES("noguns"),
				std::span<const Hash>(g_WeaponHashes), false);
			for (const auto& cat : kCats)
				rem->createChild<CommandWeaponSet>(LIT(cat.name), CMDNAMES_1(cat.rem_cmd),
					cat.hashes, false);
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabWeapons& GetCommandTabWeapons();
}
