#pragma once
#include "Commands/Weapons/CommandAimbot.hpp"
#include "Commands/Weapons/CommandNoRecoil.hpp"
#include "Commands/Weapons/CommandRangeMultiplier.hpp"
#include "Commands/Weapons/CommandRocketSpeedMultiplier.hpp"
#include "Commands/Weapons/CommandShotgunBatchSize.hpp"
#include "Commands/Weapons/CommandInstantLockon.hpp"
#include "Commands/Weapons/CommandInstantProxyDetonate.hpp"
#include "Commands/Weapons/CommandGunFreedom.hpp"
#include "Commands/Weapons/CommandTeleportGun.hpp"
#include "Commands/Weapons/CommandLaserSights.hpp"
#include "Commands/Weapons/CommandWhenShooting.hpp"
#include "Commands/Weapons/CommandWhenAiming.hpp"
#include "Commands/Weapons/CommandGravityGun.hpp"
#include "Commands/Weapons/CommandNoSpooling.hpp"
#include "Commands/Weapons/CommandNoSpread.hpp"
#include "Commands/Weapons/CommandProximityRockets.hpp"
#include "Commands/Weapons/CommandTriggerbot.hpp"
#include "Commands/Weapons/CommandVehicleRapidFire.hpp"
#include "Commands/Weapons/CommandDamageMultiplier.hpp"
#include "Commands/Weapons/CommandDisableCriticalHits.hpp"
#include "Commands/Weapons/CommandExplosionRadius.hpp"
#include "Commands/Weapons/CommandExplosionType.hpp"
#include "Commands/Weapons/CommandExplosiveAmmo.hpp"
#include "Commands/Weapons/CommandFillAmmo.hpp"
#include "Commands/Weapons/CommandImpactParticles.hpp"
#include "Commands/Weapons/CommandInfiniteAmmo.hpp"
#include "Commands/Weapons/CommandInfiniteClip.hpp"
#include "Commands/Weapons/CommandInfiniteParachutes.hpp"
#include "Commands/Weapons/CommandMeleeDamage.hpp"
#include "Commands/Weapons/CommandNoReload.hpp"
#include "Commands/Weapons/CommandRapidFire.hpp"
#include "Commands/Weapons/CommandRemoveAmmo.hpp"
#include "Commands/Weapons/CommandSuperMelee.hpp"
#include "Commands/Weapons/CommandWeaponDamage.hpp"
#include "Commands/Weapons/CommandWeaponsTools.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"
#include "Weapons/Weapon.hpp"
#include "World/Self.hpp"

#include <string>

namespace Stand
{
	class CommandWeaponSet : public CommandPhysical
	{
		std::vector<Hash> m_hashes;
		bool              m_give;
	public:
		explicit CommandWeaponSet(CommandList* const parent, Label&& name,
		                          std::vector<CommandName>&& cmds,
		                          std::vector<Hash>&& hashes, bool give)
			: CommandPhysical(COMMAND_ACTION, parent, std::move(name),
			                  std::move(cmds)),
			  m_hashes(std::move(hashes)), m_give(give)
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


	public:
		CommandFillAmmo* const            fillAmmo;
		CommandRemoveAmmo* const          removeAmmo;
		CommandInfiniteAmmo* const        infiniteAmmo;
		CommandInfiniteClip* const        infiniteClip;
		CommandNoReload* const            noReload;
		CommandInfiniteParachutes* const  infiniteParachutes;
		CommandRapidFire* const           rapidFire;
		CommandExplosiveAmmo* const       explosiveAmmo;
		CommandExplosionType* const       explosionType;
		CommandSliderFloat* const         explosionDamage;
		CommandSliderFloat* const         explosionCameraShake;
		CommandImpactParticles* const     impactParticles;
		CommandDamageMultiplier* const    damageMultiplier;
		CommandWeaponDamage* const        weaponDamage;
		CommandSliderFloat* const         weaponDamageScale;
		CommandSuperMelee* const          superMelee;
		CommandMeleeDamage* const         meleeDamage;
		CommandSliderFloat* const         meleeDamageScale;
		CommandExplosionRadius* const     explosionRadius;
		CommandSliderFloat* const         explosionRadiusScale;
		CommandDisableCriticalHits* const disableCriticalHits;
		CommandList* const                getWeapons;
		CommandList* const                removeWeapons;
		CommandGiveAllWeapons* const      giveAllWeapons;
		CommandGiveMaxAmmo* const         giveMaxAmmo;
		CommandOpenGunLockerTree* const   openGunLocker;
		CommandRangeMultiplier* const     rangeMultiplier;
		CommandNoSpread* const            noSpread;
		CommandNoRecoil* const            noRecoil;
		CommandNoSpooling* const          noSpooling;
		CommandAimbot* const              aimbot;
		CommandTriggerbot* const          triggerbot;
		CommandProximityRockets* const    proximityRockets;
		CommandVehicleRapidFire* const      vehicleRapidFire;
		CommandRocketSpeedMultiplier* const rocketSpeedMultiplier;
		CommandShotgunBatchSize* const      shotgunBatchSize;
		CommandInstantLockon* const         instantLockon;
		CommandInstantProxyDetonate* const  instantProxy;
		CommandGunFreedom* const            gunFreedom;
		CommandTeleportGun* const           teleportGun;
		CommandLaserSights* const           laserSights;
		CommandWhenShooting* const          whenShooting;
		CommandWhenAiming* const            whenAiming;
		CommandGravityGun* const            gravityGun;

		explicit CommandTabWeapons()
			: CommandList(nullptr, LIT("Weapons"), CMDNAMES("weapons")),
			  fillAmmo(createChild<CommandFillAmmo>()),
			  removeAmmo(createChild<CommandRemoveAmmo>()),
			  infiniteAmmo(createChild<CommandInfiniteAmmo>()),
			  infiniteClip(createChild<CommandInfiniteClip>()),
			  noReload(createChild<CommandNoReload>()),
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
			  impactParticles(createChild<CommandImpactParticles>()),
			  damageMultiplier(createChild<CommandDamageMultiplier>()),
			  weaponDamage(createChild<CommandWeaponDamage>()),
			  weaponDamageScale(createChild<CommandSliderFloat>(
			      LIT("Weapon Damage Scale"), CMDNAMES("weapondamagescale"),
			      NOLABEL, 0, 1000, 100)),
			  superMelee(createChild<CommandSuperMelee>()),
			  meleeDamage(createChild<CommandMeleeDamage>()),
			  meleeDamageScale(createChild<CommandSliderFloat>(
			      LIT("Melee Damage Scale"), CMDNAMES("meleedamagescale"),
			      NOLABEL, 0, 1000, 100)),
			  explosionRadius(createChild<CommandExplosionRadius>()),
			  explosionRadiusScale(createChild<CommandSliderFloat>(
			      LIT("Explosion Radius Scale"), CMDNAMES("explosionradiusscale"),
			      NOLABEL, 0, 1000, 100)),
			  disableCriticalHits(createChild<CommandDisableCriticalHits>()),
			  getWeapons(createChild<CommandList>(LIT("Get Weapons"), CMDNAMES("getgun"))),
			  removeWeapons(createChild<CommandList>(LIT("Remove Weapons"), CMDNAMES("removegun"))),
			  giveAllWeapons(createChild<CommandGiveAllWeapons>()),
			  giveMaxAmmo(createChild<CommandGiveMaxAmmo>()),
			  openGunLocker(createChild<CommandOpenGunLockerTree>()),
			  rangeMultiplier(createChild<CommandRangeMultiplier>()),
			  noSpread(createChild<CommandNoSpread>()),
			  noRecoil(createChild<CommandNoRecoil>()),
			  noSpooling(createChild<CommandNoSpooling>()),
			  aimbot(createChild<CommandAimbot>()),
			  triggerbot(createChild<CommandTriggerbot>()),
			  proximityRockets(createChild<CommandProximityRockets>()),
			  vehicleRapidFire(createChild<CommandVehicleRapidFire>()),
			  rocketSpeedMultiplier(createChild<CommandRocketSpeedMultiplier>()),
			  shotgunBatchSize(createChild<CommandShotgunBatchSize>()),
			  instantLockon(createChild<CommandInstantLockon>()),
			  instantProxy(createChild<CommandInstantProxyDetonate>()),
			  gunFreedom(createChild<CommandGunFreedom>()),
			  teleportGun(createChild<CommandTeleportGun>()),
			  laserSights(createChild<CommandLaserSights>()),
			  whenShooting(createChild<CommandWhenShooting>()),
			  whenAiming(createChild<CommandWhenAiming>()),
			  gravityGun(createChild<CommandGravityGun>())
		{
			explosiveAmmo->m_type   = explosionType;
			explosiveAmmo->m_damage = explosionDamage;
			explosiveAmmo->m_shake  = explosionCameraShake;
			weaponDamage->m_scale   = weaponDamageScale;
			meleeDamage->m_scale    = meleeDamageScale;
			explosionRadius->m_scale = explosionRadiusScale;

			auto populate = [this](CommandList* list, bool give)
			{
				list->createChild<CommandWeaponSet>(LIT("All Weapons"),
					give ? CMDNAMES("allguns") : CMDNAMES("noguns"),
					Weapon::getAllHashes(), give);

				Weapon::Category currentCat = static_cast<Weapon::Category>(-1);
				CommandList* catList = nullptr;
				for (const auto& w : Weapon::weapons)
				{
					if (w.category != currentCat)
					{
						currentCat = w.category;
						catList = list->createChild<CommandList>(
							LIT(Weapon::getCategoryName(currentCat)),
							give ? CMDNAMES_1(Weapon::getCategoryGetCmd(currentCat))
							     : CMDNAMES_1(Weapon::getCategoryRemCmd(currentCat)));
					}
					const std::string cmd = (give ? "get" : "remove") + std::string(w.cmd_suffix);
					catList->createChild<CommandWeaponSet>(LIT(w.name),
						std::vector<CommandName>{cmd},
						std::vector<Hash>{w.hash}, give);
				}
			};
			populate(getWeapons, true);
			populate(removeWeapons, false);
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabWeapons& GetCommandTabWeapons();
}
