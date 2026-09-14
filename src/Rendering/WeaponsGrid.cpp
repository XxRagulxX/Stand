#include "Rendering/WeaponsGrid.hpp"

#include "Rendering/AmmuNationGrid.hpp"
#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/Weapons/CommandCustomWeapon.hpp"
#include "Commands/Commands.hpp"
#include "Commands/Self/CommandTabWeapons.hpp"
#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemCommandSlider.hpp"
#include "Rendering/GridItemCommandListSelect.hpp"
#include "Rendering/GridItemCommandInput.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/GridItemText.hpp"
#include "Util/Joaat.hpp"
#include "Commands/CommandListSelect.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// bool IsCustomWeaponEnabled()
		// {
		// 	auto* customweapon = Commands::GetCommand<CommandToggleLegacy>("customweapon"_J);
		// 	return customweapon && customweapon->GetState();
		// }

		// Features::CustomWeapons CustomWeaponType()
		// {
		// 	auto* cmd = Commands::GetCommand<CommandListSelect>("customweapontype"_J);
		// 	return static_cast<Features::CustomWeapons>(cmd ? cmd->GetState() : 0);
		// }

		// bool IsGravityGunRow()
		// {
		// 	return IsCustomWeaponEnabled() && CustomWeaponType() == Features::CustomWeapons::GRAVITY_GUN;
		// }

		// bool IsVehicleGunRow()
		// {
		// 	return IsCustomWeaponEnabled() && CustomWeaponType() == Features::CustomWeapons::VEHICLE_GUN;
		// }

		// bool IsPaintGunTypeRow()
		// {
		// 	return IsCustomWeaponEnabled() && CustomWeaponType() == Features::CustomWeapons::PAINT_GUN;
		// }

		// bool IsPaintGunColorRow()
		// {
		// 	if (!IsPaintGunTypeRow())
		// 		return false;

		// 	auto* rainbow = Commands::GetCommand<CommandToggleLegacy>("paintgunrainbowcolorenabled"_J);
		// 	return !(rainbow && rainbow->GetState());
		// }

		// bool IsPaintGunRainbowRow()
		// {
		// 	if (!IsPaintGunTypeRow())
		// 		return false;

		// 	auto* rainbow = Commands::GetCommand<CommandToggleLegacy>("paintgunrainbowcolorenabled"_J);
		// 	return rainbow && rainbow->GetState();
		// }
	}

	WeaponsGrid::WeaponsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void WeaponsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabWeapons();

		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.aimbot));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.triggerbot));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.proximityRockets));


		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.fillAmmo));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.removeAmmo));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.infiniteAmmo));
		// items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.infiniteClip));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.noReload));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.noSpread));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.noRecoil));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.noSpooling));
		// items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.infiniteParachutes));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.rapidFire));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.vehicleRapidFire));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.weaponDamage));
		if (watchCondition([cmd = tab.weaponDamage] { return cmd->m_on; }))
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.weaponDamageScale));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.rangeMultiplier));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.rocketSpeedMultiplier));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.superMelee));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.meleeDamage));
		if (watchCondition([cmd = tab.meleeDamage] { return cmd->m_on; }))
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.meleeDamageScale));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.shotgunBatchSize));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.instantLockon));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.instantProxy));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.gunFreedom));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.teleportGun));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.laserSights));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.explosiveAmmo));
		if (watchCondition([cmd = tab.explosiveAmmo] { return cmd->m_on; }))
		{
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.explosionType));
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.explosionDamage));
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.explosionCameraShake));
		}

		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.impactParticles));



		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.explosionRadius));
		if (watchCondition([cmd = tab.explosionRadius] { return cmd->m_on; }))
			items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.explosionRadiusScale));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.disableCriticalHits));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.giveAllWeapons));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.openGunLocker));


		// items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Ammu-Nation", Theme::kText));
		// AddAmmuNationRows(*this, items_draft, Theme::kContentWidth);
		// items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Gun Van", Theme::kText));
		// items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "gunvan_weapon"_J));
		// items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "gunvan_slot"_J));
		// items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "gunvan_apply"_J));
		// items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Custom Weapons", Theme::kText));
		// items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "customweapon"_J));
		// if (watchCondition(IsCustomWeaponEnabled))
		// 	items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "customweaponenabledonweaponout"_J));
		// if (watchCondition(IsCustomWeaponEnabled))
		// 	items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "customweapontype"_J));
		// if (watchCondition(IsGravityGunRow))
		// 	items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "gravitygunlaunchonrelease"_J));
		// if (watchCondition(IsVehicleGunRow))
		// 	items_draft.push_back(std::make_unique<GridItemCommandInput>(Theme::kContentWidth, kItemH, "vehiclegunmodel"_J));
		// AddConditionalColorCommandRows(*this, items_draft, Theme::kContentWidth, "paintguncolor"_J, IsPaintGunColorRow);
		// if (watchCondition(IsPaintGunTypeRow))
		// 	items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "paintgunrainbowcolorenabled"_J));
		// if (watchCondition(IsPaintGunRainbowRow))
		// 	items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "paintgunrainbowcolorstyle"_J));
		// if (watchCondition(IsPaintGunRainbowRow))
		// 	items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "paintgunrainbowcolorspeed"_J));
	}
}
