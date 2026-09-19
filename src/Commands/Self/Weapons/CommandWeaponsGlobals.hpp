#pragma once
#include "Commands/Self/Weapons/CommandDisableCriticalHits.hpp"
#include "Commands/Self/Weapons/CommandExplosionRadius.hpp"
#include "Commands/Self/Weapons/CommandExplosionType.hpp"
#include "Commands/Self/Weapons/CommandExplosiveAmmo.hpp"
#include "Commands/Self/Weapons/CommandInfiniteAmmo.hpp"
#include "Commands/Self/Weapons/CommandInfiniteClip.hpp"
#include "Commands/Self/Weapons/CommandInfiniteParachutes.hpp"
#include "Commands/Self/Weapons/CommandMeleeDamage.hpp"
#include "Commands/Self/Weapons/CommandRapidFire.hpp"
#include "Commands/Self/Weapons/CommandWeaponDamage.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandWeaponsGlobals : public CommandList
	{
	public:
		explicit CommandWeaponsGlobals(CommandList* const parent)
			: CommandList(parent, LIT("Globals"), CMDNAMES("weaponglobals"))
		{
			createChild<CommandInfiniteAmmo>();
			createChild<CommandInfiniteClip>();
			createChild<CommandRapidFire>();
			createChild<CommandInfiniteParachutes>();

			auto* expammo    = createChild<CommandExplosiveAmmo>();
			expammo->m_type  = createChild<CommandExplosionType>();
			expammo->m_damage = createChild<CommandSliderFloat>(
				LIT("Explosion Damage Scale"), CMDNAMES("explosiondamage"),
				NOLABEL, 0, 100000, 100);
			expammo->m_shake  = createChild<CommandSliderFloat>(
				LIT("Explosion Camera Shake"), CMDNAMES("explosioncamerashake"),
				NOLABEL, 0, 1000, 10);

			auto* wdmg   = createChild<CommandWeaponDamage>();
			wdmg->m_scale = createChild<CommandSliderFloat>(
				LIT("Weapon Damage Scale"), CMDNAMES("weapondamagescale"),
				NOLABEL, 0, 1000, 100);

			auto* melee   = createChild<CommandMeleeDamage>();
			melee->m_scale = createChild<CommandSliderFloat>(
				LIT("Melee Damage Scale"), CMDNAMES("meleedamagescale"),
				NOLABEL, 0, 1000, 100);

			auto* exprad   = createChild<CommandExplosionRadius>();
			exprad->m_scale = createChild<CommandSliderFloat>(
				LIT("Explosion Radius Scale"), CMDNAMES("explosionradiusscale"),
				NOLABEL, 0, 1000, 100);

			createChild<CommandDisableCriticalHits>();
		}
	};
}
