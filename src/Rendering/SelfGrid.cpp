#include "Rendering/SelfGrid.hpp"

#include "Commands/Self/Movement/CommandTabFloppy.hpp"
#include "Commands/Self/Movement/CommandTabLevitation.hpp"
#include "Commands/Self/Movement/CommandTabMovement.hpp"
#include "Commands/Self/CommandTabSelf.hpp"
#include "Commands/Self/Movement/CommandTabSuperFlight.hpp"
#include "Commands/Self/Appearance/CommandTabTransform.hpp"
#include "Commands/Self/Weapons/CommandTabWeapons.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/GridStandCommandList.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/TransformModelListGrid.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		class LevitationSubGrid : public Grid
		{
		public:
			LevitationSubGrid() : Grid(Theme::GetContentOrigin(), 0) {}
		protected:
			void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
			{
				auto& tab = Features::GetCommandTabLevitation();
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.levitate));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.presets));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.speed));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.sprintMultiplier));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.accel));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.ignorePitch));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.keepMomentum));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.applyPitch));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.onlyRotateOnMovement));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.buttonInstructions));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.passiveMin));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.passiveMax));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.passiveSpeed));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.assistUp));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.assistDown));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.assistDeadzone));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.assistSnap));
			}
		};

		class SuperFlightSubGrid : public Grid
		{
		public:
			SuperFlightSubGrid() : Grid(Theme::GetContentOrigin(), 0) {}
		protected:
			void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
			{
				auto& tab = Features::GetCommandTabSuperFlight();
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.mode));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.speed));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.jumpToFlight));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.controls));
			}
		};

		class FloppySubGrid : public Grid
		{
		public:
			FloppySubGrid() : Grid(Theme::GetContentOrigin(), 0) {}
		protected:
			void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
			{
				auto& tab = Features::GetCommandTabFloppy();
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.floppy));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.speed));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.sprint));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.instructions));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.synch));
			}
		};

		class MovementGrid : public Grid
		{
			LevitationSubGrid  m_levitation{};
			SuperFlightSubGrid m_superFlight{};
			FloppySubGrid      m_floppy{};
		public:
			MovementGrid() : Grid(Theme::GetContentOrigin(), 0) {}
		protected:
			void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
			{
				auto& tab = Features::GetCommandTabMovement();
				items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Levitation",  &m_levitation));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.walkSpeed));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.friction));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.gracefulLanding));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.superJump));
				items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Super Flight", &m_superFlight));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.walkUnderwater));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.waterWalk));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.swimSpeed));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.superRun));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.totalFreedom));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.walkOnAir));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.swimInAir));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.drunkMode));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.tennisMode));
				items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Floppy Mode",  &m_floppy));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.reducedCollision));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.noCollision));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.selfFreeze));
			}
		};

		class TransformSubGrid : public Grid
		{
		public:
			TransformSubGrid() : Grid(Theme::GetContentOrigin(), 0) {}
		protected:
			void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
			{
				auto& tab = Features::GetCommandTabTransform();
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.findModel));
				items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Playable Characters", &TransformModelListGrid::GetOrCreate(tab.playable)));
				items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Ground Animals",      &TransformModelListGrid::GetOrCreate(tab.groundAnimals)));
				items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Water Animals",       &TransformModelListGrid::GetOrCreate(tab.waterAnimals)));
				items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Flying Animals",      &TransformModelListGrid::GetOrCreate(tab.flyingAnimals)));
				items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "NPCs",                &TransformModelListGrid::GetOrCreate(tab.npcs)));
			}
		};

		class AppearanceGrid : public Grid
		{
			TransformSubGrid m_transform{};
		public:
			AppearanceGrid() : Grid(Theme::GetContentOrigin(), 0) {}
		protected:
			void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
			{
				auto& self = Features::GetCommandTabSelf();
				items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Transform", &m_transform));
				items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Outfit",    &GridStandCommandList::GetOrCreate(self.outfit)));
				for (auto& child : self.character->children)
					items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, child.get()));
			}
		};

		class WeaponsGrid : public Grid
		{
		public:
			WeaponsGrid() : Grid(Theme::GetContentOrigin(), 0) {}
		protected:
			void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
			{
				auto& wpn = Features::GetCommandTabWeapons();
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.getWeapons));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.removeWeapons));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.whenShooting));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.whenAiming));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.gravityGun));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.vehicleGun));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.weaponComponents));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.weaponTint));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.weaponTintRainbow));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.aimbot));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.triggerbot));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.proximityRockets));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.fillAmmo));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.removeAmmo));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.infiniteAmmo));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.noReload));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.noSpread));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.noRecoil));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.noSpooling));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.rapidFire));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.vehicleRapidFire));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.damageMultiplier));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.rangeMultiplier));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.rocketSpeedMultiplier));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.superMelee));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.shotgunBatchSize));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.instantLockon));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.instantProxy));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.explosiveAmmo));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.explosionType));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.impactParticles));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.impactParticleSelect));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.gunFreedom));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.teleportGun));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.laserSights));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.openGunLocker));
				items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, wpn.gunVan));
			}
		};

		MovementGrid  g_movement{};
		AppearanceGrid g_appearance{};
		WeaponsGrid   g_weapons{};
	}

	SelfGrid::SelfGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SelfGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& self = Features::GetCommandTabSelf();

		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Movement",   &g_movement));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Appearance", &g_appearance));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Weapons",    &g_weapons));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.bodyguards));

		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.god));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.autoHeal));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.maxHealth));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.grace));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.seatGlue));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.wanted));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.freezeWanted));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.fakeWanted));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.infiniteStamina));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.paralock));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.clumsiness));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.respawnRecall));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.respawnDelay));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.regenerationRate));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.refillHealth));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.refillArmour));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, self.suicide));
	}
}
